#include <iostream>
#include <sstream>
#include <opencv4/opencv2/core.hpp>
#include <opencv4/opencv2/imgproc/imgproc.hpp>
#include <opencv4/opencv2/highgui.hpp>

#include "../src/MRIFile/Storage.hpp"

using namespace std;
using namespace cv;


int main(int argc, char** argv) {
    if(argc < 2){
        cout << "Usage: ./MRI_reconstruction PATH" << endl;
        return -1;
    }

    string path = argv[1];

    MRIStorage<MRIFile> storage;
    storage.load_data(path);
    namedWindow("Image", WINDOW_AUTOSIZE);


    int key_pressed{};
    MRIStorage<MRIFile>::Iterator itr {storage.begin()};

    while(itr != storage.end()){
        key_pressed = waitKey();
        cout << itr->filename << "\n";
        imshow("Image", itr->data);
        if(key_pressed == 83){
            ++itr;
        }else if(key_pressed == 81){
            if(itr > storage.begin())
                --itr;
        }else if(key_pressed == 27){
            break;
        }
    }
    return 0;
}
