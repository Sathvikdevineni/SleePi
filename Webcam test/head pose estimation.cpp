#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, char **argv)
{
    // Read input image
    cv::Mat im = cv::imread("headPose.jpg");
    
    // 2D image points. If you change the image, you need to change vector
    std::vector<cv::Point2d> Image_Points;
   Image_Points.push_back( cv::Point2d(359, 391) );    // Nose tip
   Image_Points.push_back( cv::Point2d(399, 561) );    // Chin
   Image_Points.push_back( cv::Point2d(387, 458) );     // Left eye left corner
   Image_Points.push_back( cv::Point2d(453, 789) );    // Right eye right corner
   Image_Points.push_back( cv::Point2d(345, 465) );    // Left Mouth corner
   Image_Points.push_back( cv::Point2d(453, 469) );    // Right mouth corner
    
    // 3D model points.
    std::vector<cv::Point3d> Model_Points;
    Model_Points.push_back(cv::Point3d(0.0f, 0.0f, 0.0f));               // Nose tip
    Model_Points.push_back(cv::Point3d(0.0f, -330.0f, -65.0f));          // Chin
    Model_Points.push_back(cv::Point3d(-225.0f, 170.0f, -135.0f));       // Left eye left corner
    Model_Points.push_back(cv::Point3d(225.0f, 170.0f, -135.0f));        // Right eye right corner
    Model_Points.push_back(cv::Point3d(-150.0f, -150.0f, -125.0f));      // Left Mouth corner
    Model_Points.push_back(cv::Point3d(150.0f, -150.0f, -125.0f));       // Right mouth corner
    
    

}

