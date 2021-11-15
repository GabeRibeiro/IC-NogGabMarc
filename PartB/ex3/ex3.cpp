#include	<cstdio>
#include	<cstring>
#include    <string.h>
#include    <fstream>
#include    <iostream>
#include	<sndfile.h>
#include    <sndfile.hh>

#define BUFFER_LEN 4096

using namespace std;

int main (int argc, char **argv){
    if(argc != 3){
        cout << "Aborted run. Usage: ./ex3 input_file output_file" << endl;
        exit(1);

    }
    SNDFILE *inwav, *outwav;
    SF_INFO sfi;

    memset(&sfi, 0, sizeof(sfi));

    if(!sf_open(argv[1], SFM_READ, &sfi)){
        printf("Error : error while opening file : %s\n", argv[1]);
        return 0;
    }
    inwav = sf_open(argv[1], SFM_READ, &sfi);

    if(!sf_open(argv[2], SFM_WRITE, &sfi)){
        printf("Error : error while opening file : %s\n", argv[2]);
        return 0;
    }
    outwav = sf_open(argv[2], SFM_WRITE, &sfi);

    if (!sf_format_check(&sfi)){
        sf_close(inwav);
        cout << "Invalid file" << endl;
        return 0;
    }

    

    float buf [BUFFER_LEN];
    int cnt;
    while((cnt = (int) sf_read_float(inwav, buf, BUFFER_LEN)) > 0)
    {
        sf_write_float(outwav,buf, cnt);
    }
    sf_close(inwav);
    sf_close(outwav);

    return 0;   
}

