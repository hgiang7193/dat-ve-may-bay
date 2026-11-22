#include <iostream>
#include <cstring>
#include "FlightManager.h"

// File test doc lap cho 3.1, 3.4.1, F1, F3, F5
// Bien dich (Models.h nam o ../common/Models.h):
//   g++ -std=c++11 -Wall TestHashSort.cpp FlightManager.cpp HashTable.cpp MergeSort.cpp -o test_hash_sort
using namespace std;

static int passed = 0, failed = 0;
static void check(bool ok, const char* name) {
    ok ? passed++ : failed++;
    cout << (ok ? "[OK]   " : "[FAIL] ") << name << "\n";
}

static unsigned long seedValue = 12345;
static unsigned long nextRandom() {
    seedValue = seedValue * 1103515245UL + 12345UL;
    return (seedValue >> 16) & 0x7fff;
}

static void makeCode(char* out, const char* prefix, int num) {
    int len = (int)strlen(prefix);
    strcpy(out, prefix);
    char digits[12];
    int d = 0;
    if (num == 0) digits[d++] = '0';
    while (num > 0) { digits[d++] = (char)('0' + num % 10); num /= 10; }
    for (int i = 0; i < d; i++) out[len + i] = digits[d - 1 - i];
    out[len + d] = '\0';
}

static void makeTime(char* out, int day, int minute) {
    int hh = minute / 60, mm = minute % 60;
    strcpy(out, "2026-11-00 00:00");
    out[8] = (char)('0' + day / 10);  out[9] = (char)('0' + day % 10);
    out[11] = (char)('0' + hh / 10);  out[12] = (char)('0' + hh % 10);
    out[14] = (char)('0' + mm / 10);  out[15] = (char)('0' + mm % 10);
}

static Flight makeFlight(const char* id, const char* route, const char* from, const char* to,
                         const char* time, double price, int total, int avail) {
    Flight f;
    strcpy(f.flight_id, id);  strcpy(f.route_id, route);
    strcpy(f.origin, from);   strcpy(f.destination, to);
    strcpy(f.departure_time, time);
    f.price = price;  f.total_seats = total;  f.available_seats = avail;
    return f;
}

static Airport makeAirport(const char* id, const char* name, const char* city) {
    Airport a;
    strcpy(a.airport_id, id);  strcpy(a.name, name);  strcpy(a.city, city);
    return a;
}

static Booking makeBooking(const char* id, const char* pax, const char* flight,
                           const char* seat, const char* status, const char* time) {
    Booking b;
    strcpy(b.booking_id, id);  strcpy(b.passenger_id, pax);  strcpy(b.flight_id, flight);
    strcpy(b.seat, seat);      strcpy(b.status, status);     strcpy(b.booking_time, time);
    return b;
}

static bool orderMatches(const Flight* arr, const char** expected, int n) {
    for (int i = 0; i < n; i++)
        if (strcmp(arr[i].flight_id, expected[i]) != 0) return false;
    return true;
}

