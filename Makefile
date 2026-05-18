CC= gcc -Wall -o

%.o: %.c
	$(CC) $@ -c $< 

onthefly: main.o onthefly.o
	$(CC) $@ $^


clean:
	@rm -rf *.o 

