#!/bin/bash
#Órdenes para el sistema de colas:
#1. Asigna al trabajo un nombre
#SBATCH --job-name=helloOMP
#2. Asignar el trabajo a una cola (partición) 
#SBATCH --partition=ac 
#2. Asignar el trabajo a un account 
#SBATCH --account=ac


echo "\n 1. Ejecución SumaVectores con vectores locales:\n\n\n\n" 
for ((P=65536 ; P<67108864+1 ; P=P*2))
do
   ./SumaVectoresLocal $P
done

echo "\n 2. Ejecución SumaVectores con vectores globales:\n\n\n\n" 
for ((P=65536;P<67108864+1;P=P*2))
do
   ./SumaVectoresGlobal $P
done

echo "\n 3. Ejecución SumaVectores con vectores dinamicos:\n\n" 
for ((P=65536;P<67108864+1;P=P*2))
do
   ./SumaVectoresDinam $P
done
