#gopro2gpx
Michiel van Wessem
Saturday, 20. July 2019 05:32PM 


##Introduction

gopro2gpx  was written to help display GPMD GPS data from GoPro videos, in particular for longer recordings where GoPro Quick does not suffice.


gopro2gpx extracts GPS data from Gopro MP4 videos and writes it to disk as a .gpx file. It extracts latitude, longitude, elevation and time. So far I've only tested it with GoPro Hero 7 black but it should work with other cameras as well.

During recording GoPro cameras  may create multiple MP4 files. If these files are located in the same directory gopro2gpx will try to find the related files and create a single gpx file for all these files combined.

##Installation

You can download a binary for Windows here and for Linux here. For other platforms, please download the source code and build the binary yourself. See the compilation section at the end of this page.

##Usage

This assumes you're able to use the command line interface or terminal on your system. On Windows that's the command prompt or powershell, on Linux/Unix it's a terminal.

From the command line simply run

>gopro2gpx GH010177.MP4

assuming GH010177.MP4 is the name of a GoPro video file. It will look for related MP4 files such as GH020177.MP4 GH030177.MP4 etc and process these in order to produce a single gpx output file. This file will be named after the first input file in the set.

If the filename of the MP4 file does not have that format, it will only process the single file and the output name will have the same base name as the mp4 file, eg gopro2gpx hello.mp4 will produce a file called hello.gpx.

It's also possible to explicitly tell gopro2gpx to only process a single file. For this use:
>gopro2gpx -s file.mp4

## Using the GPX files
Garmin has an terrific free tool called Virbedit that allows you to overlay gauges from .gpx or fit files onto video clips. It also allows you to combine those video clips.
Watch this video on how to use it: https://www.youtube.com/watch?v=s5v9ZCwcung 
 

##Compilation

This project uses CMake to produce cross platform builds. First run
>cmake projectdir

where projectdir is the name of the gopro2gpx directory (you can use . if that's already your current directory.

Then simply run 
>make 

This should build the gopro2gpx binary. Copy this to a suitable directory and you're in business.

On Windows, you can open the project directory in Visual Studio and build. It should see the CMake file and initialize things from there.

