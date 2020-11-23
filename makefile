CC=g++
CFLAGS=-g -Wall
FILES=$(shell find src -name "*.cpp")
EXEC=brainless

brain: $(FILES)
	$(CC) $(FILES) $(CFLAGS) -o $(EXEC)

run: $(EXEC)
	./$(EXEC)
