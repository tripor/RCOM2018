READER=reader.c
WRITE=writer.c
ALL=shared.c
all:
	gcc -Wall $(READER) $(ALL) -o read
	gcc -Wall $(WRITE) -o write
clean:
	rm read
	rm write
