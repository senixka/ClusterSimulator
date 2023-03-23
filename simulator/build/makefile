CPP_FILES = ../main.cpp ../Machine.cpp ../Job.cpp ../Cluster.cpp ../Scheduler.cpp ../Statistics.cpp

fast:
	g++ -Wall -Wextra -O3 $(CPP_FILES) -I /usr/include/python3.10 -lpython3.10

slow:
	g++ -Wall -Wextra -O0 -g -fsanitize=address,undefined $(CPP_FILES) -I /usr/include/python3.10 -lpython3.10
