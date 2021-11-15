#include <cstdio>
#include <cstring>
#include <string.h>
#include <fstream>
#include <iostream>
#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{

    if(argc != 3){
        cout << "Aborted run. Usage: ./ex4 input_file output_file" << endl;
        exit (1);
    }
    Mat image1, image2;

    image1 = imread(argv[1]);

    if(image1.empty())
    {
        cout << "Error while opening image or image is empty" << endl;
        exit(1);    
    }
    
        
    image2 = Mat::zeros(image1.size(), image1.type());

    int i,j, rows, cols;

    rows = image1.rows;
    cols = image1.cols;

    for(i = 0; i < rows; i++){
        for(j = 0 ; j < cols; j++){
            image2.at<Vec3b>(i, j) = image1.at<Vec3b>(i, j);
        }
    }

    imwrite(argv[2], image2);
    return 0;
}