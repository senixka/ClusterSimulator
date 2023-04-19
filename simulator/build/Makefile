CPP_JOB_MANAGER = ../job_manager/FactoryJobManager.cpp ../job_manager/RoundRobinBlockingHead.cpp ../job_manager/RoundRobinNonBlockingHead.cpp
CPP_TASK_MANAGER = ../task_manager/FactoryTaskManager.cpp ../task_manager/InJobOrder.cpp
CPP_PLACING_STRATEGY = ../placing_strategy/FactoryPlacingStrategy.cpp ../placing_strategy/Random.cpp ../placing_strategy/MinVolume.cpp ../placing_strategy/MaxVolume.cpp ../placing_strategy/Tetris.cpp
CPP_BASIC = ../Cluster.cpp ../Job.cpp ../Machine.cpp ../MachineManager.cpp ../SchedulerImpl.cpp ../Statistics.cpp ../Experiment.cpp

CPP_FILES = ../main.cpp $(CPP_BASIC) $(CPP_PLACING_STRATEGY) $(CPP_TASK_MANAGER) $(CPP_JOB_MANAGER)

BUILTIN_DEBUG_OFF = -DNDEBUG -DNS_BLOCK_ASSERTIONS
DISABLE_NOTES = -DBOOST_ALLOW_DEPRECATED_HEADERS
SANITIZE = -fsanitize=address,undefined
OUT_NAME = -o simulator.out

Release:
	g++ -Wall -Wextra -O3 $(DISABLE_NOTES) $(BUILTIN_DEBUG_OFF) $(CPP_FILES) $(OUT_NAME)

ReleaseASan:
	g++ -Wall -Wextra -O3 $(SANITIZE) $(DISABLE_NOTES) $(BUILTIN_DEBUG_OFF) $(CPP_FILES) $(OUT_NAME)

Slow:
	g++ -Wall -Wextra -O0 -g $(DISABLE_NOTES) $(CPP_FILES) $(OUT_NAME)

SlowASan:
	g++ -Wall -Wextra -O0 -g $(SANITIZE) $(DISABLE_NOTES) $(CPP_FILES) $(OUT_NAME)