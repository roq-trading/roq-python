# WORK IN PROGRESS !!!

This is the Python binding of the Roq C++ API.


## Operating Systems

* Linux (x86-64, AArch64)
* macOS (x86-64, Arm64)

> All listed combinations are regularly compiled but only Linux/x86-64 is continuously being tested.
> If you require a specific combination, please [contact us](mailto:info@roq-trading.com) before using.

> We plan to drop support for macOS/x86-64.


## Library/Package Dependencies

* [fmt](https://github.com/fmtlib/fmt) (MIT License)
* [magic_enum](https://github.com/Neargye/magic_enum) (MIT License)
* [nameof](https://github.com/Neargye/nameof) (MIT License)
* [pybind11](https://github.com/pybind/pybind11) (BSD 3-Clause License)
* roq-client (BSD 3-Clause License)


## Prerequisites

The project is primarily designed to be compatible with the conda package manager.

### Download and Install Miniforge

#### Linux / x86\_64

```bash
wget -N https://github.com/conda-forge/miniforge/releases/latest/download/Miniforge3-Linux-x86_64.sh

bash Miniforge3-Linux-x86_64.sh -b -u -p ~/conda

~/conda/bin/conda install -y \
    'gxx_linux-64>=11'
```

#### Linux / AArch64

```bash
wget -N https://github.com/conda-forge/miniforge/releases/latest/download/Miniforge3-Linux-aarch64.sh

bash Miniforge3-Linux-aarch64.sh -b -u -p ~/conda

~/conda/bin/conda install -y \
    'gxx_linux-aarch64>=11'
```

#### macOS / x86\_64

```bash
wget -N https://github.com/conda-forge/miniforge/releases/latest/download/Miniforge3-MacOSX-x86_64.sh

bash Miniforge3-MacOSX-x86_64.sh -b -u -p ~/conda

~/conda/bin/conda install -y \
   'clang_osx-64>=13'
```

#### macOS / Arm64

```bash
wget -N https://github.com/conda-forge/miniforge/releases/latest/download/Miniforge3-MacOSX-arm64.sh

bash Miniforge3-MacOSX-arm64.sh -b -u -p ~/conda

~/conda/bin/conda install -y \
   'clang_osx-arm64>=13'
```

### Install Dependencies

```bash
source ~/conda/bin/activate

conda install -y \
    git \
    cmake \
    pip \
    fmt

conda install -y --channel https://roq-trading.com/conda/stable \
    roq-oss-magic_enum \
    roq-oss-nameof \
    roq-client
```


## Building

```bash
pip install .
```

### Using


You can download the binary package like this

```bash
conda install -y --channel https://roq-trading.com/conda/stable \
    roq-python
```

Samples can be found [here](https://github.com/roq-trading/roq-python/samples).


## License

The project is released under the terms of the BSD 3-Clause license.
