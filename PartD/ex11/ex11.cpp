#include <cmath>
#include <iomanip>
#include <iostream>
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

using namespace std;
using namespace cv;

double get_e2(const Mat& i1, const Mat& i2);

int main(int argc, char ** argv){

    if(argc != 3 ){
        cout << "Aborted run. Usage: ./ex9 input_file output_file" << endl;
        exit (1);
    }

    cv::Mat img1=cv::imread( argv[1]), img2=cv::imread( argv[2] );
    if (img1.empty() || img2.empty())
    {
        printf(" Error opening image\n");
        return EXIT_FAILURE;
    }


    int i, j, k, r=img1.rows, c=img1.cols;
    double error, sum=0, max_error=0;
    for(i=0; i<r; i++){
        for(j=0; j<c; j++){
            for(k=0; k<img1.channels(); k++){
                sum += pow((int)img1.at<Vec3b>(i, j)[k] - (int)img2.at<Vec3b>(i, j)[k], 2);
                error = abs((int)img1.at<Vec3b>(i, j)[k] - (int)img2.at<Vec3b>(i, j)[k]);
                max_error = error > max_error? error : max_error;
            }
        }
    }
    cout << "\n\nMaximum per pixel absolute error: " << max_error << endl;
    
    double e2 = (double)sum/(r*c);
    double snr = 10*log10(pow(255,2)/e2);
    cout << "SNR: " << snr << "\n\n" << endl;

    return 0;
}