make clean 
make
echo -n 'Running code...'
echo ''
mpirun -np $1 ./tsp < ../tests/$2.in
