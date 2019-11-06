all:	clean main.o
	@gcc -o main captcha.o main.o

main.o:	main.c captcha.c
	@cc -c captcha.c main.c -I.

run:
	@./main

clean:
	@find -name '*~' | xargs rm -rf {}
	@find -name '*o' | xargs rm -rf {}