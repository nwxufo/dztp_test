main: dict_res_req.o main.o serial_cfg.o  protocol.o response.o
	gcc main.o serial_cfg.o response.o protocol.o dict_res_req.o -o main
main.o: main.c 
	gcc -c main.c -o main.o
serial_cfg.o: serial_cfg.c serial_cfg.h
	gcc -c serial_cfg.c -o serial_cfg.o
response.o: response.c response.h
	gcc -c response.c  -o response.o
protocol.o: protocol.c protocol.h
	gcc -c protocol.c -o protocol.o
dict_res_req.o: dict_res_req.c dict_res_req.h
	gcc -c dict_res_req.c -o dict_res_req.o

clean:
	@echo "cleanning compile object file: -rm main *.o"
	-rm main *.o
	-rm *~
	@echo "clean completed"
