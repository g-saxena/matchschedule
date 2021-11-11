master.out:main.c shared.h manager.out
	gcc main.c -o master.out -lpthread -lrt

manager.out:manager.c shared.h
	gcc manager.c -o manager.out -lpthread -lrt

clean:
	rm -f *.o *.out
