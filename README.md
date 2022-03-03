# Proyecto desarrollado en lenguaje C

Descripción: Se desarrolló el juego de Gato para jugar dos usarios; con
interfaz gráfica. 

Desarrolladores:
	Kristel Herrera Rodríguez 
	Alex Alberto Varela Quirós
	Oscar Daniel Sánchez López

#para instalación de bibliotecas 
El kit de herramientas GTK de Glade: Gtk – 3.0
correr comando desde terminal de Linux:
	
$sudo apt install glade


#Para uso del programa
#Primera forma
Compilar con el siguiente comando desde la terminal de Linux:
gcc -o Gato main.c -Wall `pkg-config --cflags --libs gtk+-3.0` -export-dynamic

Ejecutar con: ./Gato

#Segunda forma
Se adjunta archivo Makefile
Para ejecutarlo correr los siguientes comandos en Linux: 
make
make run

# Para depurar el archivo ejecutable correr en terminal de Linux:
make clean



