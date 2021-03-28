#Introduction
This is a starting guide for running the rPPG-C++ code.
--Rohith Yenumula, 01/14/2021

#Installation: MacOS

Step 1: Install XCode

Xcode: https://medium.com/@LondonAppBrewery/how-to-download-and-setup-xcode-10-for-ios-development-b63bed1865c

Step 2: Install brew and pkg-config

Brew:  https://www.howtogeek.com/211541/homebrew-for-os-x-easily-installs-desktop-apps-and-terminal-utilities

pkg-config:
brew install pkg-config

Step 3: Install wxWidgets

wxWidgets:
brew install wxmac

Step 4: Install OpenCV

OpenCV:
https://medium.com/@jaskaranvirdi/setting-up-opencv-and-c-development-environment-in-xcode-b6027728003
note: We compile OpenCV by explicitly passing opencv4, not opencv, since pkg-config --cflags --libs opencv uses version 4.5.0_2 of lib, instead of the most updated version, which is currently 4.5.1_2, (located under /usr/local/Cellar/opencv/)

Step 5: Install SDL2

SDL2:
brew install SDL2
brew install SDL2_image
brew install SDL2_ttf

#Installation: Windows 10

Step 1: Install MinGW (g++):

Install MinGW at:
https://sourceforge.net/projects/mingw/

Open up the MinGW installation manager and select mingw32-base, mingw32-gcc=g++ and msys-base. Then, go to the ‘Installation’ tab and select ‘Apply Changes’.

Next, add C:\MinGW\bin to your System Variable ‘Path’:
Go into Control Panel > System and Security > System.
Click on ‘Advanced System Settings’ on the left
Click on ‘Environment Variables’ button
Find ‘Path’ under the System Variables section, click it, then click ‘Edit’
Click ‘New’, then copy ‘C:\MinGW\bin’ into the list of Paths
Close out all the Control Panel windows

Finally, close out of the Command Prompt window and reopen it. Type ‘gcc --version’ to ensure gcc is now installed on your system.

Step 2: Install pkg-config:

...










#Run the code
We use GCC to compile, link, and build the application. In the root directory of our project, type:

g++ $(pkg-config --cflags --libs opencv4) `wx-config --libs --cxxflags` -std=c++17 rppg_main.cpp -o rppg -lSDL2 -lSDL2_image -lSDL2_ttf

Finally, to run:
./rppg
