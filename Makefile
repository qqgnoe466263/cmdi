
CC  := gcc
out := cmdi
obj := cmdi.o console.o linenoise.o

all: $(obj)
	$(CC) -o $(out) $(obj)

%.o: %.c
	$(CC) -c $^ -o $@

clean:
	rm cmdi *.o 
