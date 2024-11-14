make
make clean

valgrind --leak-check=full --show-leak-kinds=all --fair-sched=yes ./philo 5 800 200 200