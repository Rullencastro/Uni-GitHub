#!/bin/bash

export OMP_DYNAMIC=FALSE
export OMP_SCHEDULE="static"


export OMP_NUM_THREADS=1
echo "1 thread"
srun ./pmm-OpenMP 120
srun ./pmm-OpenMP 440
srun ./pmm-OpenMP 830
srun ./pmm-OpenMP 1205

export OMP_NUM_THREADS=2
echo "2 thread"
srun ./pmm-OpenMP 120
srun ./pmm-OpenMP 440
srun ./pmm-OpenMP 830
srun ./pmm-OpenMP 1205

export OMP_NUM_THREADS=3
echo "3 thread"
srun ./pmm-OpenMP 120
srun ./pmm-OpenMP 440
srun ./pmm-OpenMP 830
srun ./pmm-OpenMP 1205

export OMP_NUM_THREADS=4
echo "4 thread"
srun ./pmm-OpenMP 120
srun ./pmm-OpenMP 440
srun ./pmm-OpenMP 830
srun ./pmm-OpenMP 1205

export OMP_NUM_THREADS=5
echo "5 thread"
srun ./pmm-OpenMP 120
srun ./pmm-OpenMP 440
srun ./pmm-OpenMP 830
srun ./pmm-OpenMP 1205

export OMP_NUM_THREADS=6
echo "6 thread"
srun ./pmm-OpenMP 120
srun ./pmm-OpenMP 440
srun ./pmm-OpenMP 830
srun ./pmm-OpenMP 1205








