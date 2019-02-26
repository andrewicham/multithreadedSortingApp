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


/* Sets randArray to be a user determined size */
void setArraySize(unsigned int size){
    if(!randArray){
        randArray = malloc(size);
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
    for(j = low; j <= high; j++){
        if(randArray[j] < pivotPoint){
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
    /*if(argc != 2){
     fprintf(stderr, "usage: a.out <integer value>\n");
     return -1;
     }
     if(atoi(argv[1]) < 0){
     fprintf(stderr,"%d must be >= 0\n",atoi(argv[1]));
     return -1;
     }*/
    
    printf("Enter the size of the array to generate: ");
    scanf("%d", &size);
    printf("\n");

    setArraySize(size);
    populateArray(randArray, size);
    
    checkIfSorted(randArray, size);
    printArray(randArray, size);

    quickSort(randArray, 0, size - 1);
    
    printArray(randArray, size);
    checkIfSorted(randArray, size);
    
    //starts the clock for tread creation
    //clock_t start = clock();
    
    /*
     pthread_t tid;
     pthread_attr_t attr;
     
     pthread_t tid2;
     pthread_attr_t attr2;
     
     pthread_t tid3;
     pthread_attr_t attr3;
     
     pthread_attr_init(&attr);
     pthread_create(&tid, &attr, );*/
    
    //stops clock after threads have completed running
    //clock_t stop = clock();
    //printf("Elapsed time: %f seconds\n", (double)(stop - start) / CLOCKS_PER_SEC);
    free(randArray); //frees the dynamically allocated memory for randArray
    return 0;
}
