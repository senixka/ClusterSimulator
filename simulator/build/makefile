CPP_FILES = ../main.cpp ../Machine.cpp ../Job.cpp ../Cluster.cpp ../SchedulerRandom.cpp ../Statistics.cpp ../MachineManager.cpp
LINK_PYTHON =  -I /usr/include/python3.10 -lpython3.10
NO_DEBUG = -DNDEBUG -DNS_BLOCK_ASSERTIONS
DISABLE_NOTES = -DBOOST_ALLOW_DEPRECATED_HEADERS
SANITIZE = -fsanitize=address,undefined

Release:
	g++ -Wall -Wextra -Werror -O3 $(DISABLE_NOTES) $(NO_DEBUG) $(CPP_FILES) $(LINK_PYTHON)

Fast:
	g++ -Wall -Wextra -O3 $(DISABLE_NOTES) $(CPP_FILES) $(LINK_PYTHON)

FastASan:
	g++ -Wall -Wextra -O3 $(SANITIZE) $(DISABLE_NOTES) $(CPP_FILES) $(LINK_PYTHON)

Slow:
	g++ -Wall -Wextra -O0 -g $(DISABLE_NOTES) $(CPP_FILES) $(LINK_PYTHON)

SlowASan:
	g++ -Wall -Wextra -O0 -g $(SANITIZE) $(DISABLE_NOTES) $(CPP_FILES) $(LINK_PYTHON)
