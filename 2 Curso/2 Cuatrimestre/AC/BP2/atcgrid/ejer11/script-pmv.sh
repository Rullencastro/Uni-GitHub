#!/bin/bash

export OMP_NUM_THREADS=12
srun ./pmv-OpenMP-a 6000
srun ./pmv-OpenMP-a 30000
