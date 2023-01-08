/* COP 3502C Programming Assignment 3
This program is written by: Darian Torres*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "leak_detector_c.h"

int x;
int y;
FILE *ifp, *ofp;

// Structure to store x and y coordinates
typedef struct coordinates
{
  int coordx;
  int coordy;
} coordinates;

//structure to store coords of given store locations,
// the stores we are searching for, the number of smoothie
//shops, how many points we are searching for, and the 
// threshold for the merge and insertion sort
typedef struct input
{
  coordinates *coord;
  coordinates *search;
  int numSmoothShops;
  int ptsToSearch;
  int threshold;
} input;


//reads the data of the input file and stores all of the information into
//a data structure
input readData()
{
  //declaring variables to store in structure for later use
  int myX, myY, numSmoothShops, ptsToSearch, threshold;

  // scanning in my location, number of smoothie shops, point to search for, and threshold for the merge sort of insertion sort.
  fscanf(ifp, "%d %d %d %d %d", &myX, &myY, &numSmoothShops, &ptsToSearch, &threshold);

  //allocating space for the points of all the smoothie shops
  coordinates *mycord = malloc(sizeof(coordinates) * numSmoothShops);
  x = myX;
  y = myY;

  // reads rest of coordinates from the input file and stores them into the array
  int cordx, cordy;

  //stores an array of coordinate points into the structure 
  for (int i = 0; i < numSmoothShops; i++)
  {
    fscanf(ifp, "%d %d", &cordx, &cordy);
    mycord[i].coordx = cordx;
    mycord[i].coordy = cordy;
  } // end of storing for loop

  // storing the points we want to search for
  coordinates *search = malloc(sizeof(coordinates) * ptsToSearch);
  for (int j = 0; j < ptsToSearch; j++)
  {
    fscanf(ifp, "%d %d", &cordx, &cordy);
    search[j].coordx = cordx;
    search[j].coordy = cordy;
  }

  //holds the 2 given structures and other variables for later use
  input data;
  data.coord = mycord;
  data.search = search;
  data.numSmoothShops = numSmoothShops;
  data.ptsToSearch = ptsToSearch;
  data.threshold = threshold;

  return data;
}

//compares 2 given points 
int compareTo(coordinates *ptrPt1, coordinates *ptrPt2)
{

  int distance1 = (pow((x - ptrPt1->coordx), 2) + pow((y - ptrPt1->coordy), 2));
  int distance2 = (pow((x - ptrPt2->coordx), 2) + pow((y - ptrPt2->coordy), 2));

  if (distance1 < distance2)
    return -1;

  else if (distance1 > distance2)
    return 1;

  else if (ptrPt1->coordx == ptrPt2->coordx && ptrPt1->coordy == ptrPt2->coordy)
    return 0;

  else
  {

    if (ptrPt1->coordx < ptrPt2->coordx)
      return -2;

    else if (ptrPt1->coordx > ptrPt2->coordx)
      return 2;

    else
    {

      if (ptrPt1->coordy < ptrPt2->coordy)
        return -3;

      else if (ptrPt1->coordy > ptrPt2->coordy)
        return 3;
    } // end of second else
  }   // end of first else
  return 0;
}

// Function to sort an array using insertion sort
void insertionSort(coordinates *arr, int l, int r)
{
  int i, j, length;
  length = r - l;
  coordinates item;
  for (i = l + 1; i < l + length + 1; i++)
  {
    item = arr[i];
    for (j = i - 1; j >= l; j--)
    {
      if (compareTo(&arr[j], &item) > 0)
        arr[j + 1] = arr[j];
      else
        break;
    }
    arr[j + 1] = item;
  }
}

// Merges two subarrays of arr[].
// First subarray is arr[l..m]
// Second subarray is arr[m+1..r]
void merge(coordinates *arr, int l, int m, int r)
{
  int i, j, k;
  int n1 = m - l + 1;
  int n2 = r - m;

  /* create temp arrays */
  coordinates *L = malloc(n1 * sizeof(coordinates));
  coordinates *R = malloc(n2 * sizeof(coordinates));

  /* Copy data to temp arrays L[] and R[] */
  for (i = 0; i < n1; i++)
    L[i] = arr[l + i];
  for (j = 0; j < n2; j++)
    R[j] = arr[m + 1 + j];
  /* Merge the temp arrays back into arr[l..r]*/
  i = 0; // Initial index of first subarray
  j = 0; // Initial index of second subarray
  k = l; // Initial index of merged subarray
  while (i < n1 && j < n2)
  {
    if (compareTo(&L[i], &R[j]) < 0)
    {
      arr[k] = L[i];
      i++;
    }
    else
    {
      arr[k] = R[j];
      j++;
    }
    k++;
  }

  while (i < n1)
  {
    arr[k] = L[i];
    i++;
    k++;
  }

  while (j < n2)
  {
    arr[k] = R[j];
    j++;
    k++;
  }
  free(L);
  free(R);
}


