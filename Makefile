# Compiler and linker
CC = gcc
CFLAGS = -Wall -Iinclude -I"C:/MinGW/include" -O2
LDFLAGS = -L"C:/MinGW/lib" -lopengl32 -lglu32 -lglut32 -lgdi32

# Folder paths
SRC_DIR = source
OBJ_DIR = obj
BIN_DIR = bin

# Executable name
TARGET = $(BIN_DIR)/Isometric.exe

# Source files and object files
SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC))

# Check if OS is Windows (ComSpec is a Windows-specific variable)
ifdef ComSpec
  MKDIR = if not exist $(OBJ_DIR) mkdir $(OBJ_DIR) & if not exist $(BIN_DIR) mkdir $(BIN_DIR)
  RM = del /Q
else
  MKDIR = mkdir -p $(OBJ_DIR) $(BIN_DIR)
  RM = rm -rf
endif

# Default rule
all: $(TARGET)

# Link objects to create the executable
$(TARGET): $(OBJ)
	$(MKDIR)
	$(CC) -o $@ $^ $(LDFLAGS)

# Compile source files into object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(MKDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build files
clean:
	$(RM) $(OBJ_DIR) $(BIN_DIR)

.PHONY: all clean