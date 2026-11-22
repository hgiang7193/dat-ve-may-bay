# hash_sort_quan - Hash Table & Merge Sort

Phần việc của **Quân** trong đồ án *Hệ thống đặt vé máy bay/tàu hỏa* (C++, không dùng STL).

| Mục | Chức năng | Cấu trúc dữ liệu |
|-----|-----------|------------------|
| 3.1 | Quản lý chuyến bay & tra cứu vé theo mã | Hash Table (chaining) |
| 3.4.1 | Sắp xếp chuyến bay ổn định theo giá / giờ khởi hành | Merge Sort |
| F1 | Thêm, sửa, xóa, hiển thị chuyến bay và sân bay | Hash Table |
| F3 | Tra cứu vé theo mã đặt chỗ | Hash Table |
| F5 | Sắp xếp và hiển thị chuyến bay theo nhiều tiêu chí liên tiếp | Merge Sort ổn định |

## Cấu trúc file

```
hash_sort_quan/
├── HashTable.h / HashTable.cpp       # Hash Table (template) + F3 tra cứu vé
├── MergeSort.h / MergeSort.cpp       # Merge Sort ổn định + F5 đa tiêu chí
├── FlightManager.h / FlightManager.cpp  # F1 quản lý chuyến bay/sân bay + hiển thị
├── TestHashSort.cpp                  # Chương trình test độc lập
└── README.md
```

Các file dùng struct chung của nhóm trong `common/Models.h` (`Airport`, `Flight`, `Booking`). File này nằm ở folder `common/` cùng cấp với `hash_sort_quan/`, và được include bằng `#include "../common/Models.h"` nên **không cần** thêm cờ `-I` khi biên dịch.

```
repo/
├── common/
│   └── Models.h
└── hash_sort_quan/
```

## Yêu cầu

- Trình biên dịch hỗ trợ C++11 (g++ / MinGW).
- Chỉ dùng `<iostream>`, `<cstring>` (không dùng STL container, `std::sort`...).

## Biên dịch và chạy test

Mở terminal tại folder `hash_sort_quan` và chạy (đủ 4 file `.cpp`):

```bash
g++ -std=c++11 -Wall TestHashSort.cpp FlightManager.cpp HashTable.cpp MergeSort.cpp -o test_hash_sort
```

- Chạy:
  - Windows (PowerShell/CMD): `.\test_hash_sort.exe`
  - Linux / Mac / Git Bash: `./test_hash_sort`

Kết quả đúng kết thúc bằng dòng `Ket qua: 30 passed, 0 failed`.

> Lỗi `undefined reference to getFlightKey...` nghĩa là thiếu file `.cpp` khi biên dịch. Lỗi `../common/Models.h: No such file` nghĩa là folder `common/` không nằm cùng cấp với `hash_sort_quan/` (hoặc đang đứng sai thư mục khi chạy lệnh).

## Cách dùng

### F1 - Quản lý chuyến bay và sân bay

```cpp
#include "FlightManager.h"

FlightManager mgr;

Airport hanoi;
strcpy(hanoi.airport_id, "HAN");
strcpy(hanoi.name, "Noi Bai");
strcpy(hanoi.city, "Ha Noi");
mgr.addAirport(hanoi);                    // RESULT_OK

Flight f;
strcpy(f.flight_id, "VN101");
strcpy(f.route_id, "RT001");
strcpy(f.origin, "HAN");
strcpy(f.destination, "SGN");
strcpy(f.departure_time, "2026-11-20 08:00");
f.price = 1550000; f.total_seats = 180; f.available_seats = 120;

ResultCode code = mgr.addFlight(f);
std::cout << getResultMessage(code) << "\n";

mgr.updateFlight(f);                      // sửa theo flight_id
mgr.removeFlight("VN101");                // xóa
mgr.displayFlights();                     // in, mặc định theo giờ khởi hành
mgr.displayAirports();
```

Mã kết quả (`ResultCode`):

| Mã | Ý nghĩa |
|----|---------|
| `RESULT_OK` | Thành công |
| `RESULT_INVALID_DATA` | Vi phạm ràng buộc (ghế, giá âm, sai định dạng giờ, origin = destination...) |
| `RESULT_DUPLICATE` | Trùng khóa chính |
| `RESULT_NOT_FOUND` | Không tồn tại |
| `RESULT_AIRPORT_NOT_FOUND` | Sân bay đi/đến chưa có trong hệ thống |
| `RESULT_IN_USE` | Sân bay còn chuyến bay đang dùng, không xóa được |

