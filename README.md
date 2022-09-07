# VenomGraphics
A repository to hold all the graphics related functionality for VenomSuite

How to build:
- Install Ogre3D: https://github.com/OGRECave/ogre
-- Follow the build steps and make sure to do `make install` at the end to install the files to /usr/**
-- IMPORTANT! build ogre3d with cmake flag "Build EGL" set to false, you can find the EGL flag using curses-cmake. You can skip this step if your ogre builds fine and all samples are working, but if you get an "EGL Surface Error" just build again with EGL turned off...
- Clone this repo with --recurse-submodules, or if you forgot just clone pybind11 https://github.com/pybind/pybind11
-- pybind11 folder needs to exist in the root of this repo
- Make a build folder, do `cmake ..` from the build folder, and then `make`. Finally run `main.py` using python3

cheers~
