# swiss ephemeris
SWISSEPHLIB = -lswe
SWISSEPHINC = /usr/include/swisseph

CFLAGS = -g -Wall

# includes and libs
INC = -I${SWISSEPHINC}
LIB = ${SWISSEPHLIB} -pthread -L lib -ldl -lm

CXX = clang++
