#!/bin/bash
#Órdenes para el sistema de colas:
#1. Asigna al trabajo un nombre
#SBATCH --job-name=helloOMP
#2. Asignar el trabajo a una cola (partición) 
#SBATCH --partition=ac 
#2. Asignar el trabajo a un account 
#SBATCH --account=ac


echo -e "\n 2. Ejecución SumaVectores con vectores locales:\n" 
for ((P=65536;P<67108864+1;P=P*2))
do
   srun ./SumaVectoresLocal $P
done

echo -e "\n 2. Ejecución SumaVectores con vectores globales:\n" 
for ((P=65536;P<67108864+1;P=P*2))
do
   srun ./SumaVectoresGlobal $P
done

echo -e "\n 2. Ejecución SumaVectores con vectores dinamicos:\n" 
for ((P=65536;P<67108864+1;P=P*2))
do
   srun ./SumaVectoresDinam $P
done
