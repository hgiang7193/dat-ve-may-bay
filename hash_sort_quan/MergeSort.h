#ifndef MERGESORT_H
#define MERGESORT_H

#include "../common/Models.h"

// ============================================================
// MergeSort.h - Muc 3.4.1 + F5: Sap xep chuyen bay on dinh (stable)
// Sap xep theo gia ve / gio khoi hanh, ho tro nhieu tieu chi lien tiep.
// ============================================================

// Con tro ham so sanh: tra true neu a phai dung TRUOC b (so sanh nghiem ngat).
// Hai phan tu bang nhau (ca hai chieu deu false) giu nguyen thu tu ban dau => on dinh.
typedef bool (*FlightCompare)(const Flight& a, const Flight& b);

bool compareByPriceAsc(const Flight& a, const Flight& b);        // gia tang dan
bool compareByPriceDesc(const Flight& a, const Flight& b);       // gia giam dan
bool compareByDepartTimeAsc(const Flight& a, const Flight& b);   // departure_time tang dan
bool compareByDepartTimeDesc(const Flight& a, const Flight& b);  // giam dan

// Sap xep tong quat voi ham so sanh tuy chon
void mergeSortFlights(Flight* arr, int n, FlightCompare cmp);

void sortFlightsByPrice(Flight* arr, int n, bool ascending = true);
void sortFlightsByDepartTime(Flight* arr, int n, bool ascending = true);

// ---------------- F5: sap xep theo nhieu tieu chi lien tiep ----------------
enum SortField { SORT_BY_PRICE, SORT_BY_DEPARTURE_TIME };

struct SortCriterion {
    SortField field;
    bool ascending;
};

// criteria[0] la tieu chi uu tien cao nhat, criteria[1] dung de phan dinh khi
// criteria[0] bang nhau, ... Cach lam: chay Merge Sort on dinh tu tieu chi cuoi len dau.
void sortFlightsByCriteria(Flight* arr, int n, const SortCriterion* criteria, int count);

// Kiem tra mang da duoc sap xep theo cmp hay chua (dung cho test)
bool isSortedFlights(const Flight* arr, int n, FlightCompare cmp);

#endif
