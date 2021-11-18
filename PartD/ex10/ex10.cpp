#include <cmath>
#include "AudioFile/AudioFile.h"
#include <iostream>

using namespace std;

double get_e2(const AudioFile<double>& a1, const AudioFile<double>& a2);

int main(int argc, char ** argv)
{
    if(argc != 3){
        cout << "Aborted run. Usage: ./ex9 input_file output_file" << endl;
        exit (1);
    }

    AudioFile<double> af1, af2;
    af1.load(argv[1]);
    af2.load(argv[2]);


    int i, j;
    double error, max_error=0, sum=0;
    int numSamples = af1.getNumSamplesPerChannel();
    int numChannels = af1.getNumChannels();

    for(i=0; i< numChannels; i++){
        for(j=0; j< numSamples; j++){
            sum += pow(af1.samples[i][j] - af2.samples[i][j],2);
            error = abs(af1.samples[i][j] - af2.samples[i][j]);
            max_error = error > max_error? error : max_error;
        }
    }
    cout << "\n\nMaximum per sample absolute error: " << max_error << endl;

    double e2 = (double)sum/(numSamples*numChannels);
    double snr = 10*log10(pow(pow(2,16),2)/e2);
    cout << "SNR: " << snr << "\n\n" << endl;

    return 0;
}