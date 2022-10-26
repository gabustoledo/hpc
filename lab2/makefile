CC=gcc
CFLAGS=-Wall -g
EXECUTABLE_NAME=wave

main: wave

wave:
	@$(CC) $(CFLAGS) -o $(EXECUTABLE_NAME) wave.c

.PHONY: clean
clean:
	@rm -rf $(EXECUTABLE_NAME) *.raw salidas/*.raw