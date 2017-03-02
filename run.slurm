#!/bin/sh
#SBATCH --ntasks=64
#SBATCH --mem-per-cpu=1024
#SBATCH --time=01:00:00
#SBATCH --job-name=mpi
#SBATCH --error=job.%J.err
#SBATCH --output=job.%J.out
module load compiler/gcc/6.1 openmpi/2.0
mpirun /work/csit499unk/stahlysr/latinSquares/latinSquares.out
