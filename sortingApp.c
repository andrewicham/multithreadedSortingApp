#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_THREAD_NUMBER 3

//declaring the randArray dynamically so that a user defined length can be determined
int* randArray = NULL;

//function declarations
void setArraySize(unsigned int size);
void populateArray(int* randArray, unsigned int size);
void printArray(int randArray[], unsigned int size);
void checkIfSorted(int* randArray, unsigned int size);
void *runner(void* param);
int partition(int* randArray, int low, int high);
void quickSort(int* randArra, int low, int high);
void swap(int arr[], int a, int b);
void* quickSortJob(void*);

/* Sets randArray to be a user determined size */
void setArraySize(unsigned int size){
    if(!randArray){
        randArray = malloc(size * sizeof(unsigned int));
    }
}

/*this function will populate randArray with randomly generated numbers using the current time as a seed*/
void populateArray(int* randArray, unsigned int size){
    int i;
    //current time used as the seed for the random number generator
    srand(time(0));
    for(i = 0; i < size; i++){
        randArray[i] = rand() / 10000;
    }
}

/*this function is used to test if the array has been sorted properly*/
void printArray(int* randArray, unsigned int size){
    int i;
    for(i = 0; i < size; i++){
        printf("Array index: %d\nArray Value: %d\n\n", i, randArray[i]);
    }
}

/* This function checks to see if the array is properly sorted */
void checkIfSorted(int* randArray, unsigned int size){
    int isSorted;
    int i;
    for(i = 0; i < size-1; i++){
        if(randArray[i] <= randArray[i+1]){
            isSorted = 1;
        }
        else{
            isSorted = 0;
            break;
        }
    }
    if(isSorted == 1){
        printf("The array is sorted\n");
    }
    else{
        printf("Array is not sorted\n");
    }
}

void *runner(void *param){
    int i, upper = atoi(param);
    
    
    
    
    
    pthread_exit(0);
}
//a struct to hold args for quickSortArgs
struct dummyArgs{
	int* array;
	int low;
	int high;
};
//the runner function
void* quickSortJob(void* pArgs){
	struct dummyArgs* args = pArgs;
	printf("The pthread starts\n");
	quickSort(args->array, args->low, args->high);
	printf("The pthread ends\n");
	pthread_exit(0);	
}

void quickSort(int *randArray, int low, int high){
    if(low < high){
        int pIndex = partition(randArray, low, high);
        quickSort(randArray, low, pIndex-1);
        quickSort(randArray, pIndex + 1, high);
    }
}

int partition(int *randArray, int low, int high){
    int pivotPoint = randArray[high];
    int i, j;
    i = low - 1;
    for(j = low; j <= high-1; j++){
        if(randArray[j] <= pivotPoint){
            i++;
            swap(randArray, i, j);
        }
    }
    swap(randArray, (i+1), high);
    return (i+1);
}
void swap(int arr[], int a, int b) {
    int temp;
    temp = arr[b];
    arr[b] = arr[a];
    arr[a] = temp;
    
}

int main(int argc, char *argv[])
{
    unsigned int size = 0;
    if(argc != 2){
     fprintf(stderr, "usage: a.out <integer value>\n");
     return -1;
     }
     if((size=atoi(argv[1])) < 0){

     fprintf(stderr,"%d must be >= 0\n",atoi(argv[1]));
     return -1;
     }
    
    
    setArraySize(size);
    populateArray(randArray, size);
    
    checkIfSorted(randArray, size);
    printArray(randArray, size);
    
     pthread_t tid1;
     pthread_t tid2;
     
     
     
     
	//pthread_join always requires a void* argument
	void* status;
     	int pThreadPivot = partition(randArray, 0, size-1);
	struct dummyArgs pArgs1 = {.array=randArray, .low=0, .high=pThreadPivot-1};
	struct dummyArgs pArgs2 = {.array=randArray, .low=pThreadPivot+1, .high=size-1};
	//pthread_create doesn't require any attributes for the sake of this assignment, so NULL
     	pthread_create(&tid1, NULL, quickSortJob, &pArgs1);
	pthread_create(&tid2, NULL, quickSortJob, &pArgs2);
	//status can be reused for both instances of pthread_join
	pthread_join(tid1, &status);
	pthread_join(tid2, &status);
    	
	printArray(randArray, size);
    	checkIfSorted(randArray, size);
    //stops clock after threads have completed running
    //clock_t stop = clock();
    //printf("Elapsed time: %f seconds\n", (double)(stop - start) / CLOCKS_PER_SEC);
    free(randArray); //frees the dynamically allocated memory for randArray
    return 0;
}
