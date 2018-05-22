#[1] Set up compiler environment:
module purge
module load gnutools
export MODULEPATH=/share/apps/compute/modulefiles:$MODULEPATH
module load intel/2018.1.163 
module load cmake
#[2] Set up the compiler to be used by cmake:
export CC=icc
export CXX=icpc

