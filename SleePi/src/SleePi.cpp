
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

#include "play_audio.h"
#include "config.h"

using namespace cv;
using namespace std;
using namespace dlib;
using namespace std::chrono;

CascadeClassifier face_cascade;
shape_predictor sp;
VideoWriter outputVideo;

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
    // Does not work when using with the camera module
    // cap.set(CAP_PROP_FRAME_WIDTH, 1080);
    // cap.set(CAP_PROP_FRAME_HEIGHT, 1920);
    double fps = cap.get(CAP_PROP_FPS);
    double dWidth = cap.get(CAP_PROP_FRAME_WIDTH);   //get the width of frames of the video
    double dHeight = cap.get(CAP_PROP_FRAME_HEIGHT); //get the height of frames of the video
    // Prepares the information for video saving
    if (SAVE_TO_FILE)
    {
        Size vid_out = Size((int)dWidth * SCALE_FACTOR, (int)dHeight * SCALE_FACTOR);
        outputVideo.open("./capture.avi", VideoWriter::fourcc('D', 'I', 'V', 'X'), 24, vid_out, true);
        if (!outputVideo.isOpened())
        {
            cerr << "Could not open the output video file for write\n";
            return -1;
        }
    }
    // Load audio files and initialise playback
    init_playback();

    Mat original_frame, scaled_frame, grayscale_frame;
    float accumulator = 1.0;
    auto start_thresh = steady_clock::now();
    // Detaching because the capture can start as the instruction is being played
    std::thread thread1(play_calibration_start);
    thread1.detach();
    // Set the dynamic threhold for sleepiness
    float thresh = get_threshold(cap);
    cout << "thresh got " << thresh << "\n";
    // Play the sound of calibration completion
    std::thread thread2(play_calibartion_completed);
    thread2.detach();
    // Initialise the alarm file for playback
    std::thread thread3(init_alarm);
    thread3.detach();

    // Start main loop
    while (true)
    {
        // Start clock for counting the time for executing the loop
        auto start_fps = steady_clock::now();

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
            if ((SHOW_VIDEO_OUTPUT || SAVE_TO_FILE))
            {
                cv::putText(scaled_frame,
                            "NO FACE DETECTED",
                            cv::Point(scaled_frame.cols * 0.3, scaled_frame.rows * 0.6),
                            cv::FONT_HERSHEY_PLAIN,
                            1,
                            CV_RGB(255, 0, 0),
                            2);
            }
        }
        // Got 1 face
        else if (result == 0)
        {
            // Display rectange of detected face
            if ((SHOW_VIDEO_OUTPUT || SAVE_TO_FILE) && SHOW_FACE_DETECTION)
            {
                cv::rectangle(scaled_frame, face, Scalar(255, 0, 0), 1, 8, 0);
            }
            full_object_detection shapes;
            dlib::array2d<unsigned char> dlib_img;
            // convert frame to dlib object
            assign_image(dlib_img, cv_image<unsigned char>(grayscale_frame));
            dlib::rectangle face_dlib;
            // Convert OpenCV rectangle object to dlib rectange object
            face_dlib = openCVRectToDlib(face);
            // Predict facial landmark locations and store it in shapes object
            shapes = sp(dlib_img, face_dlib);
            // Display facial landmarks
            if ((SHOW_VIDEO_OUTPUT || SAVE_TO_FILE) && SHOW_FACIAL_LANDMARKS)
            {
                for (unsigned long i = 0, max = shapes.num_parts(); i < max; i++)
                {
                    cv::circle(scaled_frame,
                               Point(shapes.part(i).x(), shapes.part(i).y()),
                               1,
                               CV_RGB(0, 0, 255));
                }
            }
            // Draw contours around the eyes
            if ((SHOW_VIDEO_OUTPUT || SAVE_TO_FILE) && SHOW_EYE_CONTOURS)
            {
                draw_eye_contours(scaled_frame, shapes);
            }

            // get EAR
            float avg_EAR = get_EAR(shapes);
            // Calculate EMA
            accumulator = ALPHA * avg_EAR + (1.0 - ALPHA) * accumulator;
            // Show current EAR
            if (SHOW_VIDEO_OUTPUT && SHOW_EAR)
            {
                draw_EAR(scaled_frame, avg_EAR);
            }
            // Check if EMA below threshold
            if (accumulator < thresh)
            {
                // Start clock
                auto end_thresh = steady_clock::now();
                double elapsed_seconds = duration_cast<duration<double>>(end_thresh - start_thresh).count();
                // Check if above time thresgold
                if (elapsed_seconds >= TIME_THRESHOLD)
                {
                    // Show sleepiness indicator
                    if ((SHOW_VIDEO_OUTPUT || SAVE_TO_FILE))
                    {
                        cv::putText(scaled_frame,
                                    "SLEEPINESS DETECTED",
                                    cv::Point(scaled_frame.cols * 0.25, scaled_frame.rows * 0.2),
                                    cv::FONT_HERSHEY_PLAIN,
                                    1,
                                    CV_RGB(255, 0, 0),
                                    2);
                    }
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
                // Stop clock
                start_thresh = steady_clock::now();
                // Send signal to stop alarm only if it is on
                if (alarmON)
                {
                    stop_playing();
                }
            }
            // Print FPS each loop
            if (PRINT_FPS)
            {
                auto end_fps = steady_clock::now();
                // Time to process one frame
                double time_for_cycle = duration_cast<duration<double>>(end_fps - start_fps).count();
                // frames processed per second
                double real_fps = 1.0 / time_for_cycle;
                cout << "fps " << real_fps << "\n";
            }
        }
        // Save frame to file
        if (SAVE_TO_FILE)
        {
            outputVideo.write(scaled_frame);
        }
        // Display frame in a window
        if (SHOW_VIDEO_OUTPUT)
        {
            cv::imshow("Live", scaled_frame);
            // Stop on any keypress (required for imshow function)
            if (waitKey(1) >= 0)
                break;
        }
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
// Draws contours around the eyes
void draw_eye_contours(Mat frame, full_object_detection shapes)
{
    // Convert dblib points to OpenCV Points and store in vector
    std::vector<std::vector<Point>> eye_countours(2);
    for (int i = 36; i <= 41; i++)
    {
        eye_countours[0].push_back(cv::Point(shapes.part(i).x(), shapes.part(i).y()));
    }
    for (int i = 42; i <= 47; i++)
    {
        eye_countours[1].push_back(cv::Point(shapes.part(i).x(), shapes.part(i).y()));
    }

    // Set eye outline color
    Scalar color = Scalar(255, 255, 0);
    // Show eye detection
    drawContours(frame, eye_countours, -1, color, 1);
}
// Calculates the EAR values from the facial landmarks
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
// Calculates euler distance between two points
int euler_distance(dlib::point p1, dlib::point p2)
{
    return sqrt(pow(p1.x() - p2.x(), 2) + pow(p1.y() - p2.y(), 2));
}
// Calculates the EAR threshold for sleepiness
float get_threshold(VideoCapture cap)
{
    float EAR_sum = 0.0f;
    int iterator = 0;
    Mat original_frame, scaled_frame, grayscale_frame;
    while (true)
    {
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

        if (result == 1)
        {
            if ((SHOW_VIDEO_OUTPUT || SAVE_TO_FILE))
            {
                cv::putText(scaled_frame,
                            "NO FACE DETECTED",
                            cv::Point(scaled_frame.cols * 0.25, scaled_frame.rows * 0.6),
                            cv::FONT_HERSHEY_PLAIN,
                            1,
                            CV_RGB(255, 0, 0),
                            2);
            }
        }
        else if (result == 0)
        {
            full_object_detection shapes;
            dlib::array2d<unsigned char> dlib_img;
            assign_image(dlib_img, cv_image<unsigned char>(grayscale_frame));
            dlib::rectangle face_dlib;
            // Convert OpenCV rectangle object to dlib rectange object
            face_dlib = openCVRectToDlib(face);
            // Predict facial landmark locations and store it in shapes object
            shapes = sp(dlib_img, face_dlib);

            // get EAR
            float avg_EAR = get_EAR(shapes);
            // Check if EAR is higher than minimum (eyes are likely to be open)
            if (avg_EAR > MIN_EAR_THRESH)
            {
                EAR_sum += avg_EAR;
                iterator++;
            }
        }
        // Break when got enough frames
        if (iterator >= FRAMES_FOR_CALIBRATION)
        {
            break;
        }
        // Show video and/or save to file
        if ((SHOW_VIDEO_OUTPUT || SAVE_TO_FILE))
        {
            cv::putText(scaled_frame,
                        "CALIBRATION MODE",
                        cv::Point(scaled_frame.cols * 0.25, scaled_frame.rows * 0.4),
                        cv::FONT_HERSHEY_PLAIN,
                        1,
                        CV_RGB(255, 0, 0),
                        2);
            if (SHOW_VIDEO_OUTPUT)
            { // imshow function needs this to process its enent loop
                // Stops the application on any keypress
                imshow("Live", scaled_frame);
                if (waitKey(1) >= 0)
                    break;
            }
            if (SAVE_TO_FILE)
            {
                outputVideo.write(scaled_frame);
            }
        }
    }
    // Calculate the threshold
    float thresh = (EAR_sum / (float)FRAMES_FOR_CALIBRATION) - THRESHOLD_SENSITIVITY;
    return thresh;
}
// Converts OpenCV rectengle to dlib rectangle
static dlib::rectangle openCVRectToDlib(cv::Rect r)
{
    return dlib::rectangle((long)r.tl().x, (long)r.tl().y, (long)r.br().x - 1, (long)r.br().y - 1);
}