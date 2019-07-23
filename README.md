# gopro2gpx
Michiel van Wessem
Saturday, 20. July 2019 05:32PM 


## Introduction

gopro2gpx  was written to help display GPMD GPS data from GoPro videos, in particular for longer recordings where GoPro Quick does not suffice.

gopro2gpx extracts GPS data from Gopro MP4 videos using GoPro's [gpmf-parser library](https://github.com/gopro/gpmf-parser) and writes the data to disk as a .gpx file. It extracts latitude, longitude, elevation and time. So far I've only tested it with GoPro Hero 7 black but it should work with other cameras as well.

During recording GoPro cameras  may create multiple MP4 files. If these files are located in the same directory gopro2gpx will try to find the related files and create a single gpx file for all these files combined.

## Installation

Binaries:

* [Windows download binary](https://github.com/NetworkAndSoftware/gopro2gpx/blob/master/binaries/gopro2gpx-windows-0.0.1.zip?raw=true "Download Windows .exe file")
* [Linux download binary](https://github.com/NetworkAndSoftware/gopro2gpx/blob/master/binaries/gopro2gpx-linux-0.0.1.zip?raw=true "Download Linux binary")

For other platforms, please clone the repo and build the binary yourself... please let me know the results! See the compilation notes at the end of this page.

## Usage

This assumes you're able to use the command line interface or terminal on your system. On Windows that's the command prompt or powershell, on Linux/Unix it's a terminal.

From the command line simply run

>gopro2gpx GH010177.MP4

assuming GH010177.MP4 is the name of a GoPro video file. It will look for the other chapters in the same video - MP4 files such as GH020177.MP4 GH030177.MP4 etc and process these in order to produce a single gpx output file. This file will be named after the first input file in the set.

If the filename of the MP4 file does not have that format, it will only process the single file and the output name will have the same base name as the mp4 file, eg gopro2gpx hello.mp4 will produce a file called hello.gpx.

It's also possible to explicitly tell gopro2gpx to only process a single file. For this use:
>gopro2gpx -s file.mp4

Track point frequency in the gpx stream is now reduced to three samples per second. Contact me if this doesn't work for you.

## Using the GPX files
Garmin has an terrific free tool called Virbedit that allows you to overlay gauges from .gpx or fit files onto video clips. 
Watch this video on how to use it: https://www.youtube.com/watch?v=s5v9ZCwcung 
 
Virbedit does not allow the gpx file to span multiple video clips, so before you load the video into Virbedit it is necessary to combine the chapter files. You can do this with Ffmpeg's concat function. See https://trac.ffmpeg.org/wiki/Concatenate#samecodec and https://ffmpeg.org/ffmpeg-filters.html#concat 

Some massaging of the gpx files may be necessary. You can load them in a good text editor such as Vim and have at it.

## Compilation

This project uses CMake to produce cross platform builds. 

### Windows 
You can open the project folder with Visual Studio 2019 and simply pick the X64-Release configuration and build. This produces gopro2gpx.exe in a directory windows/build/X64-Release.

### Linux
Make sure you have gcc and cmake installed and there's a couple of other packages that you need.

Then run ./cmake-linux. This creates a subdirectory linux and runs cmake from there. After that there are makefiles and you can cd to the linux directory and run make. This should produce the gopro2gpx binary in that directory. Copy to a suitable directory in your path and you're in business.

## TO DO
- Improve build process, especially on Linux and for Visual Studio Code
- Add unit tests
