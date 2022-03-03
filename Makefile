all:
	gcc -o Gato main.c -Wall `pkg-config --cflags --libs gtk+-3.0` -export-dynamic


run:
	./Gato

clean:
	rm Gato
