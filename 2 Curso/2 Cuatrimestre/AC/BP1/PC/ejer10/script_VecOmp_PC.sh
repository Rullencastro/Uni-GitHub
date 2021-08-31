#!/bin/bash
#Órdenes para el sistema de colas:
#1. Asigna al trabajo un nombre
#SBATCH --job-name=helloOMP
#2. Asignar el trabajo a una cola (partición) 
#SBATCH --partition=ac 
#2. Asignar el trabajo a un account 
#SBATCH --account=ac





echo "Ejecución SumaVectores Parallel For:\n\n" 
for ((P=16384;P<67108864+1;P=P*2))
do
   ./SumaVecOmpSec $P
done


