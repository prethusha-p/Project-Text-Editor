# --- CONFIGURATION ---
CC      := gcc
CFLAGS  := -Wall -Wextra -g -fPIC
LDFLAGS := -lncurses

# Target Names
EXEC        := editor
STATIC_LIB  := libeditor.a
SHARED_LIB  := libundoredo.so

# --- RULES ---

all: $(EXEC)

# 1. Compile the Static Library (Editor Core)
# Compiles editor.c into a static archive (.a)
$(STATIC_LIB): editor.o
	ar rcs $@ $^

editor.o: editor.c editor.h
	$(CC) $(CFLAGS) -c editor.c -o $@

# 2. Compile the Shared Object Library (Undo/Redo Logic)
# Compiles undo_redo.c into a shared library (.so)
$(SHARED_LIB): undo_redo.o
	$(CC) -shared -o $@ $^

undo_redo.o: undo_redo.c undo_redo.h
	$(CC) $(CFLAGS) -c undo_redo.c -o $@

# 3. Build the final Executable
# We link main.o with libeditor.a and libundoredo.so
# -L. looks for libraries in the current directory
# -Wl,-rpath,. ensures the .so is found at runtime
$(EXEC): main.c $(STATIC_LIB) $(SHARED_LIB)
	$(CC) $(CFLAGS) main.c -o $(EXEC) -L. -leditor -lundoredo $(LDFLAGS) -Wl,-rpath,.

#run the executable
run: $(EXEC)
	./$(EXEC)

# Clean up build files
clean:
	rm -f *.o *.a *.so $(EXEC)

.PHONY: all clean