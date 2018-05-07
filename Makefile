
CC = clang

demo: demo.c rasterizer.c
	$(CC) demo.c rasterizer.c -o demo

clean:
	rm demo
