# swiss ephemeris
SWISSEPHLIB = -lswe
SWISSEPHINC = /usr/include/swisseph

CFLAGS = -g -Wall -fstack-protector-strong

# includes and libs
INC = -I${SWISSEPHINC} -I src/lib/imgui
LIB = ${SWISSEPHLIB} -pthread -ldl -lm -lglfw -lOpenGL

CXX = g++
