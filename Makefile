all: compile_server compile_client

compile_server: ./src/server.c
	gcc -lpthread -o ./dist/server ./src/server.c ./src/worker.c

compile_client: ./src/client.c
	gcc -o ./dist/client ./src/client.c
