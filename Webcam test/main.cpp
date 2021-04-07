
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

#include<time.h>

#include "play_audio.cpp"

using namespace cv;
using namespace std;
using namespace dlib;

// double const REDUCE_SIZE_BY = 3;
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

int main( int argc, char** argv )
{

    cout<<"tesdddddt\n";
    Mat frame;
    //can also be used for video input: VideoCapture cap("PATH_TO_VIDEO")
    // String face_cascade_name = samples::findFile("../static/haarcascade_frontalface_alt.xml");
    String face_cascade_name = "../static/haarcascade_frontalface_alt.xml";
    String shape_predictor_path = "../static/shape_predictor_68_face_landmarks.dat";
    
    deserialize(shape_predictor_path) >> sp;
    
     if( !face_cascade.load( face_cascade_name ) )
    {
        cout << "--(!)Error loading face cascade\n";
        return -1;
    };


    VideoCapture cap;
    // VideoCapture cap("../static/test_vid.mp4");
    // open the default camera, use something different from 0 otherwise;
    // Check VideoCapture documentation.
    int deviceID = 1;             // 0 = open default camera
    int apiID = cv::CAP_ANY;      // 0 = autodetect default API
    cap.open(deviceID, apiID);
    if (!cap.isOpened()) {
        cerr << "ERROR! Unable to open camera\n";
        return -1;
    }

    cout << "Start grabbing" << endl
        << "Press any key to terminate" << endl;

    cap.set(CAP_PROP_FRAME_WIDTH,1080);
    cap.set(CAP_PROP_FRAME_HEIGHT,1920);
    double fps = cap.get(CAP_PROP_FPS);
    double dWidth = cap.get(CAP_PROP_FRAME_WIDTH); //get the width of frames of the video
    double dHeight = cap.get(CAP_PROP_FRAME_HEIGHT); //get the height of frames of the video

    cout << "Resolution of the video : " << dWidth << " x " << dHeight <<" fps "<<fps<< endl;
    image_window win;
    Mat processed;
    clock_t start = clock();
    while(!win.is_closed())
    {
        // wait for a new frame from camera and store it into 'frame'
        cap.read(frame);
        // check if we succeeded
        if (frame.empty()) {
            cerr << "ERROR! blank frame grabbed\n";
            break;
        }

        // double scale_factor = 1/REDUCE_SIZE_BY;
        resize( frame, processed, Size(), SCALE_FACTOR, SCALE_FACTOR, INTER_LINEAR ); 
        // cvtColor( processed, processed, COLOR_BGR2GRAY );    
        // equalizeHist( processed, processed );
        // TODO: Fix convertingto grayscale, use opencv to display
        std::vector<Rect> faces;
        std::vector<full_object_detection> shapes;
        cv_image<bgr_pixel> cimg(processed);
        dlib::rectangle face_dlib;
        // detect face
        face_cascade.detectMultiScale( processed, faces );
        unsigned int face_count = faces.size();
        if(face_count == 0)
			{
				cout << "Face not detected"<<endl;
                win.clear_overlay();
				// return 0;
			}
        else if (face_count>1) {
            cout << "Expected only 1 face"<<endl;
            win.clear_overlay();
        }
        else if (face_count==1){

        face_dlib = openCVRectToDlib(faces[0]);
        shapes.push_back(sp(cimg,face_dlib));
        // }
        win.clear_overlay();

        float avg_EAR = get_EAR(shapes[0]);
        // Display EAR on screen
        std::stringstream stream;
        stream << std::fixed << std::setprecision(2) << avg_EAR;
        std::string s = stream.str();
        draw_string(cimg,point{cimg.nc()-cimg.nc()/3,10},"EAR: "+s, rgb_pixel{255, 0, 0});
// thresholding based on frame count - unreliable as processing speed might be lower than camera fps
        // if(avg_EAR < EAR_THRESH){
        //     COUNTER++;
        //     draw_string(cimg,point{10,50},to_string(COUNTER), rgb_pixel{255, 0, 0});
        //     if(COUNTER>FRAME_THRESH){
        //         draw_string(cimg,point{10,10},string{"ALERT! SLEEPINESS DETECTED"}, rgb_pixel{255, 0, 0});

        //     }
        // }
        // else{
        //     COUNTER = 0;
        // }

        if(avg_EAR < EAR_THRESH){
            clock_t finish = clock();
			double time_past = (double)(finish-start);
            // draw_string(cimg,point{20,90},to_string(time_past), rgb_pixel{255, 0, 0});

            if(time_past / 1000.0 >= TIME_THRESHOLD)
            {
                draw_string(cimg,point{10,10},string{"ALERT! SLEEPINESS DETECTED"}, rgb_pixel{255, 0, 0});
            }
        }

        else{
            start = clock();
            stop_playing();
        }


        // win.set_image(cimg);
        // show rectange of detected face;
        win.add_overlay(face_dlib);
        // show facial landmarks
        win.add_overlay(render_face_detections(shapes));
        }

    win.set_image(cimg);
        


    }

    // }

        // for ( size_t i = 0; i < faces.size(); i++ )
        // {
            // center of the face
            // Point center( faces[i].x/SCALE_FACTOR + faces[i].width/SCALE_FACTOR/2, faces[i].y/SCALE_FACTOR + faces[i].height/SCALE_FACTOR/2 );
            // // top right and bottom right rescaled coordinates for drawing a rectangle
            // Point tr_scaled_back(faces[i].x/SCALE_FACTOR, faces[i].y/SCALE_FACTOR);
            // Point bl_scaled_back(faces[i].x/SCALE_FACTOR + faces[i].width/SCALE_FACTOR, faces[i].y/SCALE_FACTOR + faces[i].height/SCALE_FACTOR);
            
            // rectangle(processed, faces[i], Scalar( 255, 0, 255 ));

            // cv::rectangle(input_frame, tr_scaled_back, bl_scaled_back, Scalar( 255, 0, 255 ),3);
            // draw an elipse around the face
            // ellipse( input_frame, center, Size( faces[i].width*REDUCE_SIZE_BY/2, faces[i].height*REDUCE_SIZE_BY/2 ), 0, 0, 360, Scalar( 255, 0, 255 ), 4 );
           

       


    return 0;
}

