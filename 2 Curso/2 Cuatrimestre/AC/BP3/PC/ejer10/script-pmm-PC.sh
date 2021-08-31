#!/bin/bash

export OMP_DYNAMIC=FALSE
export OMP_SCHEDULE="static"


export OMP_NUM_THREADS=1
echo "1 thread"
./pmm-OpenMP 120
./pmm-OpenMP 440
./pmm-OpenMP 830
./pmm-OpenMP 1205

export OMP_NUM_THREADS=2
echo "2 thread"
./pmm-OpenMP 120
./pmm-OpenMP 440
./pmm-OpenMP 830
./pmm-OpenMP 1205

export OMP_NUM_THREADS=3
echo "3 thread"
./pmm-OpenMP 120
./pmm-OpenMP 440
./pmm-OpenMP 830
./pmm-OpenMP 1205

export OMP_NUM_THREADS=4
echo "4 thread"
./pmm-OpenMP 120
./pmm-OpenMP 440
./pmm-OpenMP 830
./pmm-OpenMP 1205






