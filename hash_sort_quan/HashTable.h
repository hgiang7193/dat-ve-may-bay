#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "../common/Models.h"

// ============================================================
// HashTable.h - Muc 3.1: Quan ly chuyen bay & tra cuu ve (F3)
// Cau truc: Hash Table, xu ly dung do bang chaining (danh sach lien ket).
// Tu dong rehash (x2 dung luong) khi load factor > 0.75.
// Ky hieu: n = so phan tu, m = so bucket, L = do dai khoa.
// Dung chung 1 template cho Airport / Flight / Booking.
// ============================================================

// Ham bam chuoi djb2. Time: O(L), Space: O(1)
unsigned long hashString(const char* str);

template <typename T>
struct HashNode {
    T data;
    HashNode<T>* next;
};

template <typename T>
class HashTable {
public:
    typedef const char* (*KeyFunc)(const T&);   // ham lay khoa (chuoi) tu phan tu

private:
    HashNode<T>** buckets;
    int capacity;     // so bucket (m)
    int size;         // so phan tu (n)
    KeyFunc getKey;

    HashTable(const HashTable&);                // cam copy
    HashTable& operator=(const HashTable&);     // cam gan

    int getIndex(const char* key) const;
    void rehash();

public:
    explicit HashTable(KeyFunc keyFunc, int initialCapacity = 16);
    ~HashTable();

    bool insert(const T& item);            // them (false neu trung khoa)
    bool remove(const char* key);          // xoa theo khoa
    T* find(const char* key);              // tra cuu, nullptr neu khong co
    const T* find(const char* key) const;
    int toArray(T* out) const;             // xuat ra mang (de dua vao MergeSort)
    void clear();
    int getSize() const;
    int getCapacity() const;
};

// ------------------------------------------------------------
// Cai dat template (bat buoc nam trong header)
// ------------------------------------------------------------

// Time: O(m), Space: O(m)
template <typename T>
HashTable<T>::HashTable(KeyFunc keyFunc, int initialCapacity) {
    getKey = keyFunc;
    capacity = (initialCapacity < 8) ? 8 : initialCapacity;
    size = 0;
    buckets = new HashNode<T>*[capacity];
    for (int i = 0; i < capacity; i++) buckets[i] = nullptr;
}

// Time: O(n + m), Space: O(1)
template <typename T>
HashTable<T>::~HashTable() {
    clear();
    delete[] buckets;
}

// Time: O(L), Space: O(1)
template <typename T>
int HashTable<T>::getIndex(const char* key) const {
    return (int)(hashString(key) % (unsigned long)capacity);
}

// Time: O(n + m) (chi relink node, khong cap phat lai node), Space: O(m)
template <typename T>
void HashTable<T>::rehash() {
    int newCapacity = capacity * 2;
    HashNode<T>** newBuckets = new HashNode<T>*[newCapacity];
    for (int i = 0; i < newCapacity; i++) newBuckets[i] = nullptr;

    for (int i = 0; i < capacity; i++) {
        HashNode<T>* cur = buckets[i];
        while (cur != nullptr) {
            HashNode<T>* nextNode = cur->next;
            int idx = (int)(hashString(getKey(cur->data)) % (unsigned long)newCapacity);
            cur->next = newBuckets[idx];
            newBuckets[idx] = cur;
            cur = nextNode;
        }
    }
    delete[] buckets;
    buckets = newBuckets;
    capacity = newCapacity;
}

// Time: O(1) trung binh (amortized, tinh ca rehash), O(n) xau nhat; Space: O(1)
template <typename T>
bool HashTable<T>::insert(const T& item) {
    if (find(getKey(item)) != nullptr) return false;   // trung khoa
    if ((size + 1) * 4 > capacity * 3) rehash();       // load factor > 0.75

    int idx = getIndex(getKey(item));
    HashNode<T>* node = new HashNode<T>;
    node->data = item;
    node->next = buckets[idx];
    buckets[idx] = node;
    size++;
    return true;
}

// Time: O(1) trung binh, O(n) xau nhat; Space: O(1)
template <typename T>
const T* HashTable<T>::find(const char* key) const {
    int idx = getIndex(key);
    HashNode<T>* cur = buckets[idx];
    while (cur != nullptr) {
        if (std::strcmp(getKey(cur->data), key) == 0) return &cur->data;
        cur = cur->next;
    }
    return nullptr;
}

// Time: O(1) trung binh, O(n) xau nhat; Space: O(1)
template <typename T>
T* HashTable<T>::find(const char* key) {
    return const_cast<T*>(static_cast<const HashTable<T>*>(this)->find(key));
}

// Time: O(1) trung binh, O(n) xau nhat; Space: O(1)
template <typename T>
bool HashTable<T>::remove(const char* key) {
    int idx = getIndex(key);
    HashNode<T>* cur = buckets[idx];
    HashNode<T>* prev = nullptr;
    while (cur != nullptr) {
        if (std::strcmp(getKey(cur->data), key) == 0) {
            if (prev == nullptr) buckets[idx] = cur->next;
            else prev->next = cur->next;
            delete cur;
            size--;
            return true;
        }
        prev = cur;
        cur = cur->next;
    }
    return false;
}

// Time: O(n + m), Space: O(1) (mang out do nguoi goi cap phat, du n phan tu)
template <typename T>
int HashTable<T>::toArray(T* out) const {
    int count = 0;
    for (int i = 0; i < capacity; i++) {
        HashNode<T>* cur = buckets[i];
        while (cur != nullptr) {
            out[count++] = cur->data;
            cur = cur->next;
        }
    }
    return count;
}

// Time: O(n + m), Space: O(1)
template <typename T>
void HashTable<T>::clear() {
    for (int i = 0; i < capacity; i++) {
        HashNode<T>* cur = buckets[i];
        while (cur != nullptr) {
            HashNode<T>* nextNode = cur->next;
            delete cur;
            cur = nextNode;
        }
        buckets[i] = nullptr;
    }
    size = 0;
}

// Time: O(1), Space: O(1)
template <typename T>
int HashTable<T>::getSize() const { return size; }

// Time: O(1), Space: O(1)
template <typename T>
int HashTable<T>::getCapacity() const { return capacity; }

// ------------------------------------------------------------
// Cac bang bam cu the
// ------------------------------------------------------------
const char* getAirportKey(const Airport& a);   // airport_id
const char* getFlightKey(const Flight& f);     // flight_id
const char* getBookingKey(const Booking& b);   // booking_id

class AirportHashTable : public HashTable<Airport> {
public:
    explicit AirportHashTable(int initialCapacity = 16)
        : HashTable<Airport>(getAirportKey, initialCapacity) {}
};

class FlightHashTable : public HashTable<Flight> {
public:
    explicit FlightHashTable(int initialCapacity = 16)
        : HashTable<Flight>(getFlightKey, initialCapacity) {}
};

class BookingHashTable : public HashTable<Booking> {
public:
    explicit BookingHashTable(int initialCapacity = 16)
        : HashTable<Booking>(getBookingKey, initialCapacity) {}
};

// ------------------------------------------------------------
// F3 - Tra cuu ve theo ma dat cho
// In thong tin ve (kem thong tin chuyen bay neu flights != nullptr).
// Tra true neu tim thay, false (va in thong bao) neu khong ton tai.
// Time: O(1) trung binh
// ------------------------------------------------------------
bool lookupBooking(const BookingHashTable& bookings, const FlightHashTable* flights,
                   const char* bookingId);

#endif
