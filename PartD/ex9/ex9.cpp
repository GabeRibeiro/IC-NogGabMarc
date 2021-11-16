#include <cmath>
#include <iomanip>
#include <iostream>

#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <string.h>

using namespace std;
using namespace cv;


int main( int argc, char ** argv )
{
    cv::Mat src, dst;
    if(argc != 3 ){
        cout << "Aborted run. Usage: ./ex9 input_file output_file" << endl;
        exit (1);
    }

    src = cv::imread( argv[1]);
    if (src.empty())
    {
        cout << "Error while opening image or image is empty" << endl;
        exit(1);    
    }
    dst = Mat::zeros(src.size(), src.type());

    int i, j, k, r=src.rows, c=src.cols;
    double sum=0;
    for(i=0; i<r; i++){
        for(j=0; j<c; j++){
            for(k=0; k<3; k++){
                dst.at<Vec3b>(i, j)[k] = (src.at<Vec3b>(i, j)[k] >> 5) << 5;
            }
        }
    }
    
    cv::imshow("original",src);
    cv::imshow("destino",dst);
    imwrite( argv[2], dst);
    cv::waitKey(0);

    return 0;
}
