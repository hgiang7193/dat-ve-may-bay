#include "HashTable.h"

// ============================================================
// HashTable.cpp - Ham bam, ham lay khoa va F3 (tra cuu ve)
// ============================================================

// Time: O(L), Space: O(1)
unsigned long hashString(const char* str) {
    unsigned long hash = 5381;
    while (*str) {
        hash = hash * 33 + (unsigned char)(*str);
        str++;
    }
    return hash;
}

// Time: O(1), Space: O(1)
const char* getAirportKey(const Airport& a) { return a.airport_id; }

// Time: O(1), Space: O(1)
const char* getFlightKey(const Flight& f) { return f.flight_id; }

// Time: O(1), Space: O(1)
const char* getBookingKey(const Booking& b) { return b.booking_id; }

// F3: Tra cuu ve theo ma dat cho
// Time: O(1) trung binh (2 lan tra cuu hash), Space: O(1)
bool lookupBooking(const BookingHashTable& bookings, const FlightHashTable* flights,
                   const char* bookingId) {
    const Booking* b = bookings.find(bookingId);
    if (b == nullptr) {
        std::cout << "Khong tim thay ma dat cho: " << bookingId << "\n";
        return false;
    }

    std::cout << "=== THONG TIN VE ===\n";
    std::cout << "Ma dat cho   : " << b->booking_id << "\n";
    std::cout << "Hanh khach   : " << b->passenger_id << "\n";
    std::cout << "Chuyen bay   : " << b->flight_id << "\n";
    std::cout << "Ghe          : " << b->seat << "\n";
    std::cout << "Trang thai   : " << b->status << "\n";
    std::cout << "Thoi gian dat: " << b->booking_time << "\n";

    if (flights != nullptr) {
        const Flight* f = flights->find(b->flight_id);
        if (f != nullptr) {
            std::cout << "Hanh trinh   : " << f->origin << " -> " << f->destination << "\n";
            std::cout << "Khoi hanh    : " << f->departure_time << "\n";
        }
    }
    return true;
}
