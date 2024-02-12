#   Create temp directory
mkdir ./temp/

#   Split source.jpg RGB channels into three separate images
magick source.jpg -channel-fx "red; green; blue" ./temp/output.jpg

#   Convert the images into three audio files
python3 spectrographic.py --image ./temp/output-0.jpg --min_freq 300 --max_freq 1000 --duration 5 --resolution 255 --save ./temp/sound1.wav 

python3 spectrographic.py --image ./temp/output-1.jpg --min_freq 375 --max_freq 1200 --duration 5 --resolution 319 --save ./temp/sound2.wav 

python3 spectrographic.py --image ./temp/output-2.jpg --min_freq 450 --max_freq 1500 --duration 5 --resolution 383 --save ./temp/sound3.wav

#   Mix the three audio files together into one file
./MixProject ./temp/sound1.wav ./temp/sound2.wav ./temp/sound1-2.wav 1 1.0 && ./MixProject ./temp/sound1-2.wav ./temp/sound3.wav ./soundfinal.wav 1 1.0

#   Add a fade-in to the audio file
./FinalProject2 soundfinal.wav ./temp/outputFadeIn.wav 1 4 1.0

#   Add a fade-out to the audio file
./FinalProject2 ./temp/outputFadeIn.wav ./temp/outputFadeOut.wav 1 5 1.0

#   Convert the audio file to stereo
./MonoToStereo ./temp/outputFadeOut.wav ./finalOutput.wav

#   House cleaning
# rm -r ./temp/
rm -r ./soundfinal.wav

#End program
while true; do
    read -p "Do you want to open the song now? (y or n) " yn
    case $yn in
        [Yy]* ) open finalOutput.wav; break;;
        [Nn]* ) exit;;
        * ) echo "Please answer yes or no.";;
    esac
done