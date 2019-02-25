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
void printArray(int* randArray, unsigned int size);
void checkIfSorted(int* randArray, unsigned int size);
void *runner(void* param);
int partition(int* randArray, int low, int high);
void quickSort(int* randArray, int low, int high);

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
        randArray[i] = rand() / 100000;
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

void quickSort(int* randArray, int low, int high){
    int pIndex;
    if(low < high){
        pIndex = partition(randArray, low, high);
        quickSort(randArray, low, pIndex-1);
        quickSort(randArray, pIndex + 1, high);
    }
}

int partition(int* randArray, int low, int high){
    int pivotPoint = randArray[high];
    int i, j;
    int temp;
    i = low - 1;

    for(j = low; j <= high; j++){
        if(randArray[j] <= pivotPoint){
            i++;
            temp = randArray[i];
            randArray[i] = randArray[j];
            randArray[j] = temp;
        }
    }
    temp = randArray[i+1];
    randArray[i+1] = randArray[high];
    randArray[high] = temp;
    return (i + 1);
    
}

int main(int argc, char *argv[])
{
    unsigned int size = 0;
    
    
    printf("Enter the size of the array to generate: ");
    scanf("%d", &size);
    printf("\n");
    setArraySize(size);
    populateArray(randArray, size);

    //checkIfSorted(randArray, size);
    
    printArray(randArray, size);
    
    quickSort(randArray, 0, size - 1);
    
    free(randArray); //frees the dynamically allocated memory for randArray
    return 0;
}