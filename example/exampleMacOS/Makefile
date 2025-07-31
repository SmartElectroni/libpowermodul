CC := clang++
CFLAGS := -Wall -Wextra -std=c++17 -I. -I/Library/CANalyst-II/include
LDFLAGS := -L/Library/CANalyst-II/lib -lControlCAN -framework IOKit -framework CoreFoundation
TARGET := exampleCan
SRC := exampleCan.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(TARGET)

run:
	sudo ./$(TARGET)