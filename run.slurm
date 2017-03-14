#!/bin/sh
#SBATCH --ntasks=128
#SBATCH --mem-per-cpu=4096
#SBATCH --time=01:30:00
#SBATCH --job-name=mpi
#SBATCH --error=job.%J.err
#SBATCH --output=job.%J.out
module load compiler/gcc/6.1 openmpi/2.0
mpirun /work/csit499unk/sorgesd/csit499mpi/latinSquares.out
