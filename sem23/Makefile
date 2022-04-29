all: build clean

build:
	gcc task0.c -I/usr/local/opt/openssl@1.1/include/ -L/usr/local/Cellar//openssl@1.1/1.1.1g/lib/ -lssl -lcrypto -o test
	./test

clean:
	rm -rf test

