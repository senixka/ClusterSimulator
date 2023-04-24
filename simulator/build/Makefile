CPP_JOB_MANAGER = ../job_manager/FactoryJobManager.cpp ../job_manager/RoundRobinB.cpp ../job_manager/RoundRobinNB.cpp
CPP_TASK_MANAGER = ../task_manager/FactoryTaskManager.cpp ../task_manager/InJobOrder.cpp
CPP_PLACING_STRATEGY = ../placing_strategy/FactoryPlacingStrategy.cpp
CPP_PRIORITY_REORDER = ../placing_strategy/partition_policy/NoReorder.cpp ../placing_strategy/partition_policy/UsedFirst.cpp ../placing_strategy/partition_policy/UnusedFirst.cpp
CPP_BASIC = ../Cluster.cpp ../Job.cpp ../Machine.cpp ../MachineManager.cpp ../SchedulerImpl.cpp ../Statistics.cpp ../Experiment.cpp

CPP_FILES = ../main.cpp $(CPP_BASIC) $(CPP_PLACING_STRATEGY) $(CPP_TASK_MANAGER) $(CPP_JOB_MANAGER) $(CPP_PRIORITY_REORDER)

BUILTIN_DEBUG_OFF = -DNDEBUG -DNS_BLOCK_ASSERTIONS
DISABLE_NOTES = -DBOOST_ALLOW_DEPRECATED_HEADERS
SANITIZE = -fsanitize=address,undefined
OUT_NAME = -o simulator.out
WARN = -Wall -Wextra -Wconversion

Release:
	g++ -march=native -mtune=native -O3 $(WARN) $(DISABLE_NOTES) $(BUILTIN_DEBUG_OFF) $(CPP_FILES) $(OUT_NAME)

ReleaseASan:
	g++ -O3 $(WARN) $(SANITIZE) $(DISABLE_NOTES) $(BUILTIN_DEBUG_OFF) $(CPP_FILES) $(OUT_NAME)

Slow:
	g++ -O0 -g $(WARN) $(DISABLE_NOTES) $(CPP_FILES) $(OUT_NAME)

SlowASan:
	g++ -O0 -g $(WARN) $(SANITIZE) $(DISABLE_NOTES) $(CPP_FILES) $(OUT_NAME)

Profile:
	g++ -O3 -pg $(WARN) $(DISABLE_NOTES) $(BUILTIN_DEBUG_OFF) $(CPP_FILES) $(OUT_NAME)
