#include "fftw3.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>
#include <cstdlib>
//-------------------------------
using namespace std;
//-------------------------------
void writeData(const fftw_complex* const f, const int N, const double L,const char* const fname);

//-------------------------------

int main(int argc, char** argv){

	if(argc != 3){
		cout << "Usage: " << argv[0] << " input_file \t output_file" << endl;
		exit(1);
	}

	char *in_file  = argv[1];
	char *out_file = argv[2];

	const int N = 16384; // Anzahl Punkte aus Datei. Könnte man theoretisch auch auslesen 
	double L; // Länge des Intervalls in x 
	

	// Allocate memory
	fftw_complex* f = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * (N/2+1)); //Hier soll Fouriertrafo rein
	double* inR  = (double*) malloc(sizeof(double)*N); // hier sollen Daten aus Datei rein

  // Create PLAN
	fftw_plan FW  = fftw_plan_dft_r2c_1d(N, inR, f, FFTW_ESTIMATE); // Discrete Fourier Trafo Real 2 Complex 1Dim, estimate--> sucht sich selbst aus welche Mthode 

	// Read input data

	// Call function which reads the data from
	// the input file into the array inR

	ifstream in(in_file);
	for(int i=0;i<N;i++){
	
	  in >> L; // am Ende nur L gespeichert, aber nur wenn x0=0
	  in >> inR[i];
	}
	in.close();

  // Calculate FFT
  fftw_execute(FW); // nutz bPlan von oben

  // write output file
  writeData(f, N,  L, out_file);

  // Clean up
	fftw_destroy_plan(FW);
  fftw_free(f);
  free(inR);

	return 0;
}
//-------------------------------
void writeData(const fftw_complex* const f, const int N, const double L,const char* const fname){
	ofstream out(fname);
	const double dk = 2*M_PI/L;
	double pk;

	for(int i=0; i<=N/2; i++){
		pk = sqrt(f[i][0]*f[i][0] + f[i][1]*f[i][1])/N; //f[i][0] = realteil f[i][1] = imaginärteil 
		out << i*dk << "\t" << pk << "\t" << f[i][0] << "\t" << f[i][1] << endl;
	}

	out.close();
}
//-------------------------------
