make clean 
make
echo -n 'Running code...'
echo ''
./tsp < ../tests/$1.in
