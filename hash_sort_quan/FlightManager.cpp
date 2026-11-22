#include "FlightManager.h"

// ============================================================
// FlightManager.cpp - F1 (them/sua/xoa/hien thi) va F5 (sap xep + hien thi)
// ============================================================

// Time: O(1), Space: O(1)
const char* getResultMessage(ResultCode code) {
    switch (code) {
        case RESULT_OK:                return "Thanh cong";
        case RESULT_INVALID_DATA:      return "Loi: du lieu khong hop le";
        case RESULT_DUPLICATE:         return "Loi: ma da ton tai";
        case RESULT_NOT_FOUND:         return "Loi: khong tim thay";
        case RESULT_AIRPORT_NOT_FOUND: return "Loi: san bay di/den khong ton tai";
        case RESULT_IN_USE:            return "Loi: san bay dang duoc chuyen bay su dung";
    }
    return "Loi khong xac dinh";
}

static bool isDigit(char c) { return c >= '0' && c <= '9'; }

// Kiem tra dinh dang "YYYY-MM-DD HH:MM" (can thiet vi MergeSort so sanh bang chuoi).
// Time: O(1), Space: O(1)
static bool isValidDepartureTime(const char* t) {
    if (std::strlen(t) != 16) return false;
    for (int i = 0; i < 16; i++) {
        if (i == 4 || i == 7) { if (t[i] != '-') return false; }
        else if (i == 10)     { if (t[i] != ' ') return false; }
        else if (i == 13)     { if (t[i] != ':') return false; }
        else if (!isDigit(t[i])) return false;
    }
    int month = (t[5] - '0') * 10 + (t[6] - '0');
    int day   = (t[8] - '0') * 10 + (t[9] - '0');
    int hour  = (t[11] - '0') * 10 + (t[12] - '0');
    int min   = (t[14] - '0') * 10 + (t[15] - '0');
    return month >= 1 && month <= 12 && day >= 1 && day <= 31 && hour < 24 && min < 60;
}

// Ghi so nguyen vao buf (khong dung sprintf). Time: O(digits), Space: O(1)
static void numberToString(long long v, char* buf) {
    char tmp[24];
    int len = 0;
    bool neg = v < 0;
    if (neg) v = -v;
    if (v == 0) tmp[len++] = '0';
    while (v > 0) { tmp[len++] = (char)('0' + v % 10); v /= 10; }
    int pos = 0;
    if (neg) buf[pos++] = '-';
    while (len > 0) buf[pos++] = tmp[--len];
    buf[pos] = '\0';
}

// In chuoi va them khoang trang cho du do rong. Time: O(width)
static void printPadded(const char* s, int width) {
    std::cout << s;
    for (int i = (int)std::strlen(s); i < width; i++) std::cout << ' ';
}

// Time: O(n), Space: O(1)
void printFlightTable(const Flight* flights, int n) {
    if (n == 0) {
        std::cout << "(Khong co chuyen bay)\n";
        return;
    }
    printPadded("Ma CB", 8);   printPadded("Tuyen", 8);
    printPadded("Di", 6);      printPadded("Den", 6);
    printPadded("Khoi hanh", 18);
    printPadded("Gia", 10);    printPadded("Tong ghe", 10);  std::cout << "Con trong\n";

    char buf[24];
    for (int i = 0; i < n; i++) {
        printPadded(flights[i].flight_id, 8);
        printPadded(flights[i].route_id, 8);
        printPadded(flights[i].origin, 6);
        printPadded(flights[i].destination, 6);
        printPadded(flights[i].departure_time, 18);
        numberToString((long long)flights[i].price, buf);  printPadded(buf, 10);
        numberToString(flights[i].total_seats, buf);       printPadded(buf, 10);
        numberToString(flights[i].available_seats, buf);   std::cout << buf << "\n";
    }
}

// ======================= San bay =======================

// Time: O(1) trung binh, Space: O(1)
ResultCode FlightManager::addAirport(const Airport& airport) {
    if (!airport.isValid()) return RESULT_INVALID_DATA;
    if (!airports.insert(airport)) return RESULT_DUPLICATE;
    return RESULT_OK;
}

// Sua ten/thanh pho cua san bay (khong doi airport_id). Time: O(1) trung binh
ResultCode FlightManager::updateAirport(const Airport& airport) {
    if (!airport.isValid()) return RESULT_INVALID_DATA;
    Airport* existing = airports.find(airport.airport_id);
    if (existing == nullptr) return RESULT_NOT_FOUND;
    *existing = airport;
    return RESULT_OK;
}

