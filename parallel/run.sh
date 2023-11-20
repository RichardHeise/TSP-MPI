make clean 
make
echo -n 'Running code...'
echo ''
mpirun -np $1 ./tspws < ../tests/$2.in
