#include <iostream>
#include <fstream>
#include <string>
#include <array>
#include <exception>
#include <stdexcept>
#include <vector>

using namespace std;

// 1. Struktur Kelas TokoElektronik
class TokoElektronik {
private:
    // Atribut private berupa array kapasitas 3 (Enkapsulasi)
    array<string, 3> etalase;

public:
    // Constructor untuk mengisi data awal secara otomatis
    TokoElektronik() {
        etalase = {"Laptop", "Smartphone", "Smart TV"};
    }

    // Method public untuk mengambil produk
    // 2. Mekanisme Exception Handling
    string ambilBarang(size_t nomorRak) {
        try {
            // Mengakses indeks menggunakan .at()
            return etalase.at(nomorRak);
        }
        catch (const out_of_range& e) {
            // Menangkap error bawaan dan melempar kembali (throw) error kustom
            string pesanError = "Gagal Mengambil Barang : Rak nomor " + to_string(nomorRak) + " kosong atau tidak tersedia!";
            throw runtime_error(pesanError);
        }
    }
};

// =========================================================
// 3. Fitur Manajemen Gudang Berbasis File (File I/O & CRUD)
// =========================================================

const string NAMA_FILE = "gudang.txt";

// Fungsi READ: Menampilkan daftar barang (dipanggil otomatis)
void tampilkanBarang() {
    ifstream infile(NAMA_FILE);
    string baris;
    int nomor = 1;

    cout << "\n=== Daftar Barang di Gudang ===" << endl;
    if (infile.is_open()) {
        while (getline(infile, baris)) {
            cout << nomor << ". " << baris << endl;
            nomor++;
        }
        infile.close();
        if (nomor == 1) {
            cout << "(Gudang saat ini kosong)" << endl;
        }
    } else {
        cout << "(File gudang belum ada, silakan tambahkan barang terlebih dahulu)" << endl;
    }
    cout << "===============================" << endl;
}

// Fungsi CREATE: Menambah data barang baru
void tambahBarang() {
    ofstream outfile(NAMA_FILE, ios::app);
    string barangBaru;
    
    cout << "Masukkan nama barang baru: ";
    getline(cin >> ws, barangBaru); // ws untuk membersihkan whitespace
    
    outfile << barangBaru << endl;
    outfile.close();
    cout << "=> Barang berhasil ditambahkan ke gudang!" << endl;
}

// Fungsi UPDATE: Memperbarui data barang tertentu
void updateBarang() {
    vector<string> daftarBarang;
    ifstream infile(NAMA_FILE);
    string baris;
    
    // Membaca semua data ke dalam vector
    while (getline(infile, baris)) {
        daftarBarang.push_back(baris);
    }
    infile.close();

    if (daftarBarang.empty()) {
        cout << "=> Gudang kosong, tidak ada yang bisa di-update." << endl;
        return;
    }

    int nomor;
    cout << "Pilih nomor barang yang ingin diubah (1-" << daftarBarang.size() << "): ";
    cin >> nomor;

    if (nomor > 0 && nomor <= daftarBarang.size()) {
        string namaBaru;
        cout << "Masukkan nama barang pengganti: ";
        getline(cin >> ws, namaBaru);
        
        daftarBarang[nomor - 1] = namaBaru; // Modifikasi data pada vector

        // Menulis ulang isi vector ke dalam file (overwrite)
        ofstream outfile(NAMA_FILE, ios::out);
        for (const auto& barang : daftarBarang) {
            outfile << barang << endl;
        }
        outfile.close();
        cout << "=> Barang berhasil diperbarui!" << endl;
    } else {
        cout << "=> Nomor barang tidak valid!" << endl;
    }
}

// Fungsi DELETE: Menghapus data barang tertentu
void hapusBarang() {
    vector<string> daftarBarang;
    ifstream infile(NAMA_FILE);
    string baris;
    
    // Membaca semua data ke dalam vector
    while (getline(infile, baris)) {
        daftarBarang.push_back(baris);
    }
    infile.close();

    if (daftarBarang.empty()) {
        cout << "=> Gudang kosong, tidak ada yang bisa dihapus." << endl;
        return;
    }

    int nomor;
    cout << "Pilih nomor barang yang ingin dihapus (1-" << daftarBarang.size() << "): ";
    cin >> nomor;

    if (nomor > 0 && nomor <= daftarBarang.size()) {
        // Menghapus elemen dari vector
        daftarBarang.erase(daftarBarang.begin() + (nomor - 1));

        // Menulis ulang isi vector yang baru ke dalam file (overwrite)
        ofstream outfile(NAMA_FILE, ios::out);
        for (const auto& barang : daftarBarang) {
            outfile << barang << endl;
        }
        outfile.close();
        cout << "=> Barang berhasil dihapus!" << endl;
    } else {
        cout << "=> Nomor barang tidak valid!" << endl;
    }
}

// =========================================================
// 4. Pengujian pada Fungsi main()
// =========================================================

int main() {
    TokoElektronik tokoGibran;
    int pilihan;

    do {
        // Otomatis membaca dan menampilkan daftar barang saat menu dibuka
        tampilkanBarang();
        
        cout << "=== Menu Utama Toko Gibran Jaya ===" << endl;
        cout << "1. Tambah Barang Gudang (Create)" << endl;
        cout << "2. Update Barang Gudang (Update)" << endl;
        cout << "3. Hapus Barang Gudang  (Delete)" << endl;
        cout << "4. Simulasi Etalase (Exception Test)" << endl;
        cout << "0. Keluar" << endl;
        cout << "Pilih menu (0-4): ";
        
        // Cek input agar tidak error jika user memasukkan huruf
        if (!(cin >> pilihan)) {
            cin.clear();
            cin.ignore(10000, '\n');
            pilihan = -1; 
        }

        switch (pilihan) {
            case 1:
                tambahBarang();
                break;
            case 2:
                updateBarang();
                break;
            case 3:
                hapusBarang();
                break;
            case 4:
                cout << "\n--- Menjalankan Simulasi Etalase ---" << endl;
                try {
                    // Skenario 1: Indeks ke-1 (Harus sukses)
                    cout << "Skenario 1 - Mencoba mengambil rak indeks [1]: ";
                    cout << "Berhasil mengambil " << tokoGibran.ambilBarang(1) << endl;

                    // Skenario 2: Indeks ke-5 (Harus gagal dan melempar error kustom)
                    cout << "Skenario 2 - Mencoba mengambil rak indeks [5]: ";
                    cout << tokoGibran.ambilBarang(5) << endl;
                }
                catch (const exception& e) {
                    // Menangkap runtime_error yang dilempar dari method ambilBarang()
                    cout << "\n[ERROR TERTANGKAP] " << e.what() << endl;
                }
                cout << "------------------------------------" << endl;
                break;
            case 0:
                cout << "Terima kasih telah menggunakan sistem Toko Gibran Jaya!" << endl;
                break;
            default:
                cout << "Pilihan tidak valid. Silakan coba lagi." << endl;
                break;
        }
    } while (pilihan != 0);

    return 0;
}