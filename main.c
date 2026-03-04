#include <stdio.h>    // Digunakan untuk fungsi input output seperti printf, fprintf, fopen
#include <stdlib.h>   // Digunakan untuk fungsi umum seperti atoi dan system
#include <string.h>   // Digunakan untuk manipulasi string seperti strcpy dan strcspn
#include <ctype.h>    // Digunakan untuk pengecekan karakter seperti isdigit dan toupper

#define MAX_ITEM 5    // Menentukan jumlah mata pelajaran yang digunakan dalam program


// Struct Mapel digunakan untuk menyimpan data setiap mata pelajaran
typedef struct {
    char nama[30];     // Nama mata pelajaran
    int kkm;           // Nilai KKM (batas minimal kelulusan)
    int nilai;         // Nilai siswa yang diinput
    char status[15];   // Status kelulusan (LULUS / TIDAK LULUS)
} Mapel;


// Fungsi trim digunakan untuk menghapus karakter newline '\n'
// yang muncul dari input fgets
void trim(char str[]){
    str[strcspn(str, "\n")] = '\0';
}


// Fungsi untuk mengecek apakah nama valid
// Nama dianggap tidak valid jika mengandung angka
int validNama(char str[]){
    for(int i = 0; str[i] != '\0'; i++){
        if(isdigit(str[i])){   // Jika ditemukan angka
            return 0;          // Nama tidak valid
        }
    }
    return 1;                  // Nama valid
}


// Fungsi untuk mengecek apakah input hanya berisi angka
// Digunakan untuk validasi NISN dan nilai
int validAngka(char str[]){
    for(int i = 0; str[i] != '\0'; i++){
        if(!isdigit(str[i])){  // Jika ada karakter selain angka
            return 0;
        }
    }
    return 1;
}


// Fungsi untuk menampilkan daftar mata pelajaran dan KKM
// Tujuannya agar pengguna mengetahui standar nilai minimum
void tampilTabelAwal(Mapel m[]){

    printf("========================================================\n");
    printf("           DAFTAR MATA PELAJARAN DAN KKM\n");
    printf("========================================================\n");

    printf("%-4s %-22s %6s\n", "No", "Mata Pelajaran", "KKM"); // Header tabel
    printf("--------------------------------------------------------\n");

    for(int i = 0; i < MAX_ITEM; i++){  // Loop untuk menampilkan semua mapel
        printf("%-4d %-22s %6d\n",
               i+1,        // Nomor urut
               m[i].nama,  // Nama mata pelajaran
               m[i].kkm);  // Nilai KKM
    }

    printf("========================================================\n");
}


// Fungsi untuk menghitung rata-rata nilai dari semua mata pelajaran
float hitungRata(Mapel m[]){

    int total = 0;   // Variabel untuk menyimpan jumlah seluruh nilai

    for(int i = 0; i < MAX_ITEM; i++){
        total += m[i].nilai;   // Menambahkan nilai tiap mapel
    }

    return total / (float)MAX_ITEM;  // Mengembalikan hasil rata-rata
}


// Fungsi untuk menentukan predikat berdasarkan rata-rata nilai
char tentukanPredikat(float rata){

    if(rata >= 86){
        return 'A';   // Predikat sangat baik
    }
    else if(rata >= 80){
        return 'B';   // Predikat baik
    }
    else if(rata >= 70){
        return 'C';   // Predikat cukup
    }
    else{
        return 'D';   // Predikat kurang
    }
}


// Fungsi untuk menyimpan hasil rapor ke dalam file txt
void cetakFile(Mapel m[],
               char nama[],
               char nis[],
               char kelas[],
               char sekolah[],
               float rata,
               char predikat){

    char namaFile[50];   // Variabel untuk menyimpan nama file

    snprintf(namaFile, sizeof(namaFile), "rapor%s.txt", nis); // Membuat nama file berdasarkan NISN

    FILE *fp = fopen(namaFile, "w");   // Membuka file dalam mode tulis

    if(fp == NULL){                    // Jika file gagal dibuat
        printf("Gagal membuat file!\n");
        return;
    }

    // Menulis header rapor ke file
    fprintf(fp,"========================================================\n");
    fprintf(fp,"             RAPOR SMK NEGERI 1 DENPASAR\n");
    fprintf(fp,"========================================================\n");

    // Menulis identitas siswa
    fprintf(fp,"Nama Siswa   : %s\n", nama);
    fprintf(fp,"NISN         : %s\n", nis);
    fprintf(fp,"Kelas        : %s\n", kelas);
    fprintf(fp,"Nama Sekolah : %s\n", sekolah);

    // Menulis tabel nilai
    fprintf(fp,"========================================================\n");
    fprintf(fp,"%-4s %-22s %-6s %-7s %-12s\n",
            "No","Mapel","KKM","Nilai","Status");

    fprintf(fp,"--------------------------------------------------------\n");

    for(int i = 0; i < MAX_ITEM; i++){   // Menulis data setiap mapel
        fprintf(fp,"%-4d %-22s %-6d %-7d %-12s\n",
                i+1,
                m[i].nama,
                m[i].kkm,
                m[i].nilai,
                m[i].status);
    }

    // Menulis hasil akhir
    fprintf(fp,"========================================================\n");
    fprintf(fp,"Rata-rata Nilai : %.2f\n", rata);
    fprintf(fp,"Predikat        : %c\n", predikat);
    fprintf(fp,"========================================================\n");

    fclose(fp);   // Menutup file

    printf("Rapor berhasil disimpan ke file '%s'\n", namaFile);
}



