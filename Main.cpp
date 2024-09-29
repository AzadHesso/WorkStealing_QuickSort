#include <iostream>
#include <chrono>
#include <random>
#include "RequestHandler.h"

bool make_thread = true;

RequestHandler rh;

std::vector<res_type> results;

void quicksort(int* array, long left, long right)
{
    if (left >= right) return;
    long left_bound = left;
    long right_bound = right;

    long middle = array[(left_bound + right_bound) / 2];

    do {
        while (array[left_bound] < middle) {
            left_bound++;
        }
        while (array[right_bound] > middle) {
            right_bound--;
        }

        if (left_bound <= right_bound) {
            std::swap(array[left_bound], array[right_bound]);
            left_bound++;
            right_bound--;
        }
    } while (left_bound <= right_bound);

   
    if (make_thread && (right_bound - left > 100000))
    { 
        res_type result = rh.pushRequest(quicksort, array, left, right_bound);
        result.wait();

        quicksort(array, left_bound, right);       
    }
    else {
        quicksort(array, left, right_bound);
        quicksort(array, left_bound, right);
    }
}


int main()
{
    srand(0);
    long arr_size = 10000000;
    int* array = new int[arr_size];
    if (array == nullptr) {
        return 0;
    }
    for (long i = 0; i < arr_size; i++) {
        array[i] = rand() % 50000;
    }

    cout << "Running single-threaded quickSort..." << endl;
    time_t start, end;
    make_thread = false;
    time(&start);
    quicksort(array, 0, arr_size - 1);
    time(&end);
    double seconds = difftime(end, start);
    printf("Execution time: %f seconds\n", seconds);
    
    for (long i = 0; i < arr_size - 1; i++) {
        if (array[i] > array[i + 1]) {
            cout << "Unsorted" << endl;
            break;
        }
    }

    for (long i = 0; i < arr_size; i++) {
    array[i] = rand() % 500000;
    }  

    cout << "Running multithreaded quickSort..." << endl;
    time(&start);
    quicksort(array, 0, arr_size - 1);
    time(&end);
  
    seconds = difftime(end, start);
    printf("Execution time: %f seconds\n", seconds);

    delete[] array;

    return 0;
}