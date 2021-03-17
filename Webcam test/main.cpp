
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;
using namespace std;

double const REDUCE_SIZE_BY = 2;
CascadeClassifier face_cascade;

void detect(Mat input_frame);

int main( int argc, char** argv )
{

    cout<<"tesdddddt\n";
    Mat frame;
    //can also be used for video input: VideoCapture cap("PATH_TO_VIDEO")
    // String face_cascade_name = samples::findFile("../static/haarcascade_frontalface_alt.xml");
    String face_cascade_name = "../static/haarcascade_frontalface_alt.xml";

    
     if( !face_cascade.load( face_cascade_name ) )
    {
        cout << "--(!)Error loading face cascade\n";
        return -1;
    };


    VideoCapture cap;
    // VideoCapture cap("../static/test_vid.mp4");
    // open the default camera, use something different from 0 otherwise;
    // Check VideoCapture documentation.
    int deviceID = 0;             // 0 = open default camera
    int apiID = cv::CAP_ANY;      // 0 = autodetect default API
    cap.open(deviceID, apiID);
    if (!cap.isOpened()) {
        cerr << "ERROR! Unable to open camera\n";
        return -1;
    }

    cout << "Start grabbing" << endl
        << "Press any key to terminate" << endl;

    cap.set(CAP_PROP_FRAME_WIDTH,360);
    cap.set(CAP_PROP_FRAME_HEIGHT,640);
    double fps = cap.get(CAP_PROP_FPS);
    double dWidth = cap.get(CAP_PROP_FRAME_WIDTH); //get the width of frames of the video
    double dHeight = cap.get(CAP_PROP_FRAME_HEIGHT); //get the height of frames of the video

    cout << "Resolution of the video : " << dWidth << " x " << dHeight << endl;

    while(true)
    {
        // wait for a new frame from camera and store it into 'frame'
        cap.read(frame);
        // check if we succeeded
        if (frame.empty()) {
            cerr << "ERROR! blank frame grabbed\n";
            break;
        }
        detect(frame);

       
        // show live and wait for a key with timeout long enough to show images
        
        
        if (waitKey(5) >= 0)
            break;
    }
    return 0;
}

void detect(Mat input_frame){
    Mat processed;
    double scale_factor = 1/REDUCE_SIZE_BY;
    resize( input_frame, processed, Size(), scale_factor, scale_factor, INTER_LINEAR ); 
    cvtColor( processed, processed, COLOR_BGR2GRAY );    
    equalizeHist( processed, processed );

    std::vector<Rect> faces;
    // detect face
    face_cascade.detectMultiScale( processed, faces );

    for ( size_t i = 0; i < faces.size(); i++ )
     {
        // center of the face
        Point center( faces[i].x*REDUCE_SIZE_BY + faces[i].width*REDUCE_SIZE_BY/2, faces[i].y*REDUCE_SIZE_BY + faces[i].height*REDUCE_SIZE_BY/2 );
        // top right and bottom right rescaled coordinates for drawing a rectangle
        Point tr_scaled_back(faces[i].x*REDUCE_SIZE_BY, faces[i].y*REDUCE_SIZE_BY);
        Point bl_scaled_back(faces[i].x*REDUCE_SIZE_BY + faces[i].width*REDUCE_SIZE_BY, faces[i].y*REDUCE_SIZE_BY + faces[i].height*REDUCE_SIZE_BY);
        
        // rectangle(processed, faces[i], Scalar( 255, 0, 255 ));

        rectangle(input_frame, tr_scaled_back, bl_scaled_back, Scalar( 255, 0, 255 ),3);
        // draw an elipse around the face
        // ellipse( input_frame, center, Size( faces[i].width*REDUCE_SIZE_BY/2, faces[i].height*REDUCE_SIZE_BY/2 ), 0, 0, 360, Scalar( 255, 0, 255 ), 4 );
     }



    imshow("Original Capture", input_frame);
    // imshow("Processed Capture", processed);
}