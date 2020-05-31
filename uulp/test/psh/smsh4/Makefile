obj = smsh1.o execute.o splitline.o builtin.o process.o controlflow.o  varlib.o

smsh1 : $(obj)
	cc -o smsh1 $(obj)

*.o : *.c
	cc $<

clean : 
	rm *.o smsh1
