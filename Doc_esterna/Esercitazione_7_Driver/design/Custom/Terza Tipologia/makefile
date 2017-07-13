mygpio_uiodriver : main.o mygpio_uiointdriver.o gpio_custom.o
	cc -o mygpio_uiointdriver main.o mygpio_uiointdriver.o gpio_custom.o

main.o : main.c mygpio_uiointdriver.c gpio_custom.c mygpio_uiointdriver.h gpio_custom.h
	cc -c main.c -o main.o

mygpio_uiointdriver.o : mygpio_uiointdriver.c mygpio_uiointdriver.h gpio_custom.h
	cc -c mygpio_uiointdriver.c -o mygpio_uiointdriver.o

gpio_custom.o : gpio_custom.c gpio_custom.h
	cc -c gpio_custom.c -o gpio_custom.o

clean :
	rm -f mygpio_uiointdriver
	rm *.o
	echo "Clean all file!"
