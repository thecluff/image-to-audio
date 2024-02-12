# Image to Audio (with post-processing)

Convert an image into audio, and then apply additional audio effect processing.

--- 

The main code I'm using to actually do the image to audio conversion is by Levi Borodenko, and the github repository can be found here: https://github.com/LeviBorodenko/spectrographic/tree/master

This requires installing the following python modules: numpy, simpleaudio, wavio, Pillow

Additionally, imagemagick needs to be installed.

 - python - m pip install numpy && python - m pip install simpleaudio && python - m pip install wavio && python - m pip install Pillow

 - brew install imagemagick (or arch -arm64 brew install imagemagick, if using arm64)

---

To spice things up further, extra audio processing can be applied to resulting audio, which I wrote in C++. The github repository can be fund here: https://github.com/thecluff/CluffAudio/tree/main/C%2B%2B

These additional processing effects include: 
- Normalization
- Gain
- Clip
- Reverse
- Rectify
- Fade in
- Fade out
- Stereo to mono
- Mono to stereo
- Pan modulation
- Amplitude modulation
- Pitch change
- Extortion
- Wave shaping
- Polarity inversion

I've included the actual executables in the root directory, but the .cpp files can be found in ./processingSrc

Example values that can be set for processing effects:

- Rectify: ./FinalProject2 output.wav outputRectified.wav 1 3 
- Fade in ./FinalProject2 output.wav outputFadeIn.wav 1 4 1.0
- Fade out ./FinalProject2 output.wav outputFadeOut.wav 1 5 1.0
- Mono to stereo: ./MonoToStereo output.wav outputStereo.wav

--- 

## Usage

To run everything, I wrote a shell script which takes care of everything for the user. The program takes about 45 seconds to finish running. Note that Imagemagick may throw a warning, but the program will still run.

In terminal, run **jpg-example.sh** or **png-example.sh**
