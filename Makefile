
CFLAG= -Wall -Wextra

cc = gcc

prodcomm: thread.o module.o
	$(cc) $(CFLAG) -o prodcomm thread.o module.o -lpthread

module.o: module.c 
	$(cc) $(CFLAG) -c module.c

thread.o: thread.c
	$(cc) $(CFLAG) -c thread.c 

clean:
	 rm *.o prodcomm

scan-build: clean 
	scan-build -o $(SCAN_BUILD_DIR) make
#
#View the one scan available using firefox
#
scan-view: scan-build 
	firefox -new-window $(SCAN_BUILD_DIR)/*/index.html
