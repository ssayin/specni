# Project Details

![badge](https://github.com/ssayin/specni/actions/workflows/cmake.yml/badge.svg?branch=master)

An application which implements William Lilly Dignity/Debility point-scoring system. 
* [Understanding Planetary Dignity and Debility](http://www.skyscript.co.uk/dignities.html)
* [Lilly's Table to Examine the Strength & Debility of Each Planet](http://www.skyscript.co.uk/dig5.html#liltab)

## Built With

* [SwissEph](http://www.astro.com/swisseph/swephinfo_e.htm) - The high precision ephemeris developed by Astrodienst, largely based upon the DExxx ephemerides from NASA's JPL.
* [Dear ImGui](https://github.com/ocornut/imgui) - A bloat-free graphical user interface library for C++
* [SDL](https://github.com/libsdl-org/SDL) - A cross-platform development library designed to provide low level access to audio, keyboard, mouse, joystick, and graphics hardware via OpenGL and Direct3D.

## Build Requirements
* CMake >= 3.9
* GCC >= 12.2
* SDL2 binaries

## Cloning and Preparing

```sh
git clone --recurse-submodules https://github.com/ssayin/specni.git
cd specni
mkdir build
cd build
```
## Building

### w/ Ninja

```sh
cmake -GNinja ..
ninja
```

### w/ GNU Makefiles

```sh
cmake -G"Unix Makefiles" ..
make
```
## Ephemeris Files
***Important***\
The application uses Moshier Ephemeris by default if there are no ephemeris files present in the binary directory. Download the required ephemeris files from [this address](https://www.astro.com/ftp/swisseph/) and place them in the same directory as the binary executable if you want a more accurate ephemeris that includes asteroids and fixed stars.

***Example Binary Directory Structure***

.\
|-- bin\
|&emsp;|-- ephe\
|&emsp;|&emsp;|-- seasm114.se1\
|&emsp;|&emsp;|-- seasm24.se1\
|&emsp;|&emsp;|-- seasm60.se1\
|&emsp;|&emsp;\`-- sefstars.txt\
|&emsp;\`-- specni

*Note:* The required files are not represented by the file names listed above. Here will be updated with a list of the exact file names needed for the application to function properly.

## Running

```sh
cd bin
./specni
```
**Missing Font File**

Font file path is hardcoded as "../misc/fonts/zodiac_s.ttf". Try running the executable as follows:

```sh
./bin/specni
```
Moving the misc/fonts directory to the build directory is an alternative.

## Authors

* **Serdar Sayın** - *Initial work* - [GitHub](https://github.com/ssayin)
