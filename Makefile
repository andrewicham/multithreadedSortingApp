all: sortingApp

sortingApp: sortingApp.c
	gcc sortingApp.c -lpthread -o sortingApp

clean:
	rm -rf sortingApp

run: 
	./sortingApp