### F3 - Tra cứu vé theo mã đặt chỗ

```cpp
BookingHashTable bookings;
bookings.insert(booking);

// In thông tin vé (kèm hành trình nếu truyền bảng chuyến bay)
bool found = lookupBooking(bookings, &mgr.getFlightTable(), "BK001");
```

### F5 - Sắp xếp theo nhiều tiêu chí

`criteria[0]` ưu tiên cao nhất, các tiêu chí sau dùng để phân định khi bằng nhau.

```cpp
SortCriterion criteria[2] = {
    { SORT_BY_PRICE, true },             // giá tăng dần
    { SORT_BY_DEPARTURE_TIME, true }     // rồi giờ khởi hành tăng dần
};
mgr.displaySortedFlights(criteria, 2);

// Hoặc sắp xếp trực tiếp trên mảng:
Flight* arr = new Flight[mgr.getFlightTable().getSize()];
int n = mgr.getFlightTable().toArray(arr);
sortFlightsByCriteria(arr, n, criteria, 2);
delete[] arr;
```

Các hàm sắp xếp đơn: `sortFlightsByPrice(arr, n, ascending)`, `sortFlightsByDepartTime(arr, n, ascending)`.

## Dành cho các module khác

- Bảng băm dùng chung: lấy qua `mgr.getFlightTable()` và `mgr.getAirportTable()`.
- Trừ ghế, hoàn ghế (F2, F4, F7): `Flight* f = mgr.findFlight("VN101"); f->available_seats--;`
- Lưu file (F8): dùng `toArray(...)` của từng bảng để lấy toàn bộ phần tử ghi ra file. Khi nạp, gọi `insert(...)` / `addFlight(...)`.
- Kiểu bảng băm: `AirportHashTable`, `FlightHashTable`, `BookingHashTable`; khóa lần lượt là `airport_id`, `flight_id`, `booking_id`.

## Độ phức tạp

n = số phần tử, m = số bucket, k = số tiêu chí sắp xếp.

| Thao tác | Time | Space |
|----------|------|-------|
| `insert`, `find`, `remove` (Hash Table) | O(1) trung bình, O(n) xấu nhất | O(1) |
| `rehash` (khi load factor > 0.75, gấp đôi bucket) | O(n + m) | O(m) |
| `toArray`, `clear` | O(n + m) | O(1) |
| Merge Sort (`mergeSortFlights`) | O(n log n) mọi trường hợp | O(n) |
| Sắp xếp k tiêu chí (`sortFlightsByCriteria`) | O(k · n log n) | O(n) |
| `lookupBooking` (F3) | O(1) trung bình | O(1) |
| `removeAirport` (kiểm tra chuyến bay đang dùng) | O(n + m) | O(n) |

## Ghi chú thiết kế

- **Chaining + rehash:** mỗi bucket là danh sách liên kết; tự động gấp đôi số bucket khi load factor vượt 0.75. Hàm băm: djb2.
- **Merge Sort ổn định:** chỉ lấy phần tử nửa phải khi nó nhỏ hơn hẳn nửa trái. Nhờ vậy sắp nhiều tiêu chí được bằng cách chạy lần lượt từ tiêu chí cuối lên tiêu chí đầu.
- **So sánh giờ khởi hành bằng `strcmp`:** định dạng `YYYY-MM-DD HH:MM` có thứ tự chữ cái trùng thứ tự thời gian. Vì vậy `FlightManager` kiểm tra định dạng giờ khi thêm/sửa chuyến bay.
- **`route_id`:** F1 chỉ kiểm tra không rỗng, chưa kiểm tra tồn tại trong bảng `routes` (thuộc module đồ thị/Dijkstra).
- **Ràng buộc (flight_id, seat) duy nhất:** thuộc về F2 (đặt vé); `BookingHashTable` chỉ đảm bảo `booking_id` duy nhất.

## Code convention

- Mọi file `.h` có header guard (`#ifndef` / `#define` / `#endif`).
- Mỗi function chính đều comment độ phức tạp `Time` / `Space`.
- File đặt tên PascalCase (`FlightManager.cpp`), hàm và biến dùng camelCase (`findFlight()`). Tên trường của struct giữ theo `Models.h` (snake_case).
- Không dùng STL; không sửa code trong folder của người khác khi chưa bàn bạc.
