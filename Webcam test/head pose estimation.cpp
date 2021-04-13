#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, char **argv)
{
      // Read input image
    cv::Mat im = cv::imread("headPose.jpg");
    
    // 2D image points are described in it
    std::vector<cv::Point2d> image_points;
    image_points.push_back( cv::Point2d(359, 391) );    // Nose tip
    image_points.push_back( cv::Point2d(399, 561) );    // Chin
    image_points.push_back( cv::Point2d(337, 297) );     // Left eye left corner
    image_points.push_back( cv::Point2d(513, 301) );    // Right eye right corner
    image_points.push_back( cv::Point2d(345, 465) );    // Left Mouth corner
    image_points.push_back( cv::Point2d(453, 469) );    // Right mouth corner
    std::vector<cv::Point3d> model_points;
    model_points.push_back(cv::Point3d(0.0f, 0.0f, 0.0f)); // added the 3d point for nose tip

}

