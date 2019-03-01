#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//declaring arrays dynamically so that a user defined length can be determined
int* randArray = NULL;
int* finalArray = NULL;
int** threadArray = NULL;

//function declarations
void setArraySize(unsigned int size, unsigned int t_num, unsigned int slots);
void mergeArray(int* Array1, int* Array2, unsigned int arrSize, int* Array3);
void splitArray(unsigned int size, unsigned int t_num);
void populateArray(int* randArray, unsigned int size);
void checkIfSorted(int* finalArray, unsigned int size);
int partition(int* Array, int low, int high);
void quickSort(int* Array, int low, int high);
void swap(int arr[], int a, int b);
void* quickSortJob(void*);
void* mergeArrayJob(void* mArgs);

/* Sets randArray to be a user determined size */
void setArraySize(unsigned int size, unsigned int t_num, unsigned int slots){
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
    threadArray = malloc(slots* sizeof(int *));

    for(i = 0; i < t_num; i++){ //original sorted threads
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
    
    /* initialize original threads to sort */
    for(i = 0; i < t_num; i++){
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
        printf("\nThe array is sorted\n");
    }
    else{
        printf("\nArray is not sorted\n");
    }
}

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

void* quickSortJob(void* pArgs){
	struct sortingArgs* args = pArgs;
	quickSort(args->array, args->low, args->high);	
}
void* mergeArrayJob(void* mArgs){
	struct mergingArgs* args = mArgs;
	mergeArray(args->array1, args->array2, args->arraySize, args->array3);	
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
    unsigned int size = 0;//size of array
    unsigned int t_num = 0;//number of threads
    printf("Enter size of array to sort: ");
    scanf("%d", &size);
    printf("\nEnter number of threads: ");
    scanf("%d", &t_num);

    unsigned int slots = t_num;
    int m = (t_num/2);
    void* status;
    int i;
    int j = t_num;
    int k = 0;
    unsigned int arrSize;

    while(m != 1){ //find the number of slots(arrays) needed
	slots += m;
	m = m / 2;
    }

    setArraySize(size, t_num, slots);
    populateArray(randArray, size);
    splitArray(size, t_num);//split original array into t_num # of arrays
    
    pthread_t tidArray1[t_num]; //sorting thread ID's
    pthread_t tidArray2[(slots/1)-1]; //merging thread ID's

    clock_t start = clock(); //start clock

	for(i = 0; i < t_num; i++){ //create threads for sorting
	    struct sortingArgs pArgs = {.array=threadArray[i], .low=0, .high=(size/t_num)-1};
	    pthread_create(&tidArray1[i], NULL, quickSortJob, &pArgs);
	    pthread_join(tidArray1[i], &status);
	}

	int merge_num = t_num;
	int counter = t_num/2;
	for(i = 0; i < slots-2; i++){ //create threads for merging
	    if(counter == 0){
		merge_num = merge_num/2;		
		counter = merge_num/2;
	    }
	    arrSize = size / merge_num;
	    struct mergingArgs mArgs = {.array1=threadArray[i], 
	    .array2=threadArray[++i], .arraySize=arrSize ,.array3=threadArray[j]};
	    pthread_create(&tidArray2[k], NULL, mergeArrayJob, &mArgs);
	    pthread_join(tidArray2[k], &status);
    
	    j++;
	    k++;
	    counter--;
	}

	/* this merges the last two sub arrays into the final sorted array */
	mergeArray(threadArray[slots-2], threadArray[slots-1], size/2, finalArray);

    //stops clock after threads have completed running
    clock_t stop = clock();

    for(i = 0; i < size; i++){ //prints final array
	    printf("\n%d: %d",i+1, finalArray[i]);
	}

    checkIfSorted(finalArray, size);

    printf("Elapsed time: %f seconds\n", (float)(clock() - start) / CLOCKS_PER_SEC);

    /* frees the dynamically allocated memory */
    free(randArray);
    free(finalArray);
    for(i = 0; i < slots; i++){
    	free(threadArray[i]);
    }
    free(threadArray);
    return 0;
}

