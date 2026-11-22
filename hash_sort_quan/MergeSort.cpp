#include "MergeSort.h"

// ============================================================
// MergeSort.cpp - Cai dat Merge Sort (top-down, on dinh)
// Tong the: Time O(n log n) moi truong hop (tot/trung binh/xau),
//           Space O(n) cho mang tam + O(log n) ngan xep de quy.
// ============================================================

// Time: O(1), Space: O(1)
bool compareByPriceAsc(const Flight& a, const Flight& b) {
    return a.price < b.price;
}

// Time: O(1), Space: O(1)
bool compareByPriceDesc(const Flight& a, const Flight& b) {
    return a.price > b.price;
}

// Dinh dang "YYYY-MM-DD HH:MM" nen so sanh chuoi (strcmp) cung la so sanh thoi gian.
// Time: O(1) (chuoi co do dai co dinh 16 ky tu), Space: O(1)
bool compareByDepartTimeAsc(const Flight& a, const Flight& b) {
    return std::strcmp(a.departure_time, b.departure_time) < 0;
}

// Time: O(1), Space: O(1)
bool compareByDepartTimeDesc(const Flight& a, const Flight& b) {
    return std::strcmp(a.departure_time, b.departure_time) > 0;
}

// Tron hai nua da sap xep arr[left..mid] va arr[mid+1..right].
// On dinh: chi lay phan tu nua phai khi NO NHO HON HAN nua trai.
// Time: O(right - left + 1), Space: O(right - left + 1) (dung mang tam temp)
static void mergeRange(Flight* arr, Flight* temp, int left, int mid, int right, FlightCompare cmp) {
    int i = left;       // con tro nua trai
    int j = mid + 1;    // con tro nua phai
    int k = left;       // con tro mang tam

    while (i <= mid && j <= right) {
        if (cmp(arr[j], arr[i])) temp[k++] = arr[j++];
        else                     temp[k++] = arr[i++];
    }
    while (i <= mid)   temp[k++] = arr[i++];
    while (j <= right) temp[k++] = arr[j++];

    for (k = left; k <= right; k++) arr[k] = temp[k];
}

// Chia doi de quy roi tron.
// Time: O(n log n), Space: O(log n) ngan xep de quy
static void mergeSortRange(Flight* arr, Flight* temp, int left, int right, FlightCompare cmp) {
    if (left >= right) return;
    int mid = left + (right - left) / 2;
    mergeSortRange(arr, temp, left, mid, cmp);
    mergeSortRange(arr, temp, mid + 1, right, cmp);
    mergeRange(arr, temp, left, mid, right, cmp);
}

// Time: O(n log n) moi truong hop, Space: O(n)
void mergeSortFlights(Flight* arr, int n, FlightCompare cmp) {
    if (arr == nullptr || n <= 1 || cmp == nullptr) return;
    Flight* temp = new Flight[n];   // cap phat 1 lan duy nhat
    mergeSortRange(arr, temp, 0, n - 1, cmp);
    delete[] temp;
}

// Time: O(n log n), Space: O(n)
void sortFlightsByPrice(Flight* arr, int n, bool ascending) {
    mergeSortFlights(arr, n, ascending ? compareByPriceAsc : compareByPriceDesc);
}

// Time: O(n log n), Space: O(n)
void sortFlightsByDepartTime(Flight* arr, int n, bool ascending) {
    mergeSortFlights(arr, n, ascending ? compareByDepartTimeAsc : compareByDepartTimeDesc);
}

// Chon ham so sanh theo tieu chi. Time: O(1), Space: O(1)
static FlightCompare getComparator(const SortCriterion& c) {
    if (c.field == SORT_BY_PRICE)
        return c.ascending ? compareByPriceAsc : compareByPriceDesc;
    return c.ascending ? compareByDepartTimeAsc : compareByDepartTimeDesc;
}

// F5: Sap xep theo nhieu tieu chi. Vi Merge Sort on dinh, sap theo tieu chi cuoi
// truoc, tieu chi dau sau => tieu chi dau co uu tien cao nhat.
// Time: O(k * n log n) voi k = so tieu chi, Space: O(n)
void sortFlightsByCriteria(Flight* arr, int n, const SortCriterion* criteria, int count) {
    if (criteria == nullptr) return;
    for (int i = count - 1; i >= 0; i--) {
        mergeSortFlights(arr, n, getComparator(criteria[i]));
    }
}

// Time: O(n), Space: O(1)
bool isSortedFlights(const Flight* arr, int n, FlightCompare cmp) {
    for (int i = 1; i < n; i++) {
        if (cmp(arr[i], arr[i - 1])) return false;
    }
    return true;
}
