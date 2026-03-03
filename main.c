#include <stdio.h>    // Untuk printf, fprintf, fopen, fclose
#include <stdlib.h>   // Untuk atoi, system
#include <string.h>   // Untuk strcpy, strlen, strcspn
#include <ctype.h>    // Untuk isdigit, toupper

#define MAX_ITEM 5    // Menentukan jumlah mata pelajaran

// Struktur untuk menyimpan data tiap mata pelajaran
typedef struct {
    char nama[30];     // Nama mata pelajaran
    int kkm;           // Nilai KKM mata pelajaran
    int nilai;         // Nilai siswa
    char status[15];   // Status kelulusan (LULUS / TIDAK LULUS)
} Mapel;

// Fungsi untuk menghapus karakter newline '\n'
// dari input fgets agar string bersih
void trim(char str[]){
    str[strcspn(str, "\n")] = '\0';
}

// Fungsi untuk mengecek apakah nama valid
// Nama tidak boleh mengandung angka
// Return 1 = valid
// Return 0 = tidak valid
int validNama(char str[]){
    for(int i = 0; str[i] != '\0'; i++){
        if(isdigit(str[i])){
            return 0; // Jika ditemukan angka, langsung tidak valid
        }
    }
    return 1; // Jika tidak ada angka, maka valid
}

// Fungsi untuk mengecek apakah input hanya angka
// Return 1 = valid angka
// Return 0 = mengandung karakter selain angka
int validAngka(char str[]){
    for(int i = 0; str[i] != '\0'; i++){
        if(!isdigit(str[i])){
            return 0; // Jika ada karakter bukan angka
        }
    }
    return 1;
}

// Fungsi untuk menampilkan daftar mapel dan KKM
void tampilTabelAwal(Mapel m[]){
    printf("========================================================\n");
    printf("           DAFTAR MATA PELAJARAN DAN KKM\n");
    printf("========================================================\n");

    // Header tabel
    printf("%-4s %-22s %6s\n", "No", "Mata Pelajaran", "KKM");
    printf("--------------------------------------------------------\n");

    // Loop untuk menampilkan semua mapel
    for(int i = 0; i < MAX_ITEM; i++){
        printf("%-4d %-22s %6d\n",
               i+1,          // Nomor urut
               m[i].nama,    // Nama mapel ke-i
               m[i].kkm);    // KKM mapel ke-i
    }

    printf("========================================================\n");
}

// Fungsi untuk menghitung rata-rata nilai
float hitungRata(Mapel m[]){
    int total = 0;  // Variabel untuk menyimpan total nilai

    // Menjumlahkan seluruh nilai
    for(int i = 0; i < MAX_ITEM; i++){
        total += m[i].nilai;
    }

    // Mengembalikan hasil rata-rata
    return total / (float)MAX_ITEM;
}

// Fungsi untuk menentukan predikat berdasarkan rata-rata
char tentukanPredikat(float rata){
    if(rata >= 86){
        return 'A';
    }else if(rata >= 80){
        return 'B';
    }else if(rata >= 70){
        return 'C';
    }else{
        return 'D';
    }
}

// Fungsi untuk menyimpan hasil rapor ke file .txt
void cetakFile(Mapel m[],
               char nama[],
               char nis[],
               char kelas[],
               char sekolah[],
               float rata,
               char predikat){

    char namaFile[50]; // Variabel untuk menyimpan nama file

    // Membuat nama file berdasarkan NISN
    snprintf(namaFile, sizeof(namaFile), "rapor%s.txt", nis);

    // Membuka file dalam mode tulis
    FILE *fp = fopen(namaFile, "w");

    // Jika file gagal dibuat
    if(fp == NULL){
        printf("Gagal membuat file!\n");
        return;
    }

    // Menulis header rapor ke file
    fprintf(fp,"========================================================\n");
    fprintf(fp,"             RAPOR SMK NEGERI 1 DENPASAR\n");
    fprintf(fp,"========================================================\n");

    // Menulis identitas siswa
    fprintf(fp,"Nama Siswa   : %s\n",nama);
    fprintf(fp,"NISN         : %s\n",nis);
    fprintf(fp,"Kelas        : %s\n",kelas);
    fprintf(fp,"Nama Sekolah : %s\n",sekolah);

    // Menulis tabel nilai
    fprintf(fp,"========================================================\n");
    fprintf(fp,"%-4s %-22s %-6s %-7s %-12s\n",
            "No","Mapel","KKM","Nilai","Status");
    fprintf(fp,"--------------------------------------------------------\n");

    // Loop untuk menulis setiap mapel ke file
    for(int i = 0; i < MAX_ITEM; i++){
        fprintf(fp,"%-4d %-22s %-6d %-7d %-12s\n",
                i+1,
                m[i].nama,
                m[i].kkm,
                m[i].nilai,
                m[i].status);
    }

    // Menulis rata-rata dan predikat
    fprintf(fp,"========================================================\n");
    fprintf(fp,"Rata-rata Nilai : %.2f\n",rata);
    fprintf(fp,"Predikat        : %c\n",predikat);
    fprintf(fp,"========================================================\n");

    fclose(fp); // Menutup file

    printf("Rapor berhasil disimpan ke file '%s'\n",namaFile);
}

