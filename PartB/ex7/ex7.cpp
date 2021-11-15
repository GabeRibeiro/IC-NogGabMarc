#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>
#include <cmath>
#include <unordered_map>

using namespace std;
using namespace cv;

double getChannelEntropy(Mat image, int channel)
{   
    Vec3b intensity;
    unordered_map<int, double> ch;
    double entropy_ch=0, prob;
    int total = image.cols * image.rows;

    for(int i=0; i<image.cols; i++)
    {   for(int j=0; j<image.rows; j++)
        {   
            intensity = image.at<Vec3b>(j,i);
            ch[intensity.val[channel]]++;
            
        }
    }

    // debug
    /*for (auto const &pair: ch) {
        std::cout << "{" << pair.first << ": " << pair.second << "}\n";
    }*/ 

    for(auto it:ch) {
        prob = it.second / total;
        entropy_ch += prob * log(prob);

    }
    return -entropy_ch;
}

void getHist(Mat src){
    vector<Mat> bgr_planes;
    split( src, bgr_planes );
    int histSize = 256;
    float range[] = { 0, 256 }; 

    const float* histRange[] = { range };

    bool uniform = true, accumulate = false;

    Mat b_hist, g_hist, r_hist;

    calcHist( &bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, histRange, uniform, accumulate );
    calcHist( &bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, histRange, uniform, accumulate );
    calcHist( &bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, histRange, uniform, accumulate );

    int hist_w = 512, hist_h = 400;
    int bin_w = cvRound( (double) hist_w/histSize );

    Mat histImage( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );

    normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
    normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
    normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
    
    for( int i = 1; i < histSize; i++ )
    {
        line( histImage, Point( bin_w*(i-1), hist_h - cvRound(b_hist.at<float>(i-1)) ),
              Point( bin_w*(i), hist_h - cvRound(b_hist.at<float>(i)) ),
              Scalar( 255, 0, 0), 2, 8, 0  );
        line( histImage, Point( bin_w*(i-1), hist_h - cvRound(g_hist.at<float>(i-1)) ),
              Point( bin_w*(i), hist_h - cvRound(g_hist.at<float>(i)) ),
              Scalar( 0, 255, 0), 2, 8, 0  );
        line( histImage, Point( bin_w*(i-1), hist_h - cvRound(r_hist.at<float>(i-1)) ),
              Point( bin_w*(i), hist_h - cvRound(r_hist.at<float>(i)) ),
              Scalar( 0, 0, 255), 2, 8, 0  );
    }
    imwrite("Source_image.png", src );
    imwrite("Source_Histogram.png", histImage );
}

void getGSHist(string const& name, Mat const& image)
{
    // Set histogram bins count
    int bins = 256;
    int histSize[] = {bins};
    // Set ranges for histogram bins
    float lranges[] = {0, 256};
    const float* ranges[] = {lranges};
    // create matrix for histogram
    Mat hist;
    int channels[] = {0};

    // create matrix for histogram visualization
    int const hist_height = 256;
    Mat3b hist_image = Mat3b::zeros(hist_height, bins);

    calcHist(&image, 1, channels, Mat(), hist, 1, histSize, ranges, true, false);

    double max_val=0;
    minMaxLoc(hist, 0, &max_val);

    // visualize each bin
    for(int b = 0; b < bins; b++) {
        float const binVal = hist.at<float>(b);
        int   const height = cvRound(binVal*hist_height/max_val);
        line( hist_image, Point(b, hist_height-height), cv::Point(b, hist_height), Scalar::all(255));
    }

    imwrite(name, hist_image);
}


int main(int argc, char** argv){

    if(argc != 2){
        cout << "Aborted run. Usage: ./ex5 input_file" << endl;
        exit(1);
    }

    double entropy_r, entropy_g, entropy_b, entropy_gs, entropy_mean;

    Mat src = imread( samples::findFile(argv[1]), IMREAD_COLOR);
    Mat srcgrays = imread( samples::findFile(argv[1]), IMREAD_GRAYSCALE);
    if (src.empty() )
    {
        return EXIT_FAILURE;
    }

    getHist(src);
    getGSHist("GrayScale_Histogram.png",srcgrays);
    imwrite("Grayscale_image.png",srcgrays);

    entropy_b = getChannelEntropy(src,0);
    entropy_g = getChannelEntropy(src,1);
    entropy_r = getChannelEntropy(src,2);
    entropy_mean = (entropy_g + entropy_b + entropy_r) / 3; 

    cout << "Entropy R:" << entropy_r << endl;
    cout << "Entropy G:" << entropy_g << endl;
    cout << "Entropy B:" << entropy_b << endl;
    cout << "Entropy mean: " << entropy_mean << endl;

    return 0;
}