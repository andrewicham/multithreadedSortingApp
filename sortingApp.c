#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


//declaring the randArray dynamically so that a user defined length can be determined
int* randArray = NULL;

void populateArray(int* randArray, unsigned int size);
void printArray(int* randArray, unsigned int size);
void setArraySize(unsigned int size);
void checkIfSorted(int* randArray, unsigned int size);

int main(int argc, char *argv[])
{
    unsigned int size = 0;
    printf("Enter the size of the array: ");
    scanf("%d", &size);
    printf("\n");

    setArraySize(size);
    populateArray(randArray, size);

    printArray(randArray, size);
    checkIfSorted(randArray, size);

    free(randArray); //frees the dynamically allocated memory for randArray
    return 0;
}

/* Sets randArray to be a user determined size */
void setArraySize(unsigned int size){
    if(!randArray){
        randArray = malloc(size);
    }
}

/*this function will populate randArray with randomly generated numbers using the current time as a seed*/
void populateArray(int* randArray, unsigned int size){
    int i;
    //the current time used as a seed for the random number generator
    srand(time(0));
    for(i = 0; i < size; i++){
        randArray[i] = rand();
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
