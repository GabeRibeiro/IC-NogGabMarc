#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

using namespace std;


int main(int argc, char **argv)
{
    if(argc != 3){
        cout << "Aborted run. Usage: ./ex5 input_file output_file" << endl;
        exit(1);

    }
    const int size = 26; // alphabet size
    int narrayc[size] = {}; // number of occurences
    int narrayp[size] = {}; //percentages
    char character;
    int total_letters = 0, sum_non = 0;;
    double percent;
    double prob, i, entropy = 0;

    string file, line;

    // open file
    ifstream ifs(argv[1]);
    ofstream ofs(argv[2]);
     
    if (!ifs.is_open())
    {
        cout << "Can't open file." << endl;
        return 1;
    }
    //loop until the file ends
    while (getline(ifs, line))
    {
        for (int j = 0; j < line.size(); ++j)
        {
            character = line[j];
            // find out which letter it is
            if (character >= 'A' && character <= 'Z')
            {
                narrayc[character-'A']++;
                ++total_letters;
            }
            else if (character >= 'a' && character <= 'z')
            {
                narrayc[character-'a']++;
                ++total_letters;
            }
            
            else
                // non-letter
                ++sum_non;
        }
    }
    ifs.close();
    if (total_letters != 0)
    {   
        ofs << "letter\tcount" << endl;
        // loop to get the percentages and print into new document
       
        for (int j = 0; j < size; ++j)
        {
            prob = (double) narrayc[j] / (double) total_letters; // prob of each char
            if(prob == 0){
            i = 0;}
            else{
            i = log(prob);}
            entropy += (prob*i);
            ofs << static_cast<char>('a'+ j) << "\t" << narrayc[j] << endl;
            // narrayp[j] = round(percent); array with prob(not used)
        }

    }
    else
    {
        cout << "File has no letters" << endl;
    }
    ofs.close();
    entropy = -1 * entropy;
    cout << "Entropy: " << entropy << endl;
    return 0;
}