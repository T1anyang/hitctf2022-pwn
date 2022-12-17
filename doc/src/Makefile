CC = gcc

objects = example, server

server: api.c server.c doc.c
	cc -o doc api.c server.c doc.c

example: doc.c example.c
	cc -g -o example doc.c example.c

clean:
	rm $(objects)