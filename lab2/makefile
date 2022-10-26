CC=nvcc
CFLAGS=-g
EXECUTABLE_NAME=wave

main: wave

wave:
	@$(CC) $(CFLAGS) -o $(EXECUTABLE_NAME) wave.cu

.PHONY: clean
clean:
	@rm -rf $(EXECUTABLE_NAME) *.raw salidas/*.raw