#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_THREAD_NUMBER 3

//declaring the randArray dynamically so that a user defined length can be determined
int *randArray = NULL;
int *finalArray = NULL;
int *threadOneArr = NULL;
int *threadTwoArr = NULL;

//function declarations
void setArraySize(unsigned int size);
void splitArray(unsigned int size);
void populateArray(int *randArray, unsigned int size);
void printArray(int *arr, unsigned int size);
void mergeArray(int *arr, int *arr2, unsigned int size, int *arr3);
void checkIfSorted(int *arr, unsigned int size);
void *quickSortJob(void *);
void *mergeArrayJob(void *mArgs);
void quickSort(int *arr, int low, int high);
int partition(int *arr, int low, int high);
void swap(int arr[], int a, int b);

/* Sets the dynamically declared arrays to be a user determined size */
void setArraySize(unsigned int size)
{   
    int arrSize = (size/2);
    if (!randArray)
    {
        randArray = malloc(size * sizeof(unsigned int));
    }
    if (!finalArray)
    {
        finalArray = malloc(size * sizeof(unsigned int));
    }
    if (!threadOneArr)
    {
        threadOneArr = malloc(arrSize * sizeof(unsigned int));
    }
    if (!threadTwoArr)
    {
        threadTwoArr = malloc(arrSize * sizeof(unsigned int));
    }
}
/* splits the array even in half */
void splitArray(unsigned int size)
{
    int i;
    int j;
    for(i = 0; i < (size/2); i++){
        threadOneArr[i] = randArray[i];
    }
    for(j = size/2; j < size; j++){
        threadTwoArr[j - ((size/2))] = randArray[j];
    }
    
}

/*this function will populate randArray with randomly generated numbers using the current time as a seed*/
void populateArray(int *randArray, unsigned int size)
{
    int i;
    //current time used as the seed for the random number generator
    srand(time(0));
    for (i = 0; i < size; i++)
    {
        randArray[i] = rand() / 10000;
    }
}

/*this function is used to test if the array has been sorted properly*/
void printArray(int *randArray, unsigned int size)
{
    int i;
    for (i = 0; i < size; i++)
    {
        printf("Array index: %d\nArray Value: %d\n\n", i, randArray[i]);
    }
}
/* this function will merge the two arrays of size (size/2) together into the finalArray of size (size) */
void mergeArray(int *array1, int *array2, unsigned int arrSize, int *array3)
{
    int i = 0;
    int j = 0;
    int k = 0;
    while (i < arrSize && j < arrSize)
    {
        if (array1[i] < array2[j])
        {
            array3[k] = array1[i];
            i++;
            k++;
        }
        else
        {
            array3[k] = array2[j];
            k++;
            j++;
        }
    }
    while (i < arrSize)
    {
        array3[k] = array1[i];
        k++;
        i++;
    }
    while (j < arrSize)
    {
        array3[k] = array2[j];
        k++;
        j++;
    }
}

/* This function checks to see if the array is properly sorted */
void checkIfSorted(int *randArray, unsigned int size)
{
    int isSorted;
    int i;
    for (i = 0; i < size - 1; i++)
    {
        if (randArray[i] <= randArray[i + 1])
        {
            isSorted = 1;
        }
        else
        {
            isSorted = 0;
            break;
        }
    }
    if (isSorted == 1)
    {
        printf("The array is sorted\n");
    }
    else
    {
        printf("Array is not sorted\n");
    }
}

/*a struct to hold args for quickSortArgs*/
struct dummyArgs
{
    int *array;
    int low;
    int high;
};
struct mergingArgs
{
    int *array1;
    int *array2;
    int arraySize;
    int *array3;
};
/* the runner function that is passed into pthread_create. for each thread instance, calls quickSort to recursively sort left and 
right sides of the array */
void *quickSortJob(void *pArgs)
{
    struct dummyArgs *args = pArgs;
    quickSort(args->array, args->low, args->high);
    pthread_exit(0);
}


/* The runner function that is passed into pthread_create for the merging thread. merges the two arrays sorted by sorting threads */
void *mergeArrayJob(void *mArgs)
{
    struct mergingArgs *args = mArgs;
    printf("The merge thread starts\n");
    mergeArray(args->array1, args->array2, args->arraySize, args->array3);
    printf("The merge thread ends\n");
    pthread_exit(0);
}



//calls recursively to sort the array in two halves separated at the pIndex
void quickSort(int *arr, int low, int high)
{
    if (low < high)
    {
        int pIndex = partition(arr, low, high);
        //sorts the "left half" of the array
        quickSort(arr, low, pIndex - 1);
        //sorts the "right half"
        quickSort(arr, pIndex + 1, high);
    }
}
//partitions the array and returns a new pivot point index to be used recursively in quickSort
int partition(int *arr, int low, int high)
{
    int pivotPoint = arr[high];
    int i, j;
    i = low - 1;
    for (j = low; j <= high - 1; j++)
    {
        if (arr[j] <= pivotPoint)
        {
            i++;
            swap(arr, i, j);
        }
    }
    swap(arr, (i + 1), high);
    return (i + 1);
}
//a helper function to swap array values
void swap(int arr[], int a, int b)
{
    int temp;
    temp = arr[b];
    arr[b] = arr[a];
    arr[a] = temp;
}

int main(int argc, char *argv[])
{
    //unsigned because it sets the size of an array
    unsigned int size = 0;
    //declaring the two sorting threads
    pthread_t tid1;
    pthread_t tid2;
    pthread_t tid3;
    //pthread_join always requires a void* argument
    void *status;
    int sideOneSize;
    int sideTwoSize;

    if (argc != 2)
    {
        fprintf(stderr, "usage: a.out <integer value>\n");
        return -1;
    }
    //gets the size as a command line argument
    if ((size = atoi(argv[1])) < 0)
    {
        fprintf(stderr, "%d must be >= 0\n", atoi(argv[1]));
        return -1;
    }

    setArraySize(size);
    //initializing the array
    populateArray(randArray, size);
    //starts the timer after the array initialization
    splitArray(size);
    clock_t start = clock();
    /* creates an instance of dummyArgs to be passed into quickSortJob when calling pthread_create */
    struct dummyArgs pArgs1 = {.array = threadOneArr, .low = 0, .high = (size / 2) - 1};
    struct dummyArgs pArgs2 = {.array = threadTwoArr, .low = 0, .high = (size / 2) - 1};
    //pthread_create doesn't require any attributes for the sake of this assignment, so NULL

    pthread_create(&tid1, NULL, quickSortJob, &pArgs1);
    pthread_create(&tid2, NULL, quickSortJob, &pArgs2);
    //status can be reused for both instances of pthread_join

    pthread_join(tid1, &status);
    pthread_join(tid2, &status);
    
    int arrSize = size / 2;
    struct mergingArgs mArgs = {.array1 = threadOneArr, .array2 = threadTwoArr, .arraySize = arrSize, .array3 = finalArray};
    pthread_create(&tid3, NULL, mergeArrayJob, &mArgs);
    pthread_join(tid3, &status);

    //stops the timer before printing the sorted array to the screen
    clock_t stop = clock();
    
    printf("final thread: \n");
    checkIfSorted(finalArray, size);
    //printArray(finalArray, size);
    
    printf("Elapsed time: %f seconds\n", (double)(stop - start) / CLOCKS_PER_SEC);

    //frees the dynamically allocated memory for randArray
    free(randArray);
    free(finalArray);
    free(threadOneArr);
    free(threadTwoArr);
    return 0;
}