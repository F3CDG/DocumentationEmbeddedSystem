uiodriver : main.o uiodriver_function.o
	cc -o uiodriver main.o uiodriver_function.o

main.o : main.c uiodriver_function.c uiodriver_header.h
	cc -c main.c -o main.o

uiodriver_function.o : uiodriver_function.c uiodriver_header.h
	cc -c uiodriver_function.c -o uiodriver_function.o

clean :
	rm -f uiodriver
	rm *.o
	echo "Clean all file!"
