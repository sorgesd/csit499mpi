#!/bin/sh
#SBATCH --ntasks=1
#SBATCH --mem-per-cpu=16384
#SBATCH --time=02:00:00
#SBATCH --job-name=mpi
#SBATCH --error=job.%J.err
#SBATCH --output=job.%J.out
module load compiler/gcc/6.1 java/1.8
java -jar LatinChecker.jar 6 n /work/csit499unk/sorgesd/csit499mpi/latinsquare6printable.txt