// ==================== PROGRAM UTAMA ====================

int main(){

    // Inisialisasi data mata pelajaran dan nilai KKM
    Mapel data[MAX_ITEM] = {
        {"Pemrograman Web",76,0,"-"},
        {"Pemrograman Mobile",76,0,"-"},
        {"Basis Data",76,0,"-"},
        {"PBO",76,0,"-"},
        {"Kewirausahaan",79,0,"-"}
    };

    tampilTabelAwal(data);  // Menampilkan tabel mapel dan KKM


    // Variabel untuk menyimpan identitas siswa
    char nama[50], nis[20], kelas[20], sekolah[50];
    char temp[50];      // Variabel sementara untuk input
    char konfirmasi;    // Menyimpan pilihan Y / N
    float rata;         // Menyimpan nilai rata-rata
    char predikat;      // Menyimpan predikat nilai


    // ===== INPUT IDENTITAS SISWA =====

    do{
        printf("\n=== INPUT IDENTITAS SISWA ===\n");

        printf("Nama Siswa     : ");
        fgets(nama, sizeof(nama), stdin);
        trim(nama);

        if(!validNama(nama)){
            printf("Nama tidak boleh mengandung angka!\n");
        }

    }while(!validNama(nama));


    do{
        printf("NISN           : ");
        fgets(temp, sizeof(temp), stdin);
        trim(temp);

        if(!validAngka(temp)){
            printf("NISN harus berupa angka!\n");
        }

    }while(!validAngka(temp));

    strcpy(nis, temp);   // Menyimpan NISN yang valid


    printf("Kelas          : ");
    fgets(kelas, sizeof(kelas), stdin);
    trim(kelas);

    printf("Nama Sekolah   : ");
    fgets(sekolah, sizeof(sekolah), stdin);
    trim(sekolah);


    // ===== INPUT NILAI MATA PELAJARAN =====

    while(1){

        printf("\n=== INPUT NILAI MATA PELAJARAN ===\n");

        for(int i = 0; i < MAX_ITEM; i++){

            do{
                printf("Nilai %-22s : ", data[i].nama);

                fgets(temp, sizeof(temp), stdin);
                trim(temp);

                if(!validAngka(temp)){
                    printf("Nilai harus angka!\n");
                    continue;
                }

                data[i].nilai = atoi(temp);   // Mengubah string menjadi integer

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


        system("cls");   // Membersihkan layar


        // Menghitung rata-rata dan predikat
        rata = hitungRata(data);
        predikat = tentukanPredikat(rata);


        // Menampilkan rapor di layar
        printf("========================================================\n");
        printf("              RAPOR SMK NEGERI 1 DENPASAR\n");
        printf("========================================================\n");

        printf("Nama Siswa   : %s\n", nama);
        printf("NISN         : %s\n", nis);
        printf("Kelas        : %s\n", kelas);
        printf("Nama Sekolah : %s\n", sekolah);

        printf("========================================================\n");
        printf("%-4s %-22s %-6s %-7s %-12s\n",
               "No","Mapel","KKM","Nilai","Status");

        printf("--------------------------------------------------------\n");

        for(int i = 0; i < MAX_ITEM; i++){
            printf("%-4d %-22s %-6d %-7d %-12s\n",
                   i+1,
                   data[i].nama,
                   data[i].kkm,
                   data[i].nilai,
                   data[i].status);
        }

        printf("========================================================\n");
        printf("Rata-rata Nilai : %.2f\n", rata);
        printf("Predikat        : %c\n", predikat);
        printf("========================================================\n");


        // Konfirmasi apakah data sudah benar
        do{
            printf("\nApakah data sudah benar?\n");
            printf("Tekan Y untuk cetak rapor\n");
            printf("Tekan N untuk input ulang nilai\n");

            printf("Pilihan Anda : ");

            fgets(temp, sizeof(temp), stdin);
            konfirmasi = toupper(temp[0]);  // Mengubah input menjadi huruf besar

        }while(konfirmasi != 'Y' && konfirmasi != 'N');


        if(konfirmasi == 'Y'){
            cetakFile(data,nama,nis,kelas,sekolah,rata,predikat); // Menyimpan rapor ke file
            break;
        }

        system("cls");   // Jika memilih N maka input nilai diulang
    }

    return 0;   // Program selesai
}
