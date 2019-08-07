#include <iostream>
#include <sstream>
#include <opencv4/opencv2/imgproc.hpp>
#include <opencv4/opencv2/highgui.hpp>


using namespace std;
using namespace cv;

Mat remove_specific_values(Mat& src, int threshold_value);

int main(int argc, char** argv) {
    int threshold_value_max = 0;
    int threshold_value_min = 0;

    if(argc < 4){
        std::cout << "Usage: ./MRI_reconstruction PATH THRESHOLD_VALUE_MIN THRESHOLD_VALUE_MAX" << std::endl;
        return -1;
    }

    std::string filename = argv[1];
    std::stringstream(argv[2]) >> threshold_value_min;
    std::stringstream(argv[3]) >> threshold_value_max;

    cv::Mat src, hsv;
    src = imread(filename, CV_16F);

    if(src.empty()){
        std::cout << "No Image Data" << std::endl;
        return -1;
    }

    cvtColor(src, hsv,COLOR_BGR2GRAY);
    vector<vector<Point>> contours;
    //cv::Mat ret_value = remove_specific_values(src, threshold_value_max);
    cv::Mat ret_value, canny;
    double max = 0;

    cv::max(src, max);
    //cv::threshold(hsv, ret_value, threshold_value_max, max, 4);
    //cv::threshold(ret_value, ret_value, threshold_value_min, max, 4);


    cv::Canny(hsv, ret_value, threshold_value_max, threshold_value_min);
    findContours(ret_value, contours, RETR_TREE, CHAIN_APPROX_SIMPLE);

    vector<vector<Point>> hull(contours.size());
    for(size_t i = 0; i < contours.size(); ++i){
        cv::convexHull(contours[i], hull[i]);
    }

    Mat drawing = Mat::zeros( ret_value.size(), CV_8UC3 );
    for( size_t i = 0; i< contours.size(); i++ )
    {
        Scalar color = Scalar(  128, 128, 128 );
        drawContours( drawing, hull, (int)i, color );
    }
    namedWindow("MRIFile", WINDOW_AUTOSIZE);
    namedWindow("MRI_HSV", WINDOW_AUTOSIZE);

    imshow("MRIFile", ret_value);
    waitKey();


    return 0;
}

Mat remove_specific_values(Mat& src, int threshold_value){
    cv::Mat new_image = cv::Mat::zeros(src.size(), src.type());

    for(int y = 0; y < src.rows; y++){
        for(int x = 0; x < src.cols; x++){
            for(int c = 0; c < src.channels(); c++){
                if(src.at<Vec3b>(x,y)[c] <= threshold_value){
                    new_image.at<Vec3b>(x,y)[c] = src.at<Vec3b>(x,y)[c];
                }
            }
        }
    }

    return new_image;

}

/*cv::Mat remove_specific_values(Mat& src, int value){
    int x, y;
    int* p;

    for(x = 0; x < src.rows; ++x){
        p = src.ptr<int>(x);
        for(y = 0; y < src.cols; ++y){
            std::cout << p[y] << " ";
        }
        std::cout << std::endl;
    }
}*/