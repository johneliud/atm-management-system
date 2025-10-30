# ATM Management System Makefile
# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2
DEBUG_FLAGS = -g -DDEBUG
SRCDIR = src
DATADIR = data
TARGET = atm

# Source files and objects
SOURCES = $(SRCDIR)/main.c $(SRCDIR)/system.c $(SRCDIR)/auth.c
OBJECTS = $(SOURCES:.c=.o)
HEADER = $(SRCDIR)/header.h

# Default target
all: $(TARGET)

# Build the main executable
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^
	@echo "✓ Build complete: $(TARGET)"

# Compile source files
$(SRCDIR)/%.o: $(SRCDIR)/%.c $(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@

# Debug build
debug: CFLAGS += $(DEBUG_FLAGS)
debug: clean $(TARGET)
	@echo "✓ Debug build complete"

# Install (create data directory if needed)
install: $(TARGET)
	@mkdir -p $(DATADIR)
	@touch $(DATADIR)/users.txt $(DATADIR)/records.txt
	@echo "✓ Installation complete"

# Run the application
run: $(TARGET)
	./$(TARGET)

# Clean build artifacts
clean:
	rm -f $(OBJECTS) $(TARGET)
	@echo "✓ Clean complete"

# Clean everything including data files
distclean: clean
	rm -f $(DATADIR)/*.txt $(DATADIR)/*.backup
	@echo "✓ Distribution clean complete"

# Create backup of data files
backup:
	@mkdir -p backups
	@cp $(DATADIR)/*.txt backups/ 2>/dev/null || true
	@echo "✓ Data backup complete"

# Show help
help:
	@echo "ATM Management System - Available targets:"
	@echo "  all       - Build the application (default)"
	@echo "  debug     - Build with debug symbols"
	@echo "  install   - Install and setup data directories"
	@echo "  run       - Build and run the application"
	@echo "  clean     - Remove build artifacts"
	@echo "  distclean - Remove all generated files"
	@echo "  backup    - Backup data files"
	@echo "  help      - Show this help message"

# Declare phony targets
.PHONY: all debug install run clean distclean backup help
