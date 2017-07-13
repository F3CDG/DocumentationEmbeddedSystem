mygpio_uiodriver : main.o mygpio_uiodriver.o gpio_custom.o
	cc -o mygpio_uiodriver main.o mygpio_uiodriver.o gpio_custom.o

main.o : main.c mygpio_uiodriver.c gpio_custom.c mygpio_uiodriver.h gpio_custom.h
	cc -c main.c -o main.o

mygpio_uiodriver.o : mygpio_uiodriver.c mygpio_uiodriver.h gpio_custom.h
	cc -c mygpio_uiodriver.c -o mygpio_uiodriver.o

gpio_custom.o : gpio_custom.c gpio_custom.h
	cc -c gpio_custom.c -o gpio_custom.o

clean :
	rm -f mygpio_uiodriver
	rm *.o
	echo "Clean all file!"