// Khong cho xoa san bay dang co chuyen bay di/den.
// Time: O(n + m) vi phai duyet cac chuyen bay, Space: O(n)
ResultCode FlightManager::removeAirport(const char* airportId) {
    if (airports.find(airportId) == nullptr) return RESULT_NOT_FOUND;

    int n = flights.getSize();
    Flight* arr = new Flight[n > 0 ? n : 1];
    flights.toArray(arr);
    bool inUse = false;
    for (int i = 0; i < n && !inUse; i++) {
        if (std::strcmp(arr[i].origin, airportId) == 0 ||
            std::strcmp(arr[i].destination, airportId) == 0) inUse = true;
    }
    delete[] arr;

    if (inUse) return RESULT_IN_USE;
    airports.remove(airportId);
    return RESULT_OK;
}

// Time: O(1) trung binh, Space: O(1)
const Airport* FlightManager::findAirport(const char* airportId) const {
    return airports.find(airportId);
}

// Time: O(k log k + m), k = so san bay, m = so bucket, Space: O(k)
void FlightManager::displayAirports() const {
    int n = airports.getSize();
    if (n == 0) { std::cout << "(Khong co san bay)\n"; return; }
    Airport* arr = new Airport[n];
    airports.toArray(arr);

    // Sap theo airport_id (insertion sort, k nho)
    for (int i = 1; i < n; i++) {
        Airport key = arr[i];
        int j = i - 1;
        while (j >= 0 && std::strcmp(arr[j].airport_id, key.airport_id) > 0) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }

    printPadded("Ma", 6); printPadded("Ten san bay", 24); std::cout << "Thanh pho\n";
    for (int i = 0; i < n; i++) {
        printPadded(arr[i].airport_id, 6);
        printPadded(arr[i].name, 24);
        std::cout << arr[i].city << "\n";
    }
    delete[] arr;
}

// ======================= Chuyen bay =======================

// Kiem tra rang buoc: isValid(), dinh dang gio, origin != destination,
// san bay di/den phai ton tai. (route_id thuoc module Route/Dijkstra nen khong kiem tra o day.)
// Time: O(1) trung binh, Space: O(1)
ResultCode FlightManager::validateFlight(const Flight& flight) const {
    if (!flight.isValid()) return RESULT_INVALID_DATA;
    if (!isValidDepartureTime(flight.departure_time)) return RESULT_INVALID_DATA;
    if (std::strcmp(flight.origin, flight.destination) == 0) return RESULT_INVALID_DATA;
    if (airports.find(flight.origin) == nullptr) return RESULT_AIRPORT_NOT_FOUND;
    if (airports.find(flight.destination) == nullptr) return RESULT_AIRPORT_NOT_FOUND;
    return RESULT_OK;
}

// Time: O(1) trung binh, Space: O(1)
ResultCode FlightManager::addFlight(const Flight& flight) {
    ResultCode code = validateFlight(flight);
    if (code != RESULT_OK) return code;
    if (!flights.insert(flight)) return RESULT_DUPLICATE;
    return RESULT_OK;
}

// Ghi de toan bo thong tin chuyen bay co cung flight_id. Time: O(1) trung binh
ResultCode FlightManager::updateFlight(const Flight& flight) {
    Flight* existing = flights.find(flight.flight_id);
    if (existing == nullptr) return RESULT_NOT_FOUND;
    ResultCode code = validateFlight(flight);
    if (code != RESULT_OK) return code;
    *existing = flight;
    return RESULT_OK;
}

// Time: O(1) trung binh, Space: O(1)
ResultCode FlightManager::removeFlight(const char* flightId) {
    return flights.remove(flightId) ? RESULT_OK : RESULT_NOT_FOUND;
}

// Time: O(1) trung binh, Space: O(1)
Flight* FlightManager::findFlight(const char* flightId) {
    return flights.find(flightId);
}

// Time: O(n log n + m), Space: O(n)
void FlightManager::displayFlights() const {
    int n = flights.getSize();
    Flight* arr = new Flight[n > 0 ? n : 1];
    flights.toArray(arr);
    sortFlightsByDepartTime(arr, n, true);
    printFlightTable(arr, n);
    delete[] arr;
}

// F5: Time: O(k * n log n + m), Space: O(n)
void FlightManager::displaySortedFlights(const SortCriterion* criteria, int count) const {
    int n = flights.getSize();
    Flight* arr = new Flight[n > 0 ? n : 1];
    flights.toArray(arr);
    sortFlightsByCriteria(arr, n, criteria, count);
    printFlightTable(arr, n);
    delete[] arr;
}

// Time: O(1), Space: O(1)
FlightHashTable& FlightManager::getFlightTable() { return flights; }

// Time: O(1), Space: O(1)
AirportHashTable& FlightManager::getAirportTable() { return airports; }
