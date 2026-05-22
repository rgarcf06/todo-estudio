estudio: main.o data.o ui.o
	gcc -o estudio main.o data.o ui.o -lncurses

main.o: main.c data.h ui.h
	gcc -c main.c

data.o: data.c data.h
	gcc -c data.c

ui.o: ui.c ui.h data.h
	gcc -c ui.c

clean:
	rm -f *.o estudio

install: estudio
	sudo cp estudio /usr/local/bin/estudio
	sudo chmod +x /usr/local/bin/estudio
