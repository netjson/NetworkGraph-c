CFLAGS+= -ljson-c
LDFLAGS = -shared -Wl,-soname,$(TARGET).so
TARGET = networkgraph-c
$(TARGET):
	$(CC) -shared -fPIC -o build/$(TARGET).so src/graph.c src/lib/cJSON.c $(CFLAGS) $(LDFLAGS)

test: networkgraph-c
	$(CC) -o test/test1 test/stupidtest.c src/graph.c src/lib/cJSON.c $(CFLAGS)

clean:
	rm build/*

install:
	cp build/$(TARGET).so /usr/lib/
	cp src/graph.h /usr/include/

uninstall:
	rm -f /usr/lib/$(TARGET).so
