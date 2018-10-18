READER=reader.c
WRITE=writer.c
ALL=shared.c stateMachine.c data.c
all:
	gcc -Wall $(READER) $(ALL) -o read
	gcc -Wall $(WRITE) $(ALL) -o write
clean:
	rm read
	rm write
