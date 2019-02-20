all: sortingApp

sortingApp: sortingApp.c
	gcc sortingApp.c -o sortingApp

clean:
	rm -rf sortingApp

run: 
	./sortingApp