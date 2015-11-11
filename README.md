PoisonConvert
=============

A little tool to batch-convert movies based on your preferences.


I developed PoisonConvert a few years ago and released it in hope that it will be useful to others. If you like PoisonConvert and want to support the development, please consider making a donation. Your support is greatly appreciated!


<a href="https://www.paypal.com/cgi-bin/webscr?cmd=_donations&business=mediaware%2efactory%40gmail%2ecom&lc=US&item_name=PoisonConvert&currency_code=USD&bn=PP%2dDonationsBF%3abtn_donate_SM%2egif%3aNonHosted"><img src="https://www.paypalobjects.com/en_US/i/btn/btn_donate_LG.gif" alt="[paypal]" /></a>

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
Before you invoke "make install", make sure that the directories /usr/syno/bin/ and /usr/syno/etc/ are available.

## How to use

For further instructions please read the Wiki:
https://github.com/CutePoisonX/PoisonConvert/wiki

## Deinstallation
If you want to uninstall PoisonConvert on a Synology DiskStation, please execute the install script again and select uninstall.

