CFLAGS+= -ljson-c

networkgraph-c:
	$(CC) -shared -fPIC -o build/networkgraph-c.so src/graph.c $(CFLAGS)

clean:
	rm src/*.o
	rm build/*

install:
	cp build/networkgraph-c.so /usr/lib/

uninstall:
	rm -f /usr/lib/networkgraph-c.so
