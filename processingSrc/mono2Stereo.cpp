#include <iostream>
#include <cstdlib>
#include <sndfile.h>

using namespace std;

int main(int argc, const char *argv[])
{

	// Global variables for file functions, etc.
	SNDFILE *infile, *outfile;
	SF_INFO sfInInfo, sfOutInfo;
	sf_count_t readCount, writeCount;

	// Buffers
	double *inBuf, *outBuf;

	// Stop if the inputs are not complete
	if(argc<3) {
		cout << "Please pass an input file name, and an output audio file name." << endl;
		cout << "Application closing." << endl;
		return 0;
	}

    // Open input file
	if (!(infile = sf_open(argv[1], SFM_READ, &sfInInfo)))
	{
		cout << "Not able to open input file; closing program." << endl;
		sf_perror(NULL);
		return 1;
	}

    // Test to see if the input file is mono
	if (sfInInfo.channels != 1)
	{
		cout << "Not a mono file. closing program." << endl;
		return 0;
	}

	cout << "The number of input frames is " << sfInInfo.frames << "." << endl;
	cout << "The sample rate is " << sfInInfo.samplerate << "." << endl;
	cout << "The number of channels is" << sfInInfo.channels << "." << endl;

	// Set up the output file format
	sfOutInfo.frames = sfInInfo.frames;
	sfOutInfo.channels = 2;
	sfOutInfo.samplerate = sfInInfo.samplerate;
	sfOutInfo.format = sfInInfo.format;

	// Open the output file
	if (!(outfile = sf_open(argv[2], SFM_WRITE, &sfOutInfo)))
	{
		cout << "Not able to open the output file." << endl;
		cout << "Application will now close." << endl;
		sf_perror(NULL);
		return 1;
	}

	// Allocate input and output buffers
	cout << "The number of input samples is" << sfInInfo.frames*sfInInfo.channels << "." << endl;
	cout << "The number of output samples is" << sfOutInfo.frames*sfOutInfo.channels << "." << endl;
	inBuf = new double[sfInInfo.frames * sfInInfo.channels];
	outBuf = new double[sfInInfo.frames * 2];

	readCount = sf_read_double(infile, inBuf, sfInInfo.frames*sfInInfo.channels);
	cout << "The number of samples read is" << endl;

	for (int ndx = 0; ndx < readCount; ndx++)
	{
		outBuf[ndx * 2] = inBuf[ndx];
		outBuf[ndx * 2 + 1] = inBuf[ndx];
	}
	cout << "The buffers were copied." << endl;

	writeCount = sf_write_double(outfile, outBuf, readCount*sfOutInfo.channels);

	cout << "The number of samples read is" << readCount << "." << endl;
	cout << "The number of samples written is" << writeCount << "." << endl;

	sf_close(infile);
	sf_close(outfile);
	delete[] inBuf;
	delete[] outBuf;

	return 0;
}
