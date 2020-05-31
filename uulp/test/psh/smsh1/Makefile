obj = smsh1.o execute.o splitline.o

smsh1 : $(obj)
	cc -o smsh1 $(obj)

*.o : *.c
	cc *.c

clean : 
	rm *.o smsh1
