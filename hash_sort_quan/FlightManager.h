#ifndef FLIGHTMANAGER_H
#define FLIGHTMANAGER_H

#include "../common/Models.h"
#include "HashTable.h"
#include "MergeSort.h"

// ============================================================
// FlightManager.h - F1: Quan ly chuyen bay & san bay (them/sua/xoa/hien thi)
//                   F5: Hien thi chuyen bay theo nhieu tieu chi sap xep
// Luu tru bang Hash Table (khoa: flight_id, airport_id).
// ============================================================

enum ResultCode {
    RESULT_OK = 0,
    RESULT_INVALID_DATA,        // du lieu vi pham rang buoc
    RESULT_DUPLICATE,           // trung khoa chinh
    RESULT_NOT_FOUND,           // khong ton tai
    RESULT_AIRPORT_NOT_FOUND,   // san bay di/den chua co trong he thong
    RESULT_IN_USE               // san bay con duoc chuyen bay su dung
};

// Doi ma ket qua thanh thong bao. Time: O(1)
const char* getResultMessage(ResultCode code);

// In danh sach chuyen bay dang bang. Time: O(n)
void printFlightTable(const Flight* flights, int n);

class FlightManager {
private:
    FlightHashTable flights;
    AirportHashTable airports;

    FlightManager(const FlightManager&);
    FlightManager& operator=(const FlightManager&);

    ResultCode validateFlight(const Flight& flight) const;

public:
    FlightManager() {}

    // ---- San bay ----
    ResultCode addAirport(const Airport& airport);
    ResultCode updateAirport(const Airport& airport);
    ResultCode removeAirport(const char* airportId);
    const Airport* findAirport(const char* airportId) const;
    void displayAirports() const;

    // ---- Chuyen bay ----
    ResultCode addFlight(const Flight& flight);
    ResultCode updateFlight(const Flight& flight);   // sua theo flight_id
    ResultCode removeFlight(const char* flightId);
    Flight* findFlight(const char* flightId);
    void displayFlights() const;                     // mac dinh: theo gio khoi hanh tang dan

    // ---- F5 ----
    void displaySortedFlights(const SortCriterion* criteria, int count) const;

    // Cho cac module khac (dat ve, Dijkstra, File I/O) truy cap
    FlightHashTable& getFlightTable();
    AirportHashTable& getAirportTable();
};

#endif
