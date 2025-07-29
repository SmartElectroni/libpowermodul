
CC := g++

CFLAGS := -Wall -Wextra -std=c++17 -I.

SRC := exampleCan.cpp
OBJ := $(SRC:.cpp=.o)

TARGET := exampleCan

LIB_PATH := ../lib  
LIB_NAME := libmodul.1.0.0.so
FULL_LIB := $(LIB_PATH)/$(LIB_NAME)

LDFLAGS := -L$(LIB_PATH) -lmodul -Wl,-rpath=$(LIB_PATH)
LDLIBS := -lpthread

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

run: $(TARGET)
	LD_LIBRARY_PATH=$(LIB_PATH) sudo ./$(TARGET)