CXX=g++
AR=ar

BIN_DIR=../bin

HFILE=$(wildcard *.h)
SRC=$(wildcard *.cpp)

OBJ=$(SRC:%.cpp=%.o)

CFLAGS=-g -O2 -fno-strict-aliasing -Wall -W -Wpointer-arith -Wcast-qual -pipe
INC=-I../include
LIB=

TARGET=libtxml.a

.PHONY:all
all:$(TARGET) $(OBJ)

#bulid middle obj
%.o:%.cpp $(HFILE)
	$(CXX) $(CFLAGS) $(INC) -o $@ -c $<

$(TARGET):$(OBJ)
	$(AR) -r $@ $^

.PHONY:clean
clean:
	rm -f $(TARGET) $(OBJ)
.PHONY:love
love:clean all
