#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_THREAD_NUMBER 3

//declaring the randArray dynamically so that a user defined length can be determined
int* randArray = NULL;
int* finalArray = NULL;
int* threadArray[];

//function declarations
void setArraySize(unsigned int size, unsigned int t_num);
void mergeArray(int* Array1, int* Array2, unsigned int arrSize, int* Array3);
void splitArray(unsigned int size, unsigned int t_num);
void populateArray(int* randArray, unsigned int size);
void printArray(int** threadArray, unsigned int size, unsigned int t_num);
void checkIfSorted(int* finalArray, unsigned int size);
void *runner(void* param);
int partition(int* Array, int low, int high);
void quickSort(int* Array, int low, int high);
void swap(int arr[], int a, int b);
void* quickSortJob(void*);
void* mergeArrayJob(void* mArgs);

/* Sets randArray to be a user determined size */
void setArraySize(unsigned int size, unsigned int t_num){
    int i;
    int n = t_num;
    int index = t_num;
    unsigned int thread_size = (size / t_num);

    if(!randArray){
        randArray = malloc(size * sizeof(unsigned int));
    }

    if(!finalArray){
        finalArray = malloc(size * sizeof(unsigned int));
    }

    for(i = 0; i < t_num; i++){ //original threads
	threadArray[i] = malloc(sizeof(unsigned int)*(thread_size));
    }

    while(n/2 != 1){ //merging threads
	n = n/2;
	thread_size = (size / n);
	for(i = 0; i < n; i++){
	    threadArray[index] = malloc(sizeof(unsigned int)*(thread_size));
	    index++;
	}
    }
}

void mergeArray(int* Array1, int* Array2, unsigned int arrSize, int* Array3){
    int i = 0;
    int j = 0;
    int k = 0;
	
    while(i < arrSize && j < arrSize){
	if(Array1[i] < Array2[j]){
	    Array3[k++] = Array1[i++];
	}
	else{
	    Array3[k++] = Array2[j++];
	}
    }
    while(i < arrSize){
	Array3[k++] = Array1[i++];
    }
    while(j < arrSize){
	Array3[k++] = Array2[j++];
    }
    
}

void splitArray(unsigned int size, unsigned int t_num){
    
    int i;
    int j;
    int k = 0;     
    
    for(i = 0; i < t_num; i++){ //initialize original threads
	for(j = 0; j < (size/t_num); j++){
            threadArray[i][j] = randArray[k];
	    k++;    	
	}
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
void printArray(int** threadArray, unsigned int size, unsigned int t_num){
    int i;
    int j;

    for(i = 0; i < t_num; i++){
	for(j = 0; j < (size/t_num); j++){
	    printf("Array: %d\nArray Value: %d\n\n", i, threadArray[i][j]);    	
	}
    }
}

/* This function checks to see if the array is properly sorted */
void checkIfSorted(int* finalArray, unsigned int size){
    int isSorted;
    int i;
    for(i = 0; i < size-1; i++){
        if(finalArray[i] <= finalArray[i+1]){
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
struct sortingArgs{
	int* array;
	int low;
	int high;
};
struct mergingArgs{
	int* array1;
	int* array2;
	int arraySize;
	int* array3;
};
//the runner function
void* quickSortJob(void* pArgs){
	struct sortingArgs* args = pArgs;
	printf("The sort thread starts\n");
	quickSort(args->array, args->low, args->high);
	printf("The sort thread ends\n");
	pthread_exit(0);	
}
void* mergeArrayJob(void* mArgs){
	struct mergingArgs* args = mArgs;
	printf("The merge thread starts\n");
	mergeArray(args->array1, args->array2, args->arraySize, args->array3);
	printf("The merge thread ends\n");
	pthread_exit(0);	
}

void quickSort(int *Array, int low, int high){
    if(low < high){
        int pIndex = partition(Array, low, high);
        quickSort(Array, low, pIndex-1);
        quickSort(Array, pIndex + 1, high);
    }
}

int partition(int *Array, int low, int high){
    int pivotPoint = Array[high];
    int i, j;
    i = low - 1;
    for(j = low; j <= high-1; j++){
        if(Array[j] <= pivotPoint){
            i++;
            swap(Array, i, j);
        }
    }
    swap(Array, (i+1), high);
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
    unsigned int t_num = 2;//number of threads
    unsigned int slots = t_num;
    int m = (t_num/2);
    void* status;
    int i;
    int j = t_num;
    int k = 0;
    unsigned int arrSize;
    while(m != 1){ //number of arrays 
	slots += m;
	m = m / 2;
    }

    threadArray[slots];

    if(argc != 2){
     fprintf(stderr, "usage: a.out <integer value>\n");
     return -1;
     }
     if((size=atoi(argv[1])) < 0){

     fprintf(stderr,"%d must be >= 0\n",atoi(argv[1]));
     return -1;
     }
    
    
    setArraySize(size, t_num);
    populateArray(randArray, size);
    splitArray(size, t_num);
    
    pthread_t tidArray1[t_num]; //sorting
    pthread_t tidArray2[(slots/1)-1];
    
	//pthread_join always requires a void* argument
     	int pThreadPivot = partition(randArray, 0, size-1);

	for(i = 0; i < t_num; i++){ //create threads for sorting
	    struct sortingArgs pArgs = {.array=threadArray[i], .low=0, .high=(size/t_num)-1};
	    pthread_create(&tidArray1[i], NULL, quickSortJob, &pArgs);
	    pthread_join(tidArray1[i], &status);
	}
	//printArray(threadArray, size, t_num);
	int Q = t_num;
	int P = t_num/2;
	for(i = 0; i < slots-2; i++){ //create threads for merging
	    if(P == 0){
		Q = Q/2;		
		P = Q/2;
	    }
	    arrSize = size / Q;
	    struct mergingArgs mArgs = {.array1=threadArray[i], 
	    .array2=threadArray[++i], .arraySize=arrSize ,.array3=threadArray[j]};
	    pthread_create(&tidArray2[k], NULL, mergeArrayJob, &mArgs);
	    pthread_join(tidArray2[k], &status);
    
	    j++;
	    k++;
	    P--;
	}

	/* this merges the last two sub arrays into the final sorted array */
	mergeArray(threadArray[slots-2], threadArray[slots-1], size/2, finalArray);

	for(i = 0; i < size; i++){ //prints final array
	    printf("\n%d: %d",i+1, finalArray[i]);
	}
	checkIfSorted(finalArray, size);
    	//printArray(threadArray, size, t_num);
    	//checkIfSorted(randArray, size);
    //stops clock after threads have completed running
    //clock_t stop = clock();
    //printf("Elapsed time: %f seconds\n", (double)(stop - start) / CLOCKS_PER_SEC);
    free(randArray); //frees the dynamically allocated memory for randArray
    for(i = 0; i < t_num; i++){
    	free(threadArray[i]);
    }

    return 0;
}