void mergeSort(coordinates *arr, int l, int r, int threshold)
{
  int length = r - l;
  if (length > threshold)
  {
    // get the mid point
    int m = (l + r) / 2;
    // Sort first and second halves
    mergeSort(arr, l, m, threshold);
    mergeSort(arr, m + 1, r, threshold);
    // printf("Testing l=%d r=%d m=%d\n", l, r, m);
    merge(arr, l, m, r);
  }
  else
    insertionSort(arr, l, r);
}

//This function will determine when the code will utilize mergesort
//or insertion sort
void sort(int threshold, int numSmoothShops, coordinates *coords)
{
  if (numSmoothShops > threshold)
    mergeSort(coords, 0, numSmoothShops - 1, threshold);

  else
    insertionSort(coords, 0, numSmoothShops);
}

//Searches the sorted points of smoothie shops to see if the point we are
//looking for is in it. This code runs recursively as well!
void binarySearch(coordinates *list, coordinates item, int l, int h)
{
  //terminating condition of recursion
  if (l > h)
  {
    printf("%d %d not found\n", item.coordx, item.coordy);
    fprintf(ofp, "%d %d not found\n", item.coordx, item.coordy);
    return;
  }

  //initialize mid so it changes when needing to shift left or right.
  int mid;
  mid = (l + h) / 2;
    // Check if item is present at mid
    if (compareTo(&list[mid], &item) == 0)
    {
      printf("%d %d found at rank %d\n", list[mid].coordx, list[mid].coordy, mid + 1);
      fprintf(ofp, "%d %d found at rank %d\n", list[mid].coordx, list[mid].coordy, mid + 1);
      return;
    }
    // If item greater, ignore left half
    else if (compareTo(&item, &list[mid]) < 0)
      binarySearch(list, item, l, mid - 1);
    // If item is smaller, ignore right half
    else
      binarySearch(list, item, mid + 1, h);
}


int main(void)
{
  atexit(report_mem_leak);

  // opening input file to read and
  // output file to write to
  ifp = fopen("in.txt", "r");
  ofp = fopen("out.txt", "w");

  // storing input points
  input data = readData();

  //sorts the array of current smoothie shop coordinates
  sort(data.threshold, data.numSmoothShops, data.coord);
  
  //prints all sorted cordinates out into the file
  for (int i = 0; i < data.numSmoothShops; i++)
  {
    printf("%d %d\n", data.coord[i].coordx, data.coord[i].coordy);
    fprintf(ofp, "%d %d\n", data.coord[i].coordx, data.coord[i].coordy);
  }

  //determines if the points we are given are smoothie shops or not
  for (int j = 0; j < data.ptsToSearch; j++)
    binarySearch(data.coord, data.search[j], 0, data.numSmoothShops);

  //frees the array of coordinates we are given as smoothie shops
  //and the ones we are searching for
  free(data.coord);
  free(data.search);
}
