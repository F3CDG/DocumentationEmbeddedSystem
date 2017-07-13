mygpio_nodriver : main.o mygpio_nodriver.o gpio_custom.o
	cc -o mygpio_nodriver main.o mygpio_nodriver.o gpio_custom.o

main.o : main.c mygpio_nodriver.c gpio_custom.c mygpio_nodriver.h gpio_custom.h
	cc -c main.c -o main.o

mygpio_nodriver.o : mygpio_nodriver.c mygpio_nodriver.h gpio_custom.h
	cc -c mygpio_nodriver.c -o mygpio_nodriver.o

gpio_custom.o : gpio_custom.c gpio_custom.h
	cc -c gpio_custom.c -o gpio_custom.o

clean :
	rm -f mygpio_nodriver
	rm *.o
	echo "Clean all file!"
