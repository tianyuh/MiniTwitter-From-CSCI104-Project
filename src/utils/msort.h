#ifndef MSORT_H
#define MSORT_H

#include <vector>
#include <iostream>

template <class T, class Comparator>

class Msort{
public:

void mergeSort(std::vector<T>& myArray, Comparator comp){

	if(myArray.size() <= 1){
		return;
	}
	std::vector<T> tempArray(myArray.size());
	mergeSortHelper(myArray, 0, myArray.size()-1, comp, tempArray);
}

void mergeSortHelper(std::vector<T>& myArray, int start, int end, Comparator comp, std::vector<T>& tempArray){
	if(start >= end){
		return;
	}
	int middle = (start + end)/2;
	mergeSortHelper(myArray, start, middle, comp, tempArray);
	mergeSortHelper(myArray, middle + 1, end, comp, tempArray);
	mergeHelper(myArray, start, end, comp, tempArray);
}

void mergeHelper(std::vector<T>& myArray, int start, int end, Comparator comp, std::vector<T>& tempArray){
	int leftStart = start;
	int leftEnd = (start + end)/2;
	int rightStart = leftEnd+1;
	int rightEnd = end;

	int left = leftStart;
	int right = rightStart;
	int i = leftStart;

	// Merge
	while(left <= leftEnd && right <= rightEnd){

		if(comp(myArray[left], myArray[right])){
			tempArray[i] = myArray[left];
			left++;
		}
		else{
			tempArray[i] = myArray[right];
			right++;
		}
		i++;
	}

	// Copy the rest of the elements from myArray to tempArray
	for(int j = 0; j <= leftEnd-left; j++){
		tempArray[i+j] = myArray[j+left];
	}
	for(int j = 0; j <= rightEnd - right; j++){
		tempArray[i+j] = myArray[j+right];
	}

	// Copy everything from tempArray back to myArray
	for(int k = leftStart; k <= rightEnd; k++){
		myArray[k] = tempArray[k];
	}
}

};

#endif