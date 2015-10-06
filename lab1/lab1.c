#include <stdio.h>
#include <stdlib.h>


void insertionSort(int a[], int size) 
{
    int i;
    int j;
    for(i = 0; i < size; i++)
    {
        for(j = i; j-1 >= 0 && a[j] < a[j-1]; j--)
        {
            int temp = a[j-1];
            a[j-1] = a[j];
            a[j] = temp;
        }
    }
}

// void printArray(int a[], int size)
// {
//     int i;
//     for(i = 0; i < size; i++)
//     {
//         printf("%d\n", a[i]);
//     }
// }

int main()
{
    int sizeOfArray = 10;
    int * array = malloc(sizeof(int) * sizeOfArray);
    int i;
    for(i = 0; i < sizeOfArray; i++)
    {
        array[i] = 10-i;
    }
    insertionSort(array, sizeOfArray);
 //   printArray(array, sizeOfArray);
    return 0;
}
