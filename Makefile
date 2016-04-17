# Compiler
GCC=gcc

all: clean server client
server: $(PWD)/src/server.c
	$(GCC) -lpthread -o $(PWD)/dist/server $(PWD)/src/server.c $(PWD)/src/worker.c

client: $(PWD)/src/client.c
	$(GCC) -o $(PWD)/dist/client $(PWD)/src/client.c

clean:
	rm -rf $(PWD)/dist
	mkdir $(PWD)/dist
	touch $(PWD)/dist/EMPTY
