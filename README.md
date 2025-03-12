# CRAM
CRAM stands for Compression RAM Manager. CRAM reduces the allocated memory demands of scientific applications during computation. It does this by storing data in its compressed form and only decompressing select partitions as they are needed for reading or writing.

Contributors:

- Kristen Guernsey (kguerns@clemson.edu)
- Emily Lattanzio
- Dr. Jon Calhoun

# LibPressio Installation
I installed LibPressio using spack. Below are the commands I used to set up the environment.

1. ```git clone https://github.com/spack/spack.git``` (Install spack on Palmetto)
2. ```cd spack```
3. ```. share/spack/setup-env.sh``` (Run this command every time you log on to Palmetto)
4. ```spack env create CRAM``` (Create new spack environment)
5. ```spack env activate CRAM``` (Run this command every time you need to activate the environment)
6. ```spack add libpressio+bitgrooming+blosc+digitrounding+docs+fpzip+hdf5+mgard+mpi+openmp+sz+sz3+szauto+unix+zfp```
7. ```spack install```

Note that the installation will take quite some time, so allocate enough resources and time for your Palmetto job.

# Compile
Compiling the programs is made easy using cmake. The ```CMakeLists.txt``` file finds the required files and libraries. The following commands can be used to build the program.

1. ```cmake -B build -S .```
2. ```cmake --build build -j 1``` (Create the executable, with 1 processor) 

# Notes
LibPressio_Practice.cpp is a basic example program pulled from https://github.com/robertu94/libpressio_tutorial/blob/master/exercises/1_basics/cxx/basics.cc
