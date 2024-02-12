// This project provides a framework for building future projects

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <sndfile.h>
#include "processing.cpp"
#include "processing.hpp"

using namespace std;

// Global variables for file functions, etc.
SNDFILE *infile, *outfile;
SF_INFO sfInInfo, sfOutInfo;
sf_count_t readCount, writeCount;

// Buffers
double *inp, *outp, *mono;

// Forward declarations
int openInput(const char *fn);
int openOutput(const char *fn, int nChnls);
// int allocateBuffers(double *inBuf, double *outBuf, int length);
int readInput();
int copySamples(double *inBuf, double *outBuf, int length);
int writeOutput();
int cleanUp();

int main(int argc, const char * argv[]){

	openInput(argv[1]);
	openOutput(argv[2], atoi(argv[3]));
	// Allocate buffers
	inp = new double[sfInInfo.frames*sfInInfo.channels];
	outp = new double[sfInInfo.frames*sfInInfo.channels];
	mono = new double[sfInInfo.frames];

	readInput();
	copySamples(inp, outp, sfInInfo.frames*sfInInfo.channels);

	// Put processing functions here
	switch(atoi(argv[4])) {
	case 1:
		normalize(outp, sfInInfo.frames*sfInInfo.channels);
		gain(outp, sfInInfo.frames*sfInInfo.channels, atof(argv[5]));
		break;
	case 2:
		reverse(outp, outp, sfInInfo.frames*sfInInfo.channels);
		break;
	case 3:
		rectify(outp, sfInInfo.frames*sfInInfo.channels);
		break;
	case 4:
		fadeIn(outp, sfInInfo.samplerate, atoi(argv[3]), atof(argv[5]));
		break;
	case 5:
		fadeOut(outp, sfInInfo.frames*sfInInfo.channels, sfInInfo.samplerate, atoi(argv[3]), atof(argv[5]));
		break;
	case 6:
		stereoToMono(outp, sfInInfo.frames*sfInInfo.channels, mono);
		panMod(mono, sfInInfo.frames, outp, sfInInfo.samplerate, atof(argv[5]));
		break;
	case 7:
		ampMod(outp, sfInInfo.frames*sfInInfo.channels, sfInInfo.samplerate, atof(argv[5]));
	case 8:
		pitchChange(outp, sfInInfo.frames*sfInInfo.channels, atof(argv[5]), atof(argv[6]));
		break;
	case 9:
		extortion(outp, sfInInfo.frames*sfInInfo.channels, atof(argv[5]));
		break;
	case 10:
		waveShape(outp, sfInInfo.frames*sfInInfo.channels, atof(argv[5]), atof(argv[6]));
		break;
	}

	writeOutput();
	cleanUp();

    return 0;
}

int openInput(const char *fn) {
    // Opens input file
    if (!(infile = sf_open(fn, SFM_READ, &sfInInfo)))
	{
		// cout << "Not able to open input file; closing program." << endl;
		sf_perror(NULL);
		return 1;
	}
    // cout << "Input file opened for reading." << endl;

    return 0;
}

int openOutput(const char *fn, int nChnls) {
    	// Open the output file
	sfOutInfo.frames = sfInInfo.frames;
	sfOutInfo.channels = nChnls;
	sfOutInfo.samplerate = sfInInfo.samplerate;
	sfOutInfo.format = sfInInfo.format;

	if (!(outfile = sf_open(fn, SFM_WRITE, &sfOutInfo)))
	{
		// cout << "Not able to open the output file." << endl;
		// cout << "Application will now close." << endl;
		sf_perror(NULL);
		return 1;
	}
	// cout << "Output file  opened for writing" << endl;
    return 0;
}

int readInput() {
    readCount = sf_read_double(infile, inp, sfInInfo.frames*sfInInfo.channels);
	// cout << "The number of samples read is " << readCount << endl;
    return 0;
}

int copySamples(double *inBuf, double *outBuf, int length) {
	// Copy samples unaltered from one buffer to the other
	for(int ndx=0;ndx<length;ndx++){
		outBuf[ndx] = inBuf[ndx];
	}
	// cout << length << " samples copied from input buffer to output buffer." << endl;
	return 0;
}

int writeOutput(){
	writeCount = sf_write_double(outfile, outp, readCount*sfOutInfo.channels);
	// cout << writeCount << " samples written to output file." << endl;
	return 0;
}

int cleanUp() {
	sf_close(infile);
	sf_close(outfile);
	delete[] inp;
	delete[] outp;
	// cout << "Input and output files closed and buffers deleted." << endl;
	return 0;
}
