nodriver : main.o nodriver_function.o
	cc -o nodriver main.o nodriver_function.o

main.o : main.c nodriver_function.c nodriver_header.h
	cc -c main.c -o main.o

nodriver_function.o : nodriver_function.c nodriver_header.h
	cc -c nodriver_function.c -o nodriver_function.o

clean :
	rm -f nodriver
	rm *.o
	echo "Clean all file!"
