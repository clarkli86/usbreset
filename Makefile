CC=g++
CFLAGS=-I.
DEPS =
OBJ = usbreset.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

usbreset: $(OBJ)
	gcc -o $@ $^ $(CFLAGS) -lusb-1.0 -lstdc++
