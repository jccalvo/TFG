all:    
	gcc -g -c platformDown.c
	gcc -g -c comparador.c
	gcc -g -c PID.c
	gcc -g -c comandos.c
	gcc -g -c platformUp.c
	gcc -g -c spi.c
	gcc -g -c actuador.c
	gcc -g -o actuador platformDown.o comparador.o platformUp.o PID.o comandos.o spi.o actuador.o -ljansson -lcurl -lrt -lpigpio

clean:
	rm -f temp_dif.txt
	rm -f numeros.txt
	rm -f datos_func.txt
	$(RM) programa *.o *~

