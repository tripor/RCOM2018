READER=reader.c
WRITE=writer.c
ALL=shared.c stateMachine.c data.c
all:
	gcc -Wall $(READER) $(ALL) -o read -std=c11
	gcc -Wall $(WRITE) $(ALL) -o write -std=c11
clean:
	rm read
	rm write
