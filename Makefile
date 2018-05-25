all:
	arm-linux-gcc -o shuying.out ./src/*.c -l pthread -L ./lib/ -l ts -l jpeg
clean:
	rm cqucc