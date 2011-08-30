#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "allanVariance.h"
using namespace std;

int main(int argc, char *argv[]) {
    // input args are input filename, output filename, sampling frequency
    
    int sampleFreq = atoi(argv[3]);

    // run allan variance
    return allanVariance(argv[1],argv[2],sampleFreq);
}
