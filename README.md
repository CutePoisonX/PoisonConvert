PoisonConvert
=============

A little tool to batch-convert movies based on your preferences.

## Installation on Synology DiskStations

There is a install script available for Synology DiskStations. 
You can download it here: http://www.mediaware-factory.com/#!leer/c1y0n (or through the github release).
Please follow the installation instructions at: http://www.mediaware-factory.com/#!poisonconvert-faqs/c1i99.

Make sure that ffmpeg, ffprobe and optionally qt-faststart are installed on your DiskStation before you execute the installer script. If you don't have ffmpeg etc. installed, you may want to try out my ffmpegInstaller script: https://github.com/CutePoisonX/ffmpegInstaller.

## Installation from Source

You can download and compile it on your system with:

1) git clone https://github.com/CutePoisonX/PoisonConvert.git

2) cd PoisonConvert

3) make
   
This will produce a binary "poisonconvert" in the current directory.
Before you invoke "make install", make sure that /opt/bin and /opt/etc are available.

## How to use

For further instructions please read the Wiki:
https://github.com/CutePoisonX/PoisonConvert/wiki

## Deinstallation
If you want to uninstall PoisonConvert on a Synology DiskStation, please execute the install script again and select uninstall.

