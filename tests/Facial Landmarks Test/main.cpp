
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

using namespace cv;
using namespace std;
using namespace dlib;
using namespace std::chrono;

CascadeClassifier face_cascade;
shape_predictor sp;
VideoWriter outputVideo;

float SCALE_FACTOR = 0.5;

int euler_distance(dlib::point p1, dlib::point p2);
float get_EAR(full_object_detection landmarks);
float get_threshold(VideoCapture cap);
static dlib::rectangle openCVRectToDlib(cv::Rect r);
int detect_face(Mat frame, cv::Rect &face);
void draw_EAR(Mat frame, float EAR);
void draw_eye_contours(Mat frame, full_object_detection shapes);

// Main function for executing the program
int main(int argc, char **argv)
{
    // Set the relative locations of face cascade and shape perdictor
    String face_cascade_name = "../../../SleePi/static/haarcascade_frontalface_alt.xml";
    String shape_predictor_path = "../../../SleePi/static/shape_predictor_68_face_landmarks.dat";
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
    // Does not work when using with the camera module
    // cap.set(CAP_PROP_FRAME_WIDTH, 1080);
    // cap.set(CAP_PROP_FRAME_HEIGHT, 1920);
    double fps = cap.get(CAP_PROP_FPS);
    double dWidth = cap.get(CAP_PROP_FRAME_WIDTH);   //get the width of frames of the video
    double dHeight = cap.get(CAP_PROP_FRAME_HEIGHT); //get the height of frames of the video

    Mat original_frame, scaled_frame, grayscale_frame;
    // Start main loop
    while (true)
    {

        // Wait for a new frame from camera
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

        cv::Rect face;
        // gets a face as a rectangle object, returns 1 if face was not found
        int result = detect_face(grayscale_frame, face);
        // No face was detected
        if (result == 1)
        {

            cv::putText(scaled_frame,
                        "NO FACE DETECTED",
                        cv::Point(scaled_frame.cols * 0.3, scaled_frame.rows * 0.6),
                        cv::FONT_HERSHEY_PLAIN,
                        1,
                        CV_RGB(255, 0, 0),
                        2);
        }
        // Got 1 face
        else if (result == 0)
        {
            // Display rectange of detected face

            cv::rectangle(scaled_frame, face, Scalar(255, 0, 0), 1, 8, 0);

            full_object_detection shapes;
            dlib::array2d<unsigned char> dlib_img;
            // convert frame to dlib object
            assign_image(dlib_img, cv_image<unsigned char>(grayscale_frame));
            dlib::rectangle face_dlib;
            // Convert OpenCV rectangle object to dlib rectange object
            face_dlib = openCVRectToDlib(face);
            // Predict facial landmark locations and store it in shapes object
            shapes = sp(dlib_img, face_dlib);

            for (int i = 0; i <= 68; i++)
            {
                cv::circle(scaled_frame,
                           Point(shapes.part(i).x(), shapes.part(i).y()),
                           1,
                           CV_RGB(255, 0, 0));
            }
        }

        cv::imshow("Live", scaled_frame);
        // Stop on any keypress (required for imshow function)
        if (waitKey(1) >= 0)
            break;
    }

    return 0;
}
// Finds a face in a frame and stores it in a cv::Rect object
// Returns 1 if no face detected and 0 if 1 or more faces detected
int detect_face(Mat frame, cv::Rect &face)
{
    std::vector<Rect> faces;
    // detect faces in image and put the corresponding rectangles into a vector
    face_cascade.detectMultiScale(frame, faces, 1.1, 3, 0, Size(30, 30));
    // Determine the number of faces detected
    unsigned int face_count = faces.size();
    // If no faces detected return 1
    if (face_count == 0)
    {
        return 1;
    }
    // If more than one face is found, take the biggest one
    else if (face_count > 1)
    {
        cout << "Expected only 1 face\n";
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
    // take the single face found
    else if (face_count == 1)
    {
        face = faces[0];
    }
    return 0;
}
// Draws EAR value on a frame
void draw_EAR(Mat frame, float EAR)
{
    // Convert EAR to string
    std::stringstream stream;
    stream << std::fixed << std::setprecision(2) << EAR;
    std::string s = stream.str();

    // Show EAR on screen
    cv::putText(frame,
                "EAR: " + s,
                cv::Point(frame.cols * 0.6, frame.rows * 0.05),
                cv::FONT_HERSHEY_PLAIN,
                1,
                CV_RGB(255, 0, 0),
                2);
}
// Converts OpenCV rectengle to dlib rectangle
static dlib::rectangle openCVRectToDlib(cv::Rect r)
{
    return dlib::rectangle((long)r.tl().x, (long)r.tl().y, (long)r.br().x - 1, (long)r.br().y - 1);
}