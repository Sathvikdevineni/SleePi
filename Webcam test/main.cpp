
#include <iostream>
#include <sstream>
#include <dlib/opencv.h>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>
#include <dlib/image_io.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

#include <time.h>

using namespace cv;
using namespace std;
using namespace dlib;

double const SCALE_FACTOR = 0.2;
unsigned int COUNTER = 0;
float EAR_THRESH = 0.23;
int FRAME_THRESH = 60;
float TIME_THRESHOLD = 1.0;

CascadeClassifier face_cascade;
shape_predictor sp;

int euler_distance(dlib::point p1, dlib::point p2);
float get_EAR(full_object_detection landmarks);
static dlib::rectangle openCVRectToDlib(cv::Rect r);

int main(int argc, char **argv)
{

    cout << "tesdddddt\n";
    //can also be used for video input: VideoCapture cap("PATH_TO_VIDEO")
    // String face_cascade_name = samples::findFile("../static/haarcascade_frontalface_alt.xml");
    String face_cascade_name = "../static/haarcascade_frontalface_alt.xml";
    String shape_predictor_path = "../static/shape_predictor_68_face_landmarks.dat";

    deserialize(shape_predictor_path) >> sp;

    if (!face_cascade.load(face_cascade_name))
    {
        cout << "--(!)Error loading face cascade\n";
        return -1;
    };

    VideoCapture cap;
    // VideoCapture cap("../static/test_vid.mp4");
    // open the default camera, use something different from 0 otherwise;
    // Check VideoCapture documentation.
    int deviceID = 0;        // 0 = open default camera
    int apiID = cv::CAP_ANY; // 0 = autodetect default API
    cap.open(deviceID, apiID);
    if (!cap.isOpened())
    {
        cerr << "ERROR! Unable to open camera\n";
        return -1;
    }

    cout << "Start grabbing" << endl
         << "Press any key to terminate" << endl;

    cap.set(CAP_PROP_FRAME_WIDTH, 1080);
    cap.set(CAP_PROP_FRAME_HEIGHT, 1920);
    double fps = cap.get(CAP_PROP_FPS);
    double dWidth = cap.get(CAP_PROP_FRAME_WIDTH);   //get the width of frames of the video
    double dHeight = cap.get(CAP_PROP_FRAME_HEIGHT); //get the height of frames of the video

    cout << "Resolution of the video : " << dWidth << " x " << dHeight << " fps " << fps << endl;
    // image_window win;
    Mat original_frame, scaled_frame, grayscale_frame;
    clock_t start = clock();
    while (true)
    {
        // wait for a new frame from camera and store it into 'frame'
        cap.read(original_frame);
        // check if we succeeded
        if (original_frame.empty())
        {
            cerr << "ERROR! blank frame grabbed\n";
            break;
        }

        // resize image to be smaller for faster processing, the amount is determined by SCALE_FACTOR
        resize(original_frame, scaled_frame, Size(), SCALE_FACTOR, SCALE_FACTOR, INTER_LINEAR);

        // Convert the image to grayscale for processing
        cvtColor(scaled_frame, grayscale_frame, COLOR_BGR2GRAY);
        // equalise the histogram (normalises brightness and increases contrast of the image)
        equalizeHist(grayscale_frame, grayscale_frame);

        std::vector<Rect> faces;
        std::vector<full_object_detection> shapes;
        // cv_image<bgr_pixel> cimg(processed);
        dlib::array2d<unsigned char> dlib_img;
        assign_image(dlib_img, cv_image<unsigned char>(grayscale_frame));
        dlib::rectangle face_dlib;
        // detect faces in image and put the corresponding rectangles into a vector
        face_cascade.detectMultiScale(grayscale_frame, faces);
        // Determine the number of faces detected
        unsigned int face_count = faces.size();
        if (face_count == 0)
        {
            cout << "Face not detected" << endl;
            // win.clear_overlay();
            // return 0;
        }
        else if (face_count > 1)
        {
            cout << "Expected only 1 face" << endl;
            // win.clear_overlay();
        }
        else if (face_count == 1)
        {

            // Display rectange of detected face
            cv::rectangle(scaled_frame, faces[0], Scalar(255, 0, 0), 1, 8, 0);
            // Convert OpenCV rectangle object to dlib rectange object
            face_dlib = openCVRectToDlib(faces[0]);
            // Predict facial landmark locations and store it in shapes object
            shapes.push_back(sp(dlib_img, face_dlib));
            // }
            // win.clear_overlay();
            // get EAR
            float avg_EAR = get_EAR(shapes[0]);
            // Convert EAR to string
            std::stringstream stream;
            stream << std::fixed << std::setprecision(2) << avg_EAR;
            std::string s = stream.str();

            // Show EAR on screen
            cv::putText(scaled_frame,
                        "EAR: " + s,
                        cv::Point(scaled_frame.cols * 0.45, scaled_frame.rows * 0.05),
                        cv::FONT_HERSHEY_PLAIN,
                        1,
                        CV_RGB(255, 0, 0),
                        2);

            // Convert dblib points to opencv
            std::vector<std::vector<Point>>
                eye_countours(2);
            // std::vector<std::vector<Point>> hull(2);
            // std::vector<Point> left_eye_contour, right_eye_contour;
            for (int i = 36; i <= 41; i++)
            {
                eye_countours[0].push_back(cv::Point(shapes[0].part(i).x(), shapes[0].part(i).y()));
            }
            for (int i = 42; i <= 47; i++)
            {
                eye_countours[1].push_back(cv::Point(shapes[0].part(i).x(), shapes[0].part(i).y()));
            }

            Scalar color = Scalar(255, 255, 0);

            // Show eye detection
            drawContours(scaled_frame, eye_countours, -1, color, 1);

            if (avg_EAR < EAR_THRESH)
            {
                clock_t finish = clock();
                double time_past = (double)(finish - start);

                if (time_past / 1000.0 >= TIME_THRESHOLD)
                {
                    cv::putText(scaled_frame,
                                "SLEEPINESS DETECTED",
                                cv::Point(scaled_frame.cols * 0.05, scaled_frame.rows * 0.15),
                                cv::FONT_HERSHEY_PLAIN,
                                1,
                                CV_RGB(255, 0, 0),
                                2);
                }
            }

            else
            {
                start = clock();
                // stop_playing();
            }
        }
        imshow("Live", scaled_frame);
        // imshow("gray", grayscale_frame);

        if (waitKey(5) >= 0)
            break;
    }

    return 0;
}

float get_EAR(full_object_detection landmarks)
{
    // Since the aspect ratio is calculated there is no need to get the actual euclidian distance, ratio of squares gives the same result
    int left_height1 = euler_distance(landmarks.part(37), landmarks.part(41));
    int left_height2 = euler_distance(landmarks.part(38), landmarks.part(40));
    int left_length = euler_distance(landmarks.part(36), landmarks.part(39));
    float left_EAR = (left_height1 + left_height2) / (2.0 * left_length);

    int right_height1 = euler_distance(landmarks.part(43), landmarks.part(47));
    int right_height2 = euler_distance(landmarks.part(44), landmarks.part(46));
    int right_length = euler_distance(landmarks.part(42), landmarks.part(45));
    float right_EAR = (right_height1 + right_height2) / (2.0 * right_length);

    float average_EAR = (left_EAR + right_EAR) / 2.0;
    // cout<<"right EAR "<<right_EAR<<"\n"<<"left EAR "<<left_EAR<<"\n"<<"average EAR "<<average_EAR<<"\n";
    return average_EAR;
}

int euler_distance(dlib::point p1, dlib::point p2)
{
    return sqrt(pow(p1.x() - p2.x(), 2) + pow(p1.y() - p2.y(), 2));
}

static dlib::rectangle openCVRectToDlib(cv::Rect r)
{
    return dlib::rectangle((long)r.tl().x, (long)r.tl().y, (long)r.br().x - 1, (long)r.br().y - 1);
    // return dlib::rectangle((long)r.tl().x, (long)r.tl().y, (long)r.br().x, (long)r.br().y);
}