#include "stdafx.h"
#include "autoCorrelation.h"
#include <string>
#include <math.h>
using namespace std;


int autoCorrelation(char* inputFile,char* outputFile,int sampleFreq)
{

 //declare files
 FILE *input, *output;

//intialize variables
 double sumOfData=0;
 double meanOfData=0;
 double temp;
 double num = 0;
 int k = 0;
 int N = 0;	
 int j;
 int T;

//Read in file to size
 input = fopen(inputFile, "r");
 while(fscanf(input,"%Lf",&temp) != EOF)
 {
   N = N+1;
 } 

//Create an array the size of the data read
 double * data = new double [N];
 fclose(input);

//Read in, sum, and compute the mean of the data in the input file
 input = fopen(inputFile, "r");
 while(fscanf(input,"%Lf",&data[k]) != EOF)
 {
	sumOfData = sumOfData + data[k];
	k = k+1;
 }
 fclose(input);
 meanOfData = sumOfData/N;

//compute time 
 T =  N/sampleFreq;
 

//create arrays used in Allan Variance computation
 int * tau = new int [N];
 double * Rxx = new double [N];

//Begin AutoCorrelation Calculations------------------------------------------------------
 for(j=0;j<N;j++)
 {
	 tau[j] = j;
	 Rxx[j] = ;
}
//End AutoCorrelation Calculations-------------------------------------------------------



//Write to Output file
 output = fopen(outputFile, "w");
 for(j=0;j<N;j++)
 {
	fprintf(output,"%d %Lf\n",tau[j],Rxx[j]);
 }
 fclose(output);

 return 0;

}
