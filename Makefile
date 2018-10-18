READER=reader.c
WRITE=writer.c
all:
	gcc -Wall $(READER) -o read 
	gcc -Wall $(WRITE) -o write 
clean:
	rm read
	rm write
