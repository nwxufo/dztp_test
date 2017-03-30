main: main.o serial_cfg.o response.o protocol.o
	gcc main.o serial_cfg.o response.o protocol.o -o main
main.o: main.c 
	gcc -c main.c -o main.o
serial_cfg.o: serial_cfg.c serial_cfg.h
	gcc -c serial_cfg.c -o serial_cfg.o

response.o: response.c response.h
	gcc -c response.c -o response.o
protocol.o: protocol.c protocol.h
	gcc -c protocol.c -o protocol.o

clean:
	@echo "cleanning compile object file: -rm main *.o"
	-rm main *.o
	-rm *~
	@echo "clean completed"
