all: ioctlTestUS

ioctlTestUS: ioctlTestUS.o
	gcc ioctlTestUS.o -o ioctlTestUS

ioctlTestUS.o: ioctlTestUS.c
	gcc -c ioctlTestUS.c

clean:
	rm -rf *o ioctlTestUS
