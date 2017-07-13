uiodriver : main.o uiointdriver_function.o
	cc -o uiointdriver main.o uiointdriver_function.o

main.o : main.c uiointdriver_function.c uiointdriver_header.h
	cc -c main.c -o main.o

uiointdriver_function.o : uiointdriver_function.c uiointdriver_header.h
	cc -c uiointdriver_function.c -o uiointdriver_function.o

clean :
	rm -f uiointdriver
	rm *.o
	echo "Clean all file!"
