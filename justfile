set windows-shell := ["pwsh", "-NoProfile", "-c"]

setup:
    cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug

build:
    cmake --build build

run:
    ./build/Nodexel
