#include "AudioFile/AudioFile.h"
#include <map>
#include <math.h>
#include <fstream>

float roundoff(float value, unsigned char prec);

void writeResults(std::string filePath, std::vector<std::map<float,int>> samplesMap, std::vector<double> entropies);

u_char precision = 2;

int main(int argc, char* argv[]) {

    //validate input
    if(argc != 2) {
        std::cout << "Compile using 'g++ ex6.cpp -o ex6 -Wall'" << std::endl;
        std::cout << "Usage: ./ex6 <audioFilePath>" << std::endl;
        return -1;
    }
    
    std::string filePath = argv[1];
    std::string resultFile = "result.txt";

    AudioFile<double> audioFile;
    bool result = audioFile.load(filePath);
    if(!result)
    {
        std::cout << "ERROR: Couldn't read file!" << std::endl;
        return -1;
    }

    int nChannels = audioFile.getNumChannels();
    int nSamples = audioFile.getNumSamplesPerChannel();

    //array of maps contains sample count of each channels + average of the channels (samplesMap[numChannels])
    std::vector<std::map<float,int>> samplesMap;
    samplesMap.resize(nChannels+1);

    float allSamples; 
    //read sample count from all channels and calculate the average of the channels
    for(int i = 0; i < nSamples; i++)
    {  
        allSamples = 0;
        for(int j = 0; j < nChannels; j++)
        {
            float value = roundoff(audioFile.samples[j][i], precision);
            allSamples += value;
            samplesMap[j][value]++;
        }
        samplesMap[nChannels][roundoff(allSamples/nChannels, precision)]++;
    }


    //calculate probability of each amplitude for each channel and the average of the channels
    std::vector<std::map<float,float>> samplesProb;
    samplesProb.resize(nChannels+1);
    for(int i = 0; i < nChannels+1; i++) 
    {
        for(auto itr = samplesMap[i].begin(); itr != samplesMap[i].end(); itr++)
        {
            samplesProb[i][(*itr).first] = (*itr).second / (float) nSamples;
        }
    }

    //calculate entropy for each channel and the average of the channels
    std::vector<double> entropies;
    entropies.resize(nChannels+1);

    for(int i = 0; i < nChannels+1; i++) 
    {   
        for(auto itr = samplesProb[i].begin(); itr != samplesProb[i].end(); itr++) 
        {
            if( (*itr).second != 0) {
                entropies[i] += - (*itr).second * log2((*itr).second);
            }
        }
    }
    
    writeResults(resultFile, samplesMap, entropies);

    for(int i = 0; i < nChannels; i++)
    {
        std::cout << "Entropy of channel "<< i + 1 << ": " << entropies[i] << std::endl;
    }
        
    std::cout << "Entropy of Average(Mono): " << entropies[nChannels] << std::endl;

    audioFile.printSummary();

    return 0;
}

float roundoff(float value, unsigned char prec) {
    float pow_10 = pow(10.0f, (float)prec);
    return round(value * pow_10) / pow_10;
}


void writeResults(std::string filePath, std::vector<std::map<float,int>> samplesMap, std::vector<double> entropies)
{
    std::ofstream fileOut(filePath);
       if(!fileOut.is_open()) {
        std::cerr << "Failed to open the file '" << filePath << "'" << std::endl;
    }

    for(u_long i = 0; i < samplesMap.size()-1; i++)
    {
        fileOut << "\n------------Channel " << i+1 << "------------" << std::endl;

        for(auto itr = samplesMap[i].begin(); itr != samplesMap[i].end(); itr++)
        {
            fileOut << (*itr).first <<  ": " << (*itr).second << std::endl;
        }
        fileOut << "\nEntropy: " << entropies[i] << std::endl;
    }

    fileOut << "\n------------Average(Mono)------------" << std::endl;

    for(auto itr = samplesMap[samplesMap.size()-1].begin(); itr != samplesMap[samplesMap.size()-1].end(); itr++)
    {
        fileOut << (*itr).first <<  ": " << (*itr).second << std::endl;
    }
    
    fileOut << "\nEntropy: " << entropies[samplesMap.size()-1] << std::endl;
    
    fileOut.close();
}
