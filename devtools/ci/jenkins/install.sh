# This script is sourced from the main build script, and the executing directory
# is the top-level cpptraj directory. The environment variable COMPILER_FLAGS is
# set, as is label. This is set up exclusively to test the Intel compilers.

if [ "${OPERATING_SYSTEM}" = "linux" ]; then
  ./configure ${COMPILER_FLAGS} ${COMPILER}
else
  # Mac OS X
  ./configure -macAccelerate --with-fftw3=/opt/local --with-netcdf=/opt/local -noarpack ${COMPILER_FLAGS} clang
fi

compiler_flags_contains() {
  if [ `echo "${COMPILER_FLAGS}" | sed -e "s/$1//g"` = "${COMPILER_FLAGS}" ]; then
    echo "0" # It's the same, so $1 cannot be in COMPILER_FLAGS
  else
    echo "1"
  fi
}

# If the compiler flag is -mpi, set DO_PARALLEL. Test both 2 and 4 CPUs for
# parallel builds.
make -j4 install
if [ `compiler_flags_contains -mpi` -eq 1 ]; then
  export DO_PARALLEL='mpirun -np 2'
  make check
  export DO_PARALLEL='mpirun -np 4'
  make check
elif [ `compiler_flags_contains -openmp` -eq 1 ]; then
  export OPT=openmp OMP_NUM_THREADS=2
  make check
  export OMP_NUM_THREADS=4
  make check
elif [ `compiler_flags_contains -cuda` -eq 1 ]; then
  export OPT=cuda
  make check
else
  make check
fi
