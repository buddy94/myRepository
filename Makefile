# Compiler
GCC=gcc
DATE=$(shell date "+%Y%m%d%H%M%S")

all: clean server client
server: $(PWD)/src/server.c
	$(GCC) -lpthread -o $(PWD)/dist/server_$(DATE) $(PWD)/src/server.c $(PWD)/src/worker.c

client: $(PWD)/src/client.c
	$(GCC) -o $(PWD)/dist/client_$(DATE) $(PWD)/src/client.c

clean:
	rm -rf $(PWD)/dist
	mkdir $(PWD)/dist
	touch $(PWD)/dist/EMPTY