float get_EAR(full_object_detection landmarks){
    // Since the aspect ratio is calculated there is no need to get the actual euclidian distance, ratio of squares gives the same result
    int left_height1 = euler_distance(landmarks.part(37), landmarks.part(41));
    int left_height2 = euler_distance(landmarks.part(38), landmarks.part(40));
    int left_length = euler_distance(landmarks.part(36), landmarks.part(39));
    float left_EAR = (left_height1 + left_height2)/(2.0*left_length);

    int right_height1 = euler_distance(landmarks.part(43), landmarks.part(47));
    int right_height2 = euler_distance(landmarks.part(44), landmarks.part(46));
    int right_length = euler_distance(landmarks.part(42), landmarks.part(45));
    float right_EAR = (right_height1 + right_height2)/(2.0*right_length);
    
    float average_EAR = (left_EAR + right_EAR)/2.0;
    // cout<<"right EAR "<<right_EAR<<"\n"<<"left EAR "<<left_EAR<<"\n"<<"average EAR "<<average_EAR<<"\n";
    return average_EAR;    
}

int euler_distance(dlib::point p1, dlib::point p2){
    return sqrt(pow(p1.x()-p2.x(),2) + pow(p1.y()-p2.y(),2));
}

static dlib::rectangle openCVRectToDlib(cv::Rect r)
{
  return dlib::rectangle((long)r.tl().x, (long)r.tl().y, (long)r.br().x - 1, (long)r.br().y - 1);
}
