#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int extraMemoryAllocated;

void *Alloc(size_t sz)
{
	extraMemoryAllocated += sz;
	size_t* ret = malloc(sizeof(size_t) + sz);
	*ret = sz;
	printf("Extra memory allocated, size: %ld\n", sz);
	return &ret[1];
}

void DeAlloc(void* ptr)
{
	size_t* pSz = (size_t*)ptr - 1;
	extraMemoryAllocated -= *pSz;
	printf("Extra memory deallocated, size: %ld\n", *pSz);
	free((size_t*)ptr - 1);
}

size_t Size(void* ptr)
{
	return ((size_t*)ptr)[-1];
}

// implements heap sort
// extraMemoryAllocated counts bytes of memory allocated
void heapify(int arr[], int n, int i){
	int max = 0, l = 0, r = 0, temp = 0;

	max = i;
	l = 2 * i + 1;
	r = 2 * i + 2;

	if (l < n && arr[l] > arr[max]){
		max = l;
	}
	if (r < n && arr[r] > arr[max]){
		max = r;
	}

	if (max != i){
		temp = arr[max];
		arr[max] = arr[i];
		arr[i] = temp;
		heapify(arr, n, max);
	}
}

void heapSort(int arr[], int n)
{
	int temp = 0;

	for (int i = n / 2 - 1; i >= 0; i--){
		heapify(arr, n, i);
	}

	for (int i = n - 1; i >= 0; i--){
		temp = arr[0];
		arr[0] = arr[i];
		arr[i] = temp;
		heapify(arr, i, 0);
	}
}

// implement merge sort
// extraMemoryAllocated counts bytes of extra memory allocated
void merge(int pData[], int l, int m, int r){
	int i = 0, j = 0, ln = 0, rn = 0, k = l;
	int *left = NULL, *right = NULL;

	ln = m - l + 1;
	rn = r - m;

	left = (int *) Alloc(sizeof(int) * ln);
	right = (int *) Alloc(sizeof(int) * rn);

	for (int i = 0; i < ln; i++){
		left[i] = pData[l+i];
	}

	for (int i = 0; i < rn; i++){
		right[i] = pData[m+1+i];
	}

	while (i < ln && j < rn){
		if (left[i] <= right[j]){
			pData[k] = left[i];
			i++;
		}
		else{
			pData[k] = right[j];
			j++;
		}
		k++;
	}

	while (i < ln){
		pData[k] = left[i];
		i++;
		k++;
	}

	while (j < rn){
		pData[k] = right[j];
		j++;
		k++;
	}

	DeAlloc(left);
	DeAlloc(right);
}

void mergeSort(int pData[], int l, int r)
{
	int m = (l + r) / 2;

	if (l < r){
		mergeSort(pData, l, m);
		mergeSort(pData, m+1, r);
		merge(pData, l, m, r);
	}
}

// implement insertion sort
// extraMemoryAllocated counts bytes of memory allocated
void insertionSort(int* pData, int n)
{
	int item = 0;
	int j = 0;

	for (int i = 1; i < n; i++){
		item = pData[i];
		j = i - 1;

		while (j >= 0 && pData[j] > item){
			pData[j+1] = pData[j];
			j--;
		}

		pData[j+1] = item;
	}
}

// implement bubble sort
// extraMemoryAllocated counts bytes of extra memory allocated
void bubbleSort(int* pData, int n)
{
	int temp = 0;

	for (int i = 0; i < n; i++){
		for (int j = 0; j < n-1; j++){
			if (pData[j] > pData[j+1]){
				temp = pData[j];
				pData[j] = pData[j+1];
				pData[j+1] = temp;
			}
		}
	}
}

// implement selection sort
// extraMemoryAllocated counts bytes of extra memory allocated
void selectionSort(int* pData, int n)
{
	int min = 0;
	int temp = 0;

	for (int i = 0; i < n; i++){
		min = i;
		for (int j = i + 1; j < n; j++){
			if (pData[j] < pData[min]);
			min = j;
		}
		if (min != i){
			temp = pData[min];
			pData[min] = pData[i];
			pData[i] = temp;
		}
	}
}

// parses input file to an integer array
int parseData(char *inputFileName, int **ppData)
{
	FILE* inFile = fopen(inputFileName,"r");
	int dataSz = 0;
	*ppData = NULL;
	
	if (inFile)
	{
		fscanf(inFile,"%d\n",&dataSz);
		*ppData = (int *)Alloc(sizeof(int) * dataSz);
		// Implement parse data block
		for (int i = 0; i < dataSz; i++){
			fscanf(inFile, "%d", (*ppData) + i);
		}
	}
	
	return dataSz;
}

// prints first and last 100 items in the data array
void printArray(int pData[], int dataSz)
{
	if (dataSz < 100){
		for (int i = 0; i < dataSz; i++){
			printf("%d ", pData[i]);
		}
		printf("\n");
	}
	else{
		for (int i = 0; i < 100; i++){
			printf("%d ", pData[i]);
		}
		printf("\n\t");

		if (dataSz > 100){
			for (int i = dataSz - 100; i < dataSz; i++){
				printf("%d ", pData[i]);
			}
			printf("\n");
		}
	}
}

int main(void)
{
	clock_t start, end;
	int i;
    double cpu_time_used;
	char* fileNames[] = {"input1.txt", "input2.txt", "input3.txt"};
	
	for (i=0;i<3;++i)
	{
		int *pDataSrc, *pDataCopy;
		int dataSz = parseData(fileNames[i], &pDataSrc);
		
		if (dataSz <= 0)
			continue;
		
		pDataCopy = (int *)Alloc(sizeof(int)*dataSz);
	
		printf("---------------------------\n");
		printf("Dataset Size : %d\n",dataSz);
		printf("---------------------------\n");
		
		printf("Selection Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		selectionSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		printf("Insertion Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		insertionSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);

		printf("Bubble Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		bubbleSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		printf("Merge Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		mergeSort(pDataCopy, 0, dataSz - 1);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);

        printf("Heap Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		heapSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		DeAlloc(pDataCopy);
		DeAlloc(pDataSrc);
	}
	
}
