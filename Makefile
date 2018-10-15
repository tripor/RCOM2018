READER=reader.c
WRITE=write.c
all:
	gcc -Wall $(READER) -o read 
	gcc -Wall $(WRITE) -o write 
clean:
	rm read
	rm write
