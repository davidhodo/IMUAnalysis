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
    T =  N/sampleFreq;  // duration of data in seconds
	double maxTau=T/2;	// maximum averaging window size
	tauLength=9*log10(maxTau)-1;
    //tauLength = (T/2);	// number of tau values to calculate for
	//tauLength=T/4;

    //display program parameters
    //printf("sum = %Lf\n",sumOfData);
    //printf("mean = %Lf\n",meanOfData);
    //printf("N = %d\n",N);
    //printf("sampleFreq = %d\n",sampleFreq);
    printf("T = %d\n",T);
    printf("tauLength = %d\n",tauLength);

    

    //create arrays used in Allan Variance computation
    int * tau = new int [tauLength]; // array of averaging window times
    int * L = new int [tauLength];	// 
    int * M = new int [T];	// 
    double * cluster = new double [T];	
    double * delta = new double [T];
    double * sigma_tau = new double [tauLength];
	double * sigma_tau_err = new double [tauLength];
	time_t lastTime=time(NULL);
    time_t curTime;

    cout << "Starting calculation." << endl;
    //Begin Allan Variance Calculations
    for(j=0;j<tauLength;j++)
    {

        progress = (j/(tauLength-1))*100;
        // increment tau on a log scale
		int multiplier=((j)%9)+1;
		//if (multiplier==0)
		//	multiplier=1;
		//int multiplier;
		//if (temp==0)
		//	multiplier=1;
		//else if (temp==1)
		//	multiplier=4;
		//else if (temp==2)
		//	multiplier=7;
		temp=pow(10.0,j/9)*multiplier;
		tau[j]=temp;
        //tau[j] = j+1;  // length of integration window
		//if (j==0)
		//	tau[j]=1;
		//else
		//	tau[j]=2*j;
        L[j] = sampleFreq*(tau[j]);
        M[j] =  T/(tau[j]);

        curTime = time(NULL);
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

		// sum up the square of the numbers in the bin
        for(k=0;k<(M[j]-1);k++)
        {	
			num += delta[k]*delta[k];
        }

		// calculate two-sample variance for this tau
        sigma_tau[j]=sqrt(num/(2*(M[j] - 1))); 
        num = 0;

		// calculate error in variance calculation
		sigma_tau_err[j]=sigma_tau[j]/sqrt((double)M[j]+1.0);

    }
    
    cout << endl << "Calculations complete.  Writing output to: " << endl;
    cout << outputFile << endl;
    //open write file
	// format: 1st column = tau, 2nd column = variance, 3rd column = error bounds on the variance
    ofstream fout(outputFile);
    for(j=0;j<tauLength;j++)
    {
        fout << tau[j] << " " << sigma_tau[j] << " " << sigma_tau_err[j] << endl;
        //fprintf(output,"%d %f\n",tau[j],sigma_tau[j]);
    }
    //close write file
    fout.close();
    
    cout << "Output file writen." << endl;




    return 0;
}
