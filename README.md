# solarpass
Small CLI password generator written in C++

## Download

### Prerequisites

- [https://github.com/jedisct1/libsodium|libsodium]
- [https://github.com/bugaevc/wl-clipboard/|wl-clipboard] if you are on Wayland

### Installation

Clone the repository:
```bash
git clone --recursive https://github.com/bugaevc/wl-clipboard.git
&& cd solarpass
```

Execute the `install` script:
```bash
./install &&
cmake --build build
```

You will find the executable inside the `build` folder.