// ==================== MAIN PROGRAM ====================

int main(){

    // Inisialisasi data mata pelajaran beserta KKM
    Mapel data[MAX_ITEM] = {
        {"Pemrograman Web",76,0,"-"},
        {"Pemrograman Mobile",76,0,"-"},
        {"Basis Data",76,0,"-"},
        {"PBO",76,0,"-"},
        {"Kewirausahaan",79,0,"-"}
    };

    tampilTabelAwal(data); // Menampilkan tabel awal

    // Variabel identitas siswa
    char nama[50], nis[20], kelas[20], sekolah[50];
    char temp[50];         // Variabel sementara untuk input angka
    char konfirmasi;       // Untuk menyimpan pilihan Y/N
    float rata;            // Untuk menyimpan rata-rata
    char predikat;         // Untuk menyimpan predikat

    // ================= INPUT IDENTITAS =================

    // Input nama siswa dengan validasi
    do{
        printf("\n=== INPUT IDENTITAS SISWA ===\n");
        printf("Nama Siswa     : ");
        fgets(nama, sizeof(nama), stdin);
        trim(nama);

        if(!validNama(nama)){
            printf("Nama tidak boleh mengandung angka!\n");
        }

    }while(!validNama(nama));

    // Input NISN dengan validasi angka
    do{
        printf("NISN           : ");
        fgets(temp, sizeof(temp), stdin);
        trim(temp);

        if(!validAngka(temp)){
            printf("NISN harus berupa angka!\n");
        }

    }while(!validAngka(temp));

    strcpy(nis, temp); // Menyimpan NISN yang valid

    // Input kelas
    printf("Kelas          : ");
    fgets(kelas, sizeof(kelas), stdin);
    trim(kelas);

    // Input nama sekolah
    printf("Nama Sekolah   : ");
    fgets(sekolah, sizeof(sekolah), stdin);
    trim(sekolah);

    // ================= INPUT NILAI =================

    while(1){

        printf("\n=== INPUT NILAI MATA PELAJARAN ===\n");

        // Loop input nilai tiap mapel
        for(int i = 0; i < MAX_ITEM; i++){

            do{
                printf("Nilai %-22s : ", data[i].nama);
                fgets(temp, sizeof(temp), stdin);
                trim(temp);

                if(!validAngka(temp)){
                    printf("Nilai harus angka!\n");
                    continue;
                }

                data[i].nilai = atoi(temp);

                if(data[i].nilai < 0 || data[i].nilai > 100){
                    printf("Nilai harus antara 0 - 100!\n");
                }

            }while(!validAngka(temp) ||
                   data[i].nilai < 0 ||
                   data[i].nilai > 100);

            // Menentukan status kelulusan
            if(data[i].nilai >= data[i].kkm)
                strcpy(data[i].status,"LULUS");
            else
                strcpy(data[i].status,"TIDAK LULUS");
        }

        system("cls"); // Membersihkan layar

        // Menghitung rata-rata dan predikat
        rata = hitungRata(data);
        predikat = tentukanPredikat(rata);

        // Menampilkan hasil
        printf("========================================================\n");
        printf("              RAPOR SMK NEGERI 1 DENPASAR\n");
        printf("========================================================\n");
        printf("Nama Siswa   : %s\n",nama);
        printf("NISN         : %s\n",nis);
        printf("Kelas        : %s\n",kelas);
        printf("Nama Sekolah : %s\n",sekolah);

        printf("========================================================\n");
        printf("%-4s %-22s %-6s %-7s %-12s\n",
               "No","Mapel","KKM","Nilai","Status");
        printf("--------------------------------------------------------\n");

        for(int i = 0; i < MAX_ITEM; i++){
            printf("%-4d %-22s %-6d %-7d %-12s\n",
                   i+1, data[i].nama, data[i].kkm, data[i].nilai, data[i].status);
        }

        printf("========================================================\n");
        printf("Rata-rata Nilai : %.2f\n",rata);
        printf("Predikat        : %c\n",predikat);
        printf("========================================================\n");

        // Konfirmasi apakah ingin mencetak
        do{
            printf("\nApakah data sudah benar?\n");
            printf("Tekan Y untuk cetak rapor\n");
            printf("Tekan N untuk input ulang nilai\n");
            printf("Pilihan Anda : ");
            fgets(temp, sizeof(temp), stdin);
            konfirmasi = toupper(temp[0]);
        }while(konfirmasi != 'Y' && konfirmasi != 'N');

        if(konfirmasi == 'Y'){
            cetakFile(data,nama,nis,kelas,sekolah,rata,predikat);
            break;
        }

        system("cls");
    }

    return 0; // Program selesai
}
