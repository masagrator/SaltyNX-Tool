# SaltyNX-Tool
To manage functions of this SaltyNX fork min. 2.1.0
https://github.com/masagrator/SaltyNX

To use only in Applet mode. Title replacement mode in 99.9% of cases will block function responsible for checking if SaltyNX is alive and can even crash SaltyNX.

## Building
UI is built with [xfangfang/borealis](https://github.com/xfangfang/borealis) using the deko3d renderer.

Requirements (devkitPro): `switch-dev`, `deko3d`, `uam`, `switch-cmake`, `switch-pkg-config`
```
git clone --recursive https://github.com/masagrator/SaltyNX-Tool
cd SaltyNX-Tool
make
```
`make` is a thin wrapper around CMake, it produces `SaltyNX-Tool.nro` in the project root. You can also call CMake directly:
```
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --target SaltyNX-Tool.nro
```
