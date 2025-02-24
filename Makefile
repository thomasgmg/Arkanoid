# Compiler and flags
CC = g++
CFLAGS = -std=c++11 -Wall -Og -g -Iinclude/
LDFLAGS_LINUX = lib/libraylib.a -lGL -lm -lpthread -ldl -lrt -lX11
LDFLAGS_WINDOWS = lib/libraylib-win64.a -lopengl32 -lgdi32 -lwinmm
LDFLAGS_MACOS = lib/libraylib.a -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo

# Detect OS
OS := $(shell uname -s)
ifeq ($(OS),Linux)
    LDFLAGS = $(LDFLAGS_LINUX)
    EXT =
    ARCHIVE_CMD = tar -czf arkanoid-linux.tar.gz $(OUT) README.md resources
else ifeq ($(OS),Darwin)  # Darwin is the macOS kernel name
    LDFLAGS = $(LDFLAGS_MACOS)
    EXT =
    ARCHIVE_CMD = tar -czf arkanoid-macos.tar.gz $(OUT) README.md resources
else
    LDFLAGS = $(LDFLAGS_WINDOWS)
    EXT = .exe
    ARCHIVE_CMD = zip arkanoid-windows.zip $(OUT) README.md resources
endif

# Source and output
SRC = main.cpp score.cpp
OUT = arkanoid$(EXT)

# Build
all:
	$(CC) $(CFLAGS) $(SRC) -o $(OUT) $(LDFLAGS)

# Package with README and LICENSE
package: all
	$(ARCHIVE_CMD)

# Clean
clean:
	rm -f arkanoid arkanoid.exe arkanoid-linux.tar.gz arkanoid-windows.zip arkanoid-macos.tar.gz

