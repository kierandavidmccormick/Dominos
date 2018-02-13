#include "Lib.h"

void setArrayRand(int array[], int length, int max, int min){
	debug0("=> setArrayRandStart");
	for (int i = 0; i < length; i++){
		array[i] = gRand(max, min);
		debug1("** setArrayRandIteration", i);

	}
	debug0("<= setArrayRandEnd");
}

void setArrayVal(int array[], int length, int val){
	debug0("=> setArrayValStart");
	for (int i = 0; i < length; i++){
		array[i] = val;
		debug2("** setArrayValIteration", i, val);
	}
	debug0("<= setArrayValEnd");
}

void printArray(int array[], int length){
	debug0("=> printArrayStart");
	printf("\n");
	for (int i = 0; i < length; i++){
		printf("%3d: %4d\n", i, array[i]);
		debug2("** printArrayIteration", i, array[i]);
	}
	printf("\n");
	debug0("<= printArrayEnd");
}

int gRand(int max, int min){
	debug2("=> gRandStart", max, min);
	if (max > min){
		return (rand()) % (max + 1 - min) + min;
	} else {
		return (rand()) % (min + 1 - max) + min;
	}
	debug0("<= gRandEnd");
}

bool isSorted(int array[], int length){
	debug0("=> isSortedStart");
	int prev = array[0];
	for (int i = 1; i < length; i++){
		if (prev > array[i]){
			return false;
		}
		prev = array[i];
		debug2("** isSortedIteration", i, prev);
	}
	debug0("<= isSortedEnd");
	return true;
}

void checkSorted(int array[], int length){
	debug0("=> checkSortedStart");
	if (isSorted(array, length)){
		printf("The array is sorted\n");
	} else {
		printf("The array is not sorted\n");
	}
	debug0("<= checkSortedEnd");
}

void bubbleSort(int array[], int length){
	debug0("=> bubbleSortStart");
	int prev = array[0];
	while (!isSorted(array, length)){
		debug0("** bubbleSortIteration1");
		for (int i = 1; i < length; i++){
			if (prev > array[i]){
				swap(array, i, i-1);
			}
			prev = array[i];
			debug2("** bubbleSortIteration2", i, prev);
		}
	}
	debug0("<= bubbleSortEnd");
}

void selectionSort(int array[], int length){
	debug0("=> selectionSortStart");
	for (int j = 0, pos = 0; j < length; j++, pos = j){
		for (int i = j, lowest = array[j]; i < length; i++){
			if (lowest > array[i]){
				lowest = array[i];
				pos = i;
			}
			debug4("** selectionSortIteration1", i, j, pos, lowest);
		}
		swap(array, j, pos);
		debug1("** selectionSortIteration2", j)
		printArray(array, length);
		if(DEBUG){printArray(array, length);}
	}
	debug0("<= selectionSortEnd");
}

void swap(int array[], int a, int b){
	debug2("=> swapStart", array[a], array[b]);
	int c = array[a];
	array[a] = array[b];
	array[b] = c;
	debug0("<= swapEnd");
}

void swap2(int *a, int *b){
	debug2("=> swapStart", *a, *b);
	int temp = *a;
	*a = *b;
	*b = temp;
	debug0("<= swapEnd");
}

void markFlags(int argc, char *argv[], int numVars, ...){
// format is flagName(char), &flagVariable(bool)...
	debug1("=> markFlagsStart", argc);
	char **s = argv;
	if (numVars % 2 != 0){
		printf("***** markFlags must have an even number of arguments");
	}
	char names[numVars];
	bool* pointers[numVars];
	char *a = &names[0];
	bool **b = &pointers[0];
	debug0("** Variables declared\n");
	va_list ap;
	va_start(ap, numVars);
	for (int i = 0; i < numVars; i++){
		debug1("** Variables assigned", i);
		*a++ = (char) va_arg(ap, int);
		*b++ = va_arg(ap, bool*);
	}
	debug0("** Variable assignment ended");
	a = &names[0];
	b = &pointers[0];
	debug0("** checking started");
	for (int i = 0; i < numVars; i++, a++, b++, s = argv){
		debug1("** checking1", i);
		if (DEBUG) {printf("** checking1 a: %ld, b: %ld\n", a - &names[0], b - &pointers[0]);}
		for (int j = argc; j > 0; j--, s++){
			debug2("** checking2", i, j);
			if (DEBUG) {printf("** checking2 a: %ld, b: %ld\n", a - &names[0], b - &pointers[0]);}
			if (((*s)[0] == '-') && ((*s)[1] == *a)){
				**b = true;
			}
			debug2("** checking3", i, j);
		}
	}
	va_end(ap);
}

int stoi(char* s){
	debug0("=> stoiStart");
	char* so = s;
	int i = 0;
	int j = 0;
	int number = 0;
	while (*s++ != '\0'){
		i++;
		j++;
	}
	s -= 2;
	debug2("** stoiVal", i, j)
	for (; i > 0; i--, s--){
		if (ctoi(*s) != -1){
			number += pow(10, (j - i)) * ctoi(*s);
		} else {
			j--;
		}
		debug4("** stoiVal2", i, j, number, ctoi(*s));
	}
	debug0("<= stoiEnd");
	return number;
}

int ctoi(char c){
	debug0("=> ctoiStart");
	int number;
	switch (c){
		case '0': number = 0;
		break;
		case '1': number = 1;
		break;
		case '2': number = 2;
		break;
		case '3': number = 3;
		break;
		case '4': number = 4;
		break;
		case '5': number = 5;
		break;
		case '6': number = 6;
		break;
		case '7': number = 7;
		break;
		case '8': number = 8;
		break;
		case '9': number = 9;
		break;
		default: number = -1;
		printf("***** Illegal charachter value for ctoi: (\'");
		putchar(c);
		printf("\')\n");
		break;
	}
	debug1("** ctoiVal", number);
	debug0("<= ctoiEnd");
	return number;
}

void itos(char string[], int length, int number){
// make sure array is correct length
// for 0-99 only
	debug1("** => itosStart", number);
	if (number < 10){
		string[length - 2] = itoc(number);
	} else {
		string[length - 3] = (int) itoc(floor(number / 10));
		number -= 10 * (int) floor(number / 10);
		itos(string, length, number);
	}
	debug0("<= itosEnd");
}

char itoc(int n){
	debug1("=> itocStart", n);
	char c;
	switch (n){
		case 0: c = '0';
		break;
		case 1: c = '1';
		break;
		case 2: c = '2';
		break;
		case 3: c = '3';
		break;
		case 4: c = '4';
		break;
		case 5: c = '5';
		break;
		case 6: c = '6';
		break;
		case 7: c = '7';
		break;
		case 8: c = '8';
		break;
		case 9: c = '9';
		break;
		default: c = '\0';
		printf("Illegal value for itoc: (%d)\n", n);
		break;
	}
	if (DEBUG){printf("** itoc c: \'%c\'\n", c);}
	debug0("<= itocEnd");
	return c;
}









	
