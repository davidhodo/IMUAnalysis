#include <string>
#include <math.h>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <time.h>
#include "allanVariance.h"
using namespace std;

int progress=0;

int allanVariance(char* inputFile,char* outputFile,int sampleFreq)
{
    
    cout << "Calculating Allan Variance:" << endl;
    cout << "Input file: " << inputFile << endl;
    cout << "Output file: " << outputFile << endl;
    cout << "Sample Frequnecy: " << sampleFreq << endl << endl;
    //declare files
    ifstream fin(inputFile);

    //intialize variables
    double sumOfData=0;
    double meanOfData=0;
    double segmentSum = 0;
    double temp;
    double num = 0;
    int k = 0;
    int N = 0;	
    int j,n,T,tauLength;

    //Read in file to size
    N = std::count(std::istreambuf_iterator<char>(fin), 
             std::istreambuf_iterator<char>(), '\n'); 
	fin.clear();
	fin.seekg(0, ios::beg);

    //create an array the size of the data read
    double * data = new double [N];

    cout << "Read in " << N << " values from input file." << endl << endl;
    
	cout << "Reading and averaging data..." << endl;
    //Read in, sum, and compute the mean of the data in the input file
    while (!fin.eof())
    {
		double temp;
        fin >> temp;
		data[k]=temp;
        sumOfData = sumOfData + data[k];
        k = k+1;
    }
    fin.close();
    meanOfData = sumOfData/N;
    
    cout << "Data mean = " << meanOfData << endl << endl;

    //compute time and length of tau vector
    T =  N/sampleFreq;
    tauLength = T/2;

    //display program parameters
    //printf("sum = %Lf\n",sumOfData);
    //printf("mean = %Lf\n",meanOfData);
    //printf("N = %d\n",N);
    //printf("sampleFreq = %d\n",sampleFreq);
    printf("T = %d\n",T);
    printf("tauLength = %d\n",tauLength);

    

    //create arrays used in Allan Variance computation
    int * tau = new int [tauLength];
    int * L = new int [tauLength];	
    int * M = new int [T];	
    double * cluster = new double [T];	
    double * delta = new double [T];
    double * sigma_tau = new double [tauLength];
    time_t lastTime=time(NULL);
    time_t curTime;

    cout << "Starting calculation." << endl;
    //Begin Allan Variance Calculations
    for(j=0;j<tauLength;j++)
    {

        progress = (j/(tauLength-1))*100;
        tau[j] = j+1;
        L[j] = sampleFreq*(tau[j]);
        M[j] =  T/(tau[j]);

        curTime = time(NULL);
        //cout << curTime << "," << lastTime << endl;
        if ((curTime-lastTime)>5) {
            cout << "\rtau = " << tau[j] << " of " << tauLength;
            cout.flush();
            lastTime=curTime;
        }

        for(k=0;k<M[j];k++)
        {
            for(n=0;n<L[j];n++)
            {
                segmentSum += data[(n)+L[j]*(k)] ;
            }

            //create cluster average
            cluster[k] = segmentSum/((double)L[j]);
            segmentSum = 0;
            if(k != 0)
            {
                delta[k-1]=(cluster[k]-cluster[k-1]);
            }
        }

        for(k=0;k<(M[j]-1);k++)
        {	
         num += delta[k]*delta[k];
        }

        sigma_tau[j]=sqrt(num/(2*(M[j] - 1))); 
        num = 0;


    }
    
    cout << endl << "Calculations complete.  Writing output to: " << endl;
    cout << outputFile << endl;
    //open write file
    ofstream fout(outputFile);
    for(j=0;j<tauLength;j++)
    {
        fout << tau[j] << " " << sigma_tau[j] << endl;
        //fprintf(output,"%d %f\n",tau[j],sigma_tau[j]);
    }
    //close write file
    fout.close();
    
    cout << "Output file writen." << endl;




    return 0;
}
