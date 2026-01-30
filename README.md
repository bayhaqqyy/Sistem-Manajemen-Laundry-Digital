# LaundryExpress Pro

LaundryExpress Pro adalah aplikasi CLI berbasis C untuk manajemen laundry digital. Program ini mengimplementasikan algoritma "Smart Laundry Management" sesuai kebutuhan tugas: input order tervalidasi, kalkulator harga cerdas, tracking status berbasis array 2D, optimasi urutan pencucian, sistem pencarian, laporan bisnis harian, dan estimasi waktu rekursif.

## Fitur Utama

- Input order dengan validasi nama, jenis pakaian, berat, layanan, dan prioritas.
- Kalkulator harga: harga dasar per kg, multiplier jenis pakaian dan layanan, serta diskon otomatis.
- Tracking status order dan timeline menggunakan array 2D.
- Optimasi urutan pencucian berdasarkan prioritas + efisiensi mesin.
- Pencarian order (nama, status, rentang ID).
- Laporan bisnis harian (pendapatan, rata-rata berat, layanan populer, pelanggan terbanyak).
- Prediksi waktu selesai dengan fungsi rekursif.

## Struktur Data

- `orders[MAX_ORDERS]` menyimpan data order (struct).
- `status_timeline[MAX_ORDERS][STATUS_COUNT]` menyimpan progress status per order.

## Cara Menjalankan

### Windows (MinGW / GCC)

```bash
g++ main.cpp -o main.exe
.\main.exe
```

### Linux / macOS

```bash
g++ main.cpp -o main
./main
```

## Menu Utama

1. Tambah Order Baru
2. Proses Order (Update Status)
3. Cari & Lihat Detail Order
4. Hitung Estimasi Waktu & Biaya
5. Generate Laporan Harian
6. Optimasi Urutan Pencucian
7. Dashboard Status Order
8. Reset Data Harian
9. Keluar

## Aturan Perhitungan Harga

- Harga dasar: Rp 5.000/kg
- Multiplier jenis pakaian:
  - Baju: 1.0
  - Celana: 1.2
  - Jaket: 1.5
  - Selimut: 2.0
  - Lainnya: 1.3
- Multiplier layanan:
  - Express: 2.0 (3 jam)
  - Fast: 1.5 (6 jam)
  - Normal: 1.0 (24 jam)
- Diskon:
  - Berat > 10 kg: 10%
  - Order ke-3 (atau lebih) dari pelanggan yang sama: 15%

## Catatan

- Aplikasi ini berbasis CLI dan tidak menyimpan data secara permanen (data hilang saat program ditutup).
- Reset Data Harian menghapus semua order pada sesi berjalan.
