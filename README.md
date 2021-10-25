# Viktori PSJ IOT Server

PSJ kok iot?

***Hanya bisa di run di UNIX System, windows gabisa***
> Bisa tapi ribet nanti, skip lah ya

## Yang perlu diinstall supaya bisa ngerun

- gcc (Pasti ada sih klo install linux)
- make (untuk build)
- cmake (untuk memudahkan build)
- vscode (*opsional, ada ekstensi yang memudahkan ngebuild binarynya)

> make sama cmake itu opsional **KALAU** bisa compile manual pake gcc dan segala flagsnya

## Cara ngerun

### Compile/build dulu lah

Pindah ke direktori dimana `CMakeLists.txt` berada, trus

```sh
cmake --build ./build
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
- [ ] Mengformat text file yang ditulis or smth
- [ ] Support multithreading

Todos lanjutan menyesuaikan dengan client (masih no clue).
