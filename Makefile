EXEC=pseudoshell
FLAGS=-g -o

all: pseudoshell

pseudoshell: main.o string_parser.o command.o
	$(CC) $(FLAGS) $@ $^

main.o: main.c
	$(CC) -c $^    

# %< grabs left dependency

%.o: %.c %.h
	$(CC) -c $<    

# does string_parser and command at once (line above)

clean:
	rm -f *.o $(EXEC)

# More about makefiles on canvas