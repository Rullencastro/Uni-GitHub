#!/bin/bash

export OMP_DYNAMIC=FALSE
export OMP_NUM_THREADS=12

export OMP_SCHEDULE="static"
echo "static defecto"
srun ./pmtv-OpenMP 15360

export OMP_SCHEDULE="static,1"
echo "static 1"
srun ./pmtv-OpenMP 15360

export OMP_SCHEDULE="static,64"
echo "static 64"
srun ./pmtv-OpenMP 15360

export OMP_SCHEDULE="dynamic"
echo "dynamic defecto"
srun ./pmtv-OpenMP 15360

export OMP_SCHEDULE="dynamic,1"
echo "dynamic 1"
srun ./pmtv-OpenMP 15360

export OMP_SCHEDULE="dynamic,64"
echo "dynamic 64"
srun ./pmtv-OpenMP 15360

export OMP_SCHEDULE="guided"
echo "guided defecto"
srun ./pmtv-OpenMP 15360

export OMP_SCHEDULE="guided,1"
echo "guided 1"
srun ./pmtv-OpenMP 15360

export OMP_SCHEDULE="guided,64"
echo "guided 64"
srun ./pmtv-OpenMP 15360

