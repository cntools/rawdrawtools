all : convert

CFLAGS:=-I.. -I.

MINGW32:=/usr/bin/i686-w64-mingw32-

convert.exe : convert.c oldfont.h
	$(MINGW32)gcc -m32 -o $@ $^  -lgdi32 $(CFLAGS)
	./convert.exe

convert : convert.c oldfont.h
	gcc -o $@ $^ -lm -lpthread -g $(CFLAGS)
	chmod +777 convert
	./convert
	

clean :
	rm -rf *.o *~ text
	
