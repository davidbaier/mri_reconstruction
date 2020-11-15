#include <opencv4/opencv2/core.hpp>

class MRIFile{

    public:
        cv::Mat data;
        int y_pos {-1};
        std::string filename {};

        MRIFile(cv::Mat _data) : data(_data) {};
};