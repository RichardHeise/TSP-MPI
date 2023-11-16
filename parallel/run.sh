make clean 
make
echo -n 'Running code...'
echo ''
mpirun -np 4 ./tsp < ../tests/$1.in
