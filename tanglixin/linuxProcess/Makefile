all:
	gcc -o processchd processChild.c
	gcc -o processmain processParrent.c
	cp -fr processchd processmain /usr/bin/.

.PHONY: clean
clean:
	rm -f *.o
	rm /usr/bin/processmain /usr/bin/processchd

