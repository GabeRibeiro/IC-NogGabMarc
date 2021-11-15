#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char **argv){
    
   if(argc != 3){
        cout << "Aborted run. Usage: ./ex2 input_file output_file" << endl;
        exit(1);

    }

    ifstream ifs(argv[1]);
    ofstream ofs(argv[2]);
    
    char x;
    while(true){
        ifs.get(x);
        if(ifs.eof())break;
        ofs << x;
    }
    ofs.close();
    return 0;
}