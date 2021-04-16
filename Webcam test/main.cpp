
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
#include <chrono>
#include "play_audio.cpp"

#define DISPLAY_EAR false

using namespace cv;
using namespace std;
using namespace dlib;
using namespace std::chrono;

double const SCALE_FACTOR = 0.2;
float EAR_THRESH = 0.24;
float TIME_THRESHOLD = 1.0;
float ALPHA = 1 / 10.0;

// initial value for accumulator
float accumulator = 1.0;
CascadeClassifier face_cascade;
shape_predictor sp;

int euler_distance(dlib::point p1, dlib::point p2);
float get_EAR(full_object_detection landmarks);
float calibrate(VideoCapture cap);
static dlib::rectangle openCVRectToDlib(cv::Rect r);

int main(int argc, char **argv)
{

    String face_cascade_name = "../static/haarcascade_frontalface_alt.xml";
    String shape_predictor_path = "../static/shape_predictor_68_face_landmarks.dat";
    // Load the shape predictor for facial landmark detection
    deserialize(shape_predictor_path) >> sp;

    if (!face_cascade.load(face_cascade_name))
    {
        cout << "--(!)Error loading face cascade\n";
        return -1;
    };
    // This can also be used with a video input: VideoCapture cap("PATH_TO_VIDEO")
    VideoCapture cap;
    // Open the default camera, use something different if system has more cameras;
    int deviceID = 0;        // 0 = open default camera
    int apiID = cv::CAP_ANY; // 0 = autodetect default API
    // Try to open camera
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

    // Load audio files and initialise playback
    init_playback();

    cout << "Resolution of the video : " << dWidth << " x " << dHeight << " fps " << fps << endl;
    Mat original_frame, scaled_frame, grayscale_frame;

    // time_t start_thresh, end_thresh;
    // time(&start_thresh);
    auto start_thresh = steady_clock::now();
    // time_point<system_clock> start_thresh, end_thresh;

    std::thread thread1(play_calibration_start);
    thread1.detach();

    // CALIBRATION GOES HERE

    // calibrate(cap);
    sleep(100);
    std::thread thread2(play_calibartion_completed);
    thread2.detach();

    std::thread thread3(init_alarm);
    thread3.detach();

    // Start main loop

    while (true)
    {
        auto start_fps = steady_clock::now();
        // Wait for a new frame from camera and store it into 'frame'
        cap.read(original_frame);
        if (original_frame.empty())
        {
            cerr << "ERROR! blank frame grabbed\n";
            break;
        }

        // Resize image to be smaller for faster processing, the amount is determined by SCALE_FACTOR
        resize(original_frame, scaled_frame, Size(), SCALE_FACTOR, SCALE_FACTOR, INTER_LINEAR);

        // Convert the image to grayscale for processing
        cvtColor(scaled_frame, grayscale_frame, COLOR_BGR2GRAY);
        // Equalise the histogram (normalises brightness and increases contrast of the image)
        equalizeHist(grayscale_frame, grayscale_frame);

        std::vector<Rect> faces;
        std::vector<full_object_detection> shapes;
        dlib::array2d<unsigned char> dlib_img;
        assign_image(dlib_img, cv_image<unsigned char>(grayscale_frame));
        dlib::rectangle face_dlib;
        // detect faces in image and put the corresponding rectangles into a vector
        face_cascade.detectMultiScale(grayscale_frame, faces, 1.1, 3, 0, Size(30, 30));
        // Determine the number of faces detected
        unsigned int face_count = faces.size();
        cv::Rect face;
        if (face_count == 0)
        {
            cout << "Face not detected" << endl;
        }
        // If more than one face is found, take the biggest one
        else if (face_count > 1)
        {
            cout << "Expected only 1 face" << endl;
            int max_size = faces[0].area();
            int max_index = 0;
            for (int i = 1; i < face_count; i++)
            {
                if (faces[i].area() > max_size)
                {
                    max_size = faces[i].area();
                    max_index = i;
                }
                face = faces[max_index];
            }
        }
        else if (face_count == 1)
        {
            // take the single face found
            face = faces[0];

            // Display rectange of detected face
            cv::rectangle(scaled_frame, face, Scalar(255, 0, 0), 1, 8, 0);
            // Convert OpenCV rectangle object to dlib rectange object
            face_dlib = openCVRectToDlib(face);
            // Predict facial landmark locations and store it in shapes object
            shapes.push_back(sp(dlib_img, face_dlib));
            // get EAR
            float avg_EAR = get_EAR(shapes[0]);

            // ALPHA = 1 / (real_fps * TIME_THRESHOLD);

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

            // Convert dblib points to OpenCV Points and store in vector
            std::vector<std::vector<Point>> eye_countours(2);
            for (int i = 36; i <= 41; i++)
            {
                eye_countours[0].push_back(cv::Point(shapes[0].part(i).x(), shapes[0].part(i).y()));
            }
            for (int i = 42; i <= 47; i++)
            {
                eye_countours[1].push_back(cv::Point(shapes[0].part(i).x(), shapes[0].part(i).y()));
            }

            // Set eye outline color
            Scalar color = Scalar(255, 255, 0);
            // Show eye detection
            drawContours(scaled_frame, eye_countours, -1, color, 1);

            accumulator = ALPHA * avg_EAR + (1.0 - ALPHA) * accumulator;

            if (accumulator < EAR_THRESH)
            {
                // time(&end_thresh);
                auto end_thresh = steady_clock::now();
                double elapsed_seconds = duration_cast<duration<double>>(end_thresh - start_thresh).count();
                // cout << "seconds " << elapsed_seconds << "\n";
                if (elapsed_seconds >= TIME_THRESHOLD)
                {
                    cv::putText(scaled_frame,
                                "SLEEPINESS DETECTED",
                                cv::Point(scaled_frame.cols * 0.05, scaled_frame.rows * 0.15),
                                cv::FONT_HERSHEY_PLAIN,
                                1,
                                CV_RGB(255, 0, 0),
                                2);
                    // Play only when alarm sample is initialised and don't call new threads if the alarm is already playing
                    if (!alarmON && alarmReady)
                    {
                        std::thread thre_obj(start_alarm);
                        // Can be detached, because it will loop until stop signal is sent
                        thre_obj.detach();
                    }
                }
            }
            else
            {
                start_thresh = steady_clock::now();
                // Send signal to stop alarm only if it is on
                if (alarmON)
                {
                    stop_playing();
                    cout << "EAR was " << accumulator << "\n";
                }
            }
            auto end_fps = steady_clock::now();
            // Time to process one frame
            double time_for_cycle = duration_cast<duration<double>>(end_fps - start_fps).count();
            // frames processed per second
            double real_fps = 1.0 / time_for_cycle;
            // cout << "fps " << 1.0 / time_for_cycle << "\n";
        }
        imshow("Live", scaled_frame);

        // cout << "cycle time " << time_for_cycle << "\n";
        // cout << "fps " << 1.0 / time_for_cycle << "\n";

        // imshow("gray", grayscale_frame);

        if (waitKey(1) >= 0)
            break;
    }

    return 0;
}

float get_EAR(full_object_detection landmarks)
{
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