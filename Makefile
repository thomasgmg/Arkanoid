# Compiler and flags
CC = g++
CFLAGS = -Wall -Og -g -static-libgcc -static-libstdc++ -Iinclude/
LDFLAGS_LINUX = lib/libraylib.a -lGL -lm -lpthread -ldl -lrt -lX11
LDFLAGS_WINDOWS = lib/libraylib-win64.a -lopengl32 -lgdi32 -lwinmm

# Detect OS
OS := $(shell uname -s)
ifeq ($(OS),Linux)
    LDFLAGS = $(LDFLAGS_LINUX)
    EXT =
    ARCHIVE_CMD = tar -czf arkanoid-linux.tar.gz $(OUT) README.md resources
else
    LDFLAGS = $(LDFLAGS_WINDOWS)
    EXT = .exe
    ARCHIVE_CMD = zip arkanoid-windows.zip $(OUT) README.md resources
endif

# Source and output
SRC = main.cpp
OUT = arkanoid$(EXT)

# Build
all:
	$(CC) $(CFLAGS) $(SRC) -o $(OUT) $(LDFLAGS)

# Package with README and LICENSE
package: all
	$(ARCHIVE_CMD)

# Clean
clean:
	rm -f arkanoid arkanoid.exe arkanoid-linux.tar.gz arkanoid-windows.zip

