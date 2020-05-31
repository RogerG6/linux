obj = smsh1.o execute.o splitline.o process.o controlflow.o

smsh1 : $(obj)
	cc -o smsh1 $(obj)

*.o : *.c
	cc $<

clean : 
	rm *.o smsh1
