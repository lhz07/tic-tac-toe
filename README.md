# Tic-Tac-Toe

## Build

### Dependencies

- cmake
- qt6 (only tested on Qt 6.9.0)

### Install Dependencies

#### Arch

```sh
pacman -S cmake qt6-base
```

#### Debian / Ubuntu

```sh
apt install cmake qt6-base-dev
```

### MacOS

```sh
brew install cmake qt
```

### Build and Run

```sh
git clone https://github.com/lhz07/tic-tac-toe.git
cd tic-tac-toe
mkdir build
cd build
cmake ..
cmake --build . --parallel
./tic-tac-toe
```

