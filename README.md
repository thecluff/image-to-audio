# Image to Audio (with post-processing)

Convert an image into audio, and then apply additional audio effect processing.

--- 

The main code I'm using to actually do the image to audio conversion is by Levi Borodenko, and the github repository can be found here: https://github.com/LeviBorodenko/spectrographic/tree/master

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

Example values that can be set for each processing effect:

- Rectify: ./FinalProject2 output.wav outputRectified.wav 1 3 
- Fade in ./FinalProject2 output.wav outputFadeIn.wav 1 4 1.0
- Fade out ./FinalProject2 output.wav outputFadeOut.wav 1 5 1.0
- Mono to stereo: ./MonoToStereo output.wav outputStereo.wav


To merge the two projects together, a shell script would allow users to specify an input image file, duration, min and max frequencies, resolution, contrast, as well as post-processing effect to be applied to the resulting audio. (Not yet written)

This entails a rather long command for the user to enter, so I have provided an example below.

---
Image to Audio + Rectify (Distortion):

input_image.jpg -> specrographic -> cluffAudioProcessing -> output.wav

To convert an image to audio and then rectify the signal (input.JPG -> output.WAV -> rectifyOutput.WAV), one would use this command:

python spectrographic.py --image source.png --min_freq 10000 --max_freq 20000 --duration 10 --save sound.wav && FinalProject2 sound.wav outputRectified.wav 1 3