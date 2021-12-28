# Viktori PSJ Server

## Index link

[Link untuk referensi](https://lordronz.github.io/viktori-psj-server)

## Deskripsi

Repo ini adalah sisi server dari projek FP pemrograman sistem dan jaringan.

Untuk client akses di [link ini](https://github.com/lordronz/viktori-psj-client)

Dan untuk program yang ditempatkan di arduino silahkan akses di [link ini](https://github.com/lordronz/viktori-psj-arduino)

## Kelompok

- Aaron Christopher 07211940000055
- I Putu Haris Setiadi 07211940000046
- I Gusti Agung Hari 07211940000073

***Hanya bisa di run di UNIX System, windows gabisa***
> Bisa tapi ribet nanti

## Fitur

- Komunikasi dengan client menggunakan TCP Socket
- Multithreading support
- Support fork (child process memiliki threadnya masing-masing juga)
- Validasi input/request dari client
- Menulis request di txt dengan menggunakan stdio
- Implementasi command `tail` menggunakan UNIX I/O untuk mendapatkan `n` baris terakhir pada txt
- Menggunakan signal handler untuk mematikan server, dan membersihkan semua resources yang ada.

## Yang perlu diinstall supaya bisa ngerun

- gcc (Pasti ada sih klo install linux)
- make (untuk build)
- cmake (untuk memudahkan build)
- vscode (*opsional, ada ekstensi yang memudahkan ngebuild binarynya)

> make sama cmake itu opsional **KALAU** bisa compile manual pake gcc dan segala flagsnya

## Cara ngerun

### Compile cara pertama

Pindah ke direktori dimana `CMakeLists.txt` berada, trus

```sh
cmake --build ./build
```

### Compile cara kedua

Eksekusi command build.sh

```sh
./build.sh
```

Atau

```sh
bash build.sh
```

### Ngerun

Ini gapake argument sih, buat output cara pemakaian aja

```sh
./build/Server
```

Ini kalo IPnya 0.0.0.0 (listen anywhere)

```sh
./build/Server 6969
```

Ini ngerun di local area network, cek ip pake command `ifconfig`

```sh
./build/Server 6969 192.168.1.5
```

## TODOs

- [X] Write buffer yang diterima ke text file
- [X] Mengformat text file yang ditulis or smth
- [X] Support multithreading
- [ ] Pakai scripting language e.g Python untuk mempermudah ngerun dan nampilin help description

Todos lanjutan menyesuaikan dengan client.
