
CC  := gcc
out := cmdi
obj := cmdi.o console.o linenoise.o
CFLAGS = -g

all: $(obj)
	$(CC) $(CFLAGS) -o $(out) $(obj)

%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@

clean:
	rm cmdi *.o 
