
CC = clang

demo: demo.c rasterizer.c
	$(CC) -lm demo.c rasterizer.c -o demo

clean:
	rm demo
