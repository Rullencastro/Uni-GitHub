#!/bin/bash

export OMP_DYNAMIC=FALSE
export OMP_SCHEDULE="static"


export OMP_NUM_THREADS=7
echo "7 thread"
srun ./pmm-OpenMP 120
srun ./pmm-OpenMP 440
srun ./pmm-OpenMP 830
srun ./pmm-OpenMP 1205

export OMP_NUM_THREADS=8
echo "8 thread"
srun ./pmm-OpenMP 120
srun ./pmm-OpenMP 440
srun ./pmm-OpenMP 830
srun ./pmm-OpenMP 1205

export OMP_NUM_THREADS=9
echo "9 thread"
srun ./pmm-OpenMP 120
srun ./pmm-OpenMP 440
srun ./pmm-OpenMP 830
srun ./pmm-OpenMP 1205

export OMP_NUM_THREADS=10
echo "10 thread"
srun ./pmm-OpenMP 120
srun ./pmm-OpenMP 440
srun ./pmm-OpenMP 830
srun ./pmm-OpenMP 1205

export OMP_NUM_THREADS=11
echo "11 thread"
srun ./pmm-OpenMP 120
srun ./pmm-OpenMP 440
srun ./pmm-OpenMP 830
srun ./pmm-OpenMP 1205

export OMP_NUM_THREADS=12
echo "12 thread"
srun ./pmm-OpenMP 120
srun ./pmm-OpenMP 440
srun ./pmm-OpenMP 830
srun ./pmm-OpenMP 1205