int main() {
    // ===================== F1: Quan ly san bay / chuyen bay =====================
    cout << "--- F1: Quan ly chuyen bay & san bay ---\n";
    FlightManager mgr;
    check(mgr.addAirport(makeAirport("HAN", "Noi Bai", "Ha Noi")) == RESULT_OK, "Them san bay HAN");
    mgr.addAirport(makeAirport("SGN", "Tan Son Nhat", "TP HCM"));
    mgr.addAirport(makeAirport("DAD", "Da Nang", "Da Nang"));
    mgr.addAirport(makeAirport("CXR", "Cam Ranh", "Khanh Hoa"));
    mgr.addAirport(makeAirport("PQC", "Phu Quoc", "Kien Giang"));
    mgr.addAirport(makeAirport("HUI", "Phu Bai", "Hue"));
    mgr.addAirport(makeAirport("DLI", "Lien Khuong", "Lam Dong"));
    check(mgr.addAirport(makeAirport("HAN", "X", "Y")) == RESULT_DUPLICATE, "Them san bay trung ma -> DUPLICATE");
    check(mgr.addAirport(Airport()) == RESULT_INVALID_DATA, "Them san bay rong -> INVALID_DATA");
    check(mgr.updateAirport(makeAirport("HAN", "Noi Bai Intl", "Ha Noi")) == RESULT_OK
          && strcmp(mgr.findAirport("HAN")->name, "Noi Bai Intl") == 0, "Sua san bay");

    Flight sample[11] = {
        makeFlight("VN101", "RT001", "HAN", "SGN", "2026-11-20 08:00", 1550000, 180, 120),
        makeFlight("VN102", "RT001", "HAN", "SGN", "2026-11-20 14:30", 1450000, 180, 45),
        makeFlight("VJ201", "RT003", "HAN", "DAD", "2026-11-20 07:15", 850000, 150, 0),
        makeFlight("VJ202", "RT005", "DAD", "SGN", "2026-11-20 11:00", 900000, 150, 62),
        makeFlight("QH301", "RT007", "HAN", "CXR", "2026-11-20 09:30", 1350000, 160, 88),
        makeFlight("QH302", "RT008", "CXR", "SGN", "2026-11-20 13:45", 700000, 160, 110),
        makeFlight("VN401", "RT009", "SGN", "PQC", "2026-11-21 06:45", 820000, 120, 0),
        makeFlight("VJ501", "RT014", "HAN", "HUI", "2026-11-21 10:00", 750000, 150, 95),
        makeFlight("VJ502", "RT015", "HUI", "SGN", "2026-11-21 15:20", 950000, 150, 74),
        makeFlight("QH601", "RT012", "DAD", "DLI", "2026-11-21 08:30", 750000, 120, 50),
        makeFlight("QH602", "RT013", "DLI", "SGN", "2026-11-21 12:15", 650000, 120, 30)
    };
    bool allAdded = true;
    for (int i = 0; i < 11; i++) allAdded = (mgr.addFlight(sample[i]) == RESULT_OK) && allAdded;
    check(allAdded && mgr.getFlightTable().getSize() == 11, "Them 11 chuyen bay mau");

    check(mgr.addFlight(sample[0]) == RESULT_DUPLICATE, "Them chuyen bay trung ma -> DUPLICATE");
    check(mgr.addFlight(makeFlight("ZZ1", "RT1", "XXX", "SGN", "2026-11-20 08:00", 1, 10, 5)) == RESULT_AIRPORT_NOT_FOUND,
          "Them chuyen bay san bay khong ton tai -> AIRPORT_NOT_FOUND");
    check(mgr.addFlight(makeFlight("ZZ2", "RT1", "HAN", "SGN", "2026-11-20 08:00", 1, 10, 11)) == RESULT_INVALID_DATA,
          "available_seats > total_seats -> INVALID_DATA");
    check(mgr.addFlight(makeFlight("ZZ3", "RT1", "HAN", "SGN", "2026-13-45 99:99", 1, 10, 5)) == RESULT_INVALID_DATA,
          "Gio khoi hanh sai dinh dang -> INVALID_DATA");
    check(mgr.addFlight(makeFlight("ZZ4", "RT1", "HAN", "HAN", "2026-11-20 08:00", 1, 10, 5)) == RESULT_INVALID_DATA,
          "origin == destination -> INVALID_DATA");
    check(mgr.addFlight(makeFlight("ZZ5", "RT1", "HAN", "SGN", "2026-11-20 08:00", -5, 10, 5)) == RESULT_INVALID_DATA,
          "Gia am -> INVALID_DATA");

    Flight edited = sample[0];
    edited.price = 1600000;
    check(mgr.updateFlight(edited) == RESULT_OK && mgr.findFlight("VN101")->price == 1600000, "Sua chuyen bay VN101");
    check(mgr.updateFlight(makeFlight("NOPE", "R", "HAN", "SGN", "2026-11-20 08:00", 1, 10, 5)) == RESULT_NOT_FOUND,
          "Sua chuyen bay khong ton tai -> NOT_FOUND");
    check(mgr.removeFlight("VN102") == RESULT_OK && mgr.removeFlight("VN102") == RESULT_NOT_FOUND, "Xoa chuyen bay (2 lan)");
    check(mgr.removeAirport("SGN") == RESULT_IN_USE, "Xoa san bay dang duoc dung -> IN_USE");
    mgr.addAirport(makeAirport("VII", "Vinh", "Nghe An"));
    check(mgr.removeAirport("VII") == RESULT_OK && mgr.removeAirport("VII") == RESULT_NOT_FOUND, "Xoa san bay khong dung");

    cout << "\nDanh sach san bay:\n";
    mgr.displayAirports();
    cout << "\nDanh sach chuyen bay (mac dinh theo gio khoi hanh):\n";
    mgr.displayFlights();

    // ===================== F3: Tra cuu ve =====================
    cout << "\n--- F3: Tra cuu ve theo ma dat cho ---\n";
    BookingHashTable bookings;
    bookings.insert(makeBooking("BK001", "PAX001", "VN101", "01A", "BOOKED", "2026-11-15 08:30"));
    bookings.insert(makeBooking("BK006", "PAX006", "VN102", "10D", "CANCELLED", "2026-11-15 10:15"));
    check(bookings.getSize() == 2 && !bookings.insert(makeBooking("BK001", "X", "Y", "1A", "BOOKED", "")),
          "BookingHashTable tu choi trung booking_id");
    check(lookupBooking(bookings, &mgr.getFlightTable(), "BK001"), "Tra cuu BK001 (co thong tin chuyen bay)");
    check(!lookupBooking(bookings, &mgr.getFlightTable(), "BK999"), "Tra cuu BK999 -> khong ton tai");
    check(lookupBooking(bookings, nullptr, "BK006"), "Tra cuu BK006 (da huy, khong co flights)");
    check(bookings.remove("BK001") && bookings.find("BK001") == nullptr, "Xoa booking");

    // ===================== F5 + 3.4.1: Sap xep =====================
    cout << "\n--- F5: Sap xep nhieu tieu chi ---\n";
    Flight arr[11];
    for (int i = 0; i < 11; i++) arr[i] = sample[i];

    sortFlightsByPrice(arr, 11, true);
    const char* byPrice[11] = {"QH602", "QH302", "VJ501", "QH601", "VN401", "VJ201",
                               "VJ202", "VJ502", "QH301", "VN102", "VN101"};
    check(orderMatches(arr, byPrice, 11), "Sort theo gia tang dan (on dinh: VJ501 truoc QH601)");

    for (int i = 0; i < 11; i++) arr[i] = sample[i];
    sortFlightsByDepartTime(arr, 11, true);
    const char* byTime[11] = {"VJ201", "VN101", "QH301", "VJ202", "QH302", "VN102",
                              "VN401", "QH601", "VJ501", "QH602", "VJ502"};
    check(orderMatches(arr, byTime, 11), "Sort theo gio khoi hanh tang dan");

    // Tieu chi 1: gia tang dan, tieu chi 2: gio khoi hanh tang dan (QH601 08:30 < VJ501 10:00)
    for (int i = 0; i < 11; i++) arr[i] = sample[i];
    SortCriterion multi[2] = { {SORT_BY_PRICE, true}, {SORT_BY_DEPARTURE_TIME, true} };
    sortFlightsByCriteria(arr, 11, multi, 2);
    const char* byMulti[11] = {"QH602", "QH302", "QH601", "VJ501", "VN401", "VJ201",
                               "VJ202", "VJ502", "QH301", "VN102", "VN101"};
    check(orderMatches(arr, byMulti, 11), "Sort gia tang dan, hoa gio khoi hanh tang dan");

    for (int i = 0; i < 11; i++) arr[i] = sample[i];
    SortCriterion multi2[2] = { {SORT_BY_PRICE, true}, {SORT_BY_DEPARTURE_TIME, false} };
    sortFlightsByCriteria(arr, 11, multi2, 2);
    check(strcmp(arr[2].flight_id, "VJ501") == 0 && strcmp(arr[3].flight_id, "QH601") == 0,
          "Sort gia tang dan, hoa gio khoi hanh giam dan");

    cout << "\nHien thi theo gia tang dan, roi gio khoi hanh tang dan:\n";
    mgr.displaySortedFlights(multi, 2);

    // ===================== Hieu nang voi dataset lon =====================
    cout << "\n--- Dataset lon ---\n";
    const int N = 200000;
    FlightHashTable big;
    bool allIns = true;
    for (int i = 0; i < N; i++) {
        char id[12], t[20];
        makeCode(id, "VN", i);
        makeTime(t, 1 + (int)(nextRandom() % 30), (int)(nextRandom() % 1440));
        allIns = big.insert(makeFlight(id, "RT001", "HAN", "SGN", t,
                            500000.0 + (double)(nextRandom() % 100) * 10000.0, 180, 180)) && allIns;
    }
    check(allIns && big.getSize() == N, "Insert 200000 chuyen bay (co rehash)");
    check(big.find("VN12345") != nullptr && big.remove("VN100") && big.find("VN100") == nullptr, "Find/Remove tren dataset lon");

    Flight* bigArr = new Flight[big.getSize()];
    int n = big.toArray(bigArr);
    check(n == N - 1, "toArray tra dung so phan tu");
    sortFlightsByPrice(bigArr, n, true);
    check(isSortedFlights(bigArr, n, compareByPriceAsc), "Sort 200000 theo gia tang dan");
    SortCriterion multi3[2] = { {SORT_BY_PRICE, true}, {SORT_BY_DEPARTURE_TIME, true} };
    sortFlightsByCriteria(bigArr, n, multi3, 2);
    bool okMulti = isSortedFlights(bigArr, n, compareByPriceAsc);
    for (int i = 1; i < n && okMulti; i++)
        if (bigArr[i].price == bigArr[i - 1].price && compareByDepartTimeAsc(bigArr[i], bigArr[i - 1])) okMulti = false;
    check(okMulti, "Sort nhieu tieu chi tren dataset lon");
    delete[] bigArr;

    cout << "\nKet qua: " << passed << " passed, " << failed << " failed\n";
    return failed == 0 ? 0 : 1;
}
