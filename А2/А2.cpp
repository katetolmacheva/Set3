#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <iomanip>
#include <fstream>

class ArrayGenerator {
public:
  ArrayGenerator(int maxsize = 10000, int minval = 0, int maxval = 6000, int swaps = 100)
    : maxsize_(maxsize), minval_(minval), maxval_(maxval), swaps_(swaps)
  {
    generateRandomArray();
    generateReverseSortedArray();
    generateNearlySortedArray();
  }

  std::vector<int> getRandomArray(int size) const {
    if (size > maxsize_) size = maxsize_;
    std::vector<int> s(randomarray_.begin(), randomarray_.begin() + size);
    return s;
  }

  std::vector<int> getReverseSortedArray(int size) const {
    if (size > maxsize_) size = maxsize_;
    std::vector<int> s(reversesortedarray_.begin(), reversesortedarray_.begin() + size);
    return s;
  }

  std::vector<int> getNearlySortedArray(int size) const {
    if (size > maxsize_) size = maxsize_;
    std::vector<int> s(nearlysortedarray_.begin(), nearlysortedarray_.begin() + size);
    return s;
  }

private:
  int maxsize_;
  int minval_;
  int maxval_;
  int swaps_;
  std::vector<int> randomarray_;
  std::vector<int> reversesortedarray_;
  std::vector<int> nearlysortedarray_;

  void generateRandomArray() {
    randomarray_.resize(maxsize_);
    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(minval_, maxval_);
    for (int& x : randomarray_) {
      x = dist(rng);
    }
  }

  void generateReverseSortedArray() {
    reversesortedarray_ = getRandomArray(maxsize_);
    std::sort(reversesortedarray_.begin(), reversesortedarray_.end(), std::greater<int>());
  }

  void generateNearlySortedArray() {
    nearlysortedarray_ = getRandomArray(maxsize_);
    std::sort(nearlysortedarray_.begin(), nearlysortedarray_.end());
    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(0, maxsize_ - 1);
    for (int i = 0; i < swaps_; ++i) {
      int idx1 = dist(rng);
      int idx2 = dist(rng);
      std::swap(nearlysortedarray_[idx1], nearlysortedarray_[idx2]);
    }
  }
};

class SortTester {
public:
  SortTester(int threshold = 15)
    : INSERTION_SORT_THRESHOLD(threshold)
  {}

  long long runStandardMergeSort(std::vector<int>& arr) {
    auto start = std::chrono::high_resolution_clock::now();
    standardMergeSort(arr, 0, arr.size() - 1);
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
  }

  long long runHybridSort(std::vector<int>& arr) {
    auto start = std::chrono::high_resolution_clock::now();
    hybridMergeSort(arr, 0, arr.size() - 1);
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
  }

private:
  int INSERTION_SORT_THRESHOLD;

  void standardMergeSort(std::vector<int>& arr, int left, int right) {
    if (left >= right)
      return;
    int mid = left + (right - left) / 2;
    standardMergeSort(arr, left, mid);
    standardMergeSort(arr, mid + 1, right);
    merge(arr, left, mid, right);
  }

  void hybridMergeSort(std::vector<int>& arr, int left, int right) {
    if (left >= right)
      return;
    if (right - left + 1 <= INSERTION_SORT_THRESHOLD) {
      insertionSort(arr, left, right);
      return;
    }
    int mid = left + (right - left) / 2;
    hybridMergeSort(arr, left, mid);
    hybridMergeSort(arr, mid + 1, right);
    merge(arr, left, mid, right);
  }

  void insertionSort(std::vector<int>& arr, int left, int right) {
    for (int i = left + 1; i <= right; ++i) {
      int key = arr[i];
      int j = i - 1;
      while (j >= left && arr[j] > key) {
        arr[j + 1] = arr[j];
        --j;
      }
      arr[j + 1] = key;
    }
  }

  void merge(std::vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    std::vector<int> L(n1);
    std::vector<int> R(n2);
    for (int i = 0; i < n1; ++i)
      L[i] = arr[left + i];
    for (int j = 0; j < n2; ++j)
      R[j] = arr[mid + 1 + j];
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
      if (L[i] <= R[j]) {
        arr[k++] = L[i++];
      }
      else {
        arr[k++] = R[j++];
      }
    }
    while (i < n1) {
      arr[k++] = L[i++];
    }
    while (j < n2) {
      arr[k++] = R[j++];
    }
  }
};

int main() {
  ArrayGenerator generator;
  SortTester tester(15);

  std::ofstream outfile("results.csv");
  if (!outfile.is_open()) {
    std::cerr << "Не удалось открыть файл results.csv для записи.\n";
    return 1;
  }
  outfile << "Size,Type,Algorithm,Time_microseconds\n";

  enum ArrayType { RANDOM, REVERSE, NEARLY_SORTED };
  const std::vector<std::string> typenames = { "Random", "Reverse Sorted", "Nearly Sorted" };
  const std::vector<ArrayType> arraytypes = { RANDOM, REVERSE, NEARLY_SORTED };

  for (int size = 500; size <= 10000; size += 100) {
    for (auto type : arraytypes) {
      std::vector<int> array;
      if (type == RANDOM) {
        array = generator.getRandomArray(size);
      }
      else if (type == REVERSE) {
        array = generator.getReverseSortedArray(size);
      }
      else if (type == NEARLY_SORTED) {
        array = generator.getNearlySortedArray(size);
      }

      std::vector<int> arraystandard = array;
      std::vector<int> arrayhybrid = array;
      long long timestandard = tester.runStandardMergeSort(arraystandard);
      long long timehybrid = tester.runHybridSort(arrayhybrid);
      outfile << size << "," << typenames[type] << ",Standard Merge Sort," << timestandard << "\n";
      outfile << size << "," << typenames[type] << ",Hybrid Merge+Insertion Sort," << timehybrid << "\n";
      if (size % 1000 == 0 && type == NEARLY_SORTED) {
        std::cerr << "Processed size = " << size << ", Type = " << typenames[type] << "\n";
      }
    }
  }

  outfile.close();
  std::cout << "Результаты сохранены в файле results.csv.\n";

  return 0;
}
