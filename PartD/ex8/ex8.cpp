#include "AudioFile/AudioFile.h"
#include <math.h>


int reduceAmount = 12;

int main(int argc, char * argv[])
{

    //validate input
    if(argc != 2) {
        std::cout << "Compile using 'g++ ex8.cpp -o ex8 -Wall'" << std::endl;
        std::cout << "Usage: ./ex8 <audioFilePath>" << std::endl;
        return -1;
    }
    
    std::string filePath = argv[1];

    AudioFile<double> audioFile;
    audioFile.load(filePath);

    // print summary of original file
    // audioFile.printSummary();

    // original parameters
    int numChannels = audioFile.getNumChannels();
    int numSamples = audioFile.getNumSamplesPerChannel();
    int bitDepth = audioFile.getBitDepth();
    
    // get number of levels and define min and max values
    int nLevels = std::pow(2,bitDepth);
    int maxValue = 1;
    int minValue = -1;

    //delta: interval between each level
    double delta = (maxValue-minValue)/( (float) nLevels);

    //updated parameters
    int newBitDepth = bitDepth - reduceAmount;
    int newLevels = std::pow(2,newBitDepth);
    double newDelta = (maxValue-minValue)/( (float) newLevels);

    //calculate new amplitude level for each sample, according to new bit depth
    for(int x = 0; x < numChannels; x++)
    {  
        for(int y = 0; y < numSamples; y++)
        {
            // divide all samples by delta and divide by 2^reduceAmount
            // new sample Level is obtained from that
            int sampleLevel = ((int) (audioFile.samples[x][y]/delta)) >> reduceAmount; 
            // new sample value is obtained by multiplying the sample level by delta(new delta)
            double sampleValue = newDelta * sampleLevel;
            audioFile.samples[x][y] = sampleValue; //update audiofile sample values
        }
    }
    // save in transformed file
    audioFile.save("result.wav");
    // update bitdepth with new bit depth
    audioFile.setBitDepth(newBitDepth);

    // print summary of transformed file
    // audioFile.printSummary();
}