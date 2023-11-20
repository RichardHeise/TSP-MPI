make clean 
make
echo -n 'Running code...'
echo ''
mpirun -np $1 ./tsptest < ../tests/$2.in
