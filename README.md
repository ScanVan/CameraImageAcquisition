cmake -DCMAKE_BUILD_TYPE=Release .
cmake -DCMAKE_BUILD_TYPE=Debug .
make run -j

sudo sysctl net.core.rmem_max=2097152