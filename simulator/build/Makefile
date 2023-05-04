CPP_JOB_MANAGER = ../job_manager/FactoryJobManager.cpp
CPP_TASK_MANAGER = ../task_manager/FactoryTaskManager.cpp ../task_manager/RoundRobinNB.cpp
CPP_PLACING_STRATEGY = ../placing_strategy/FactoryPlacingStrategy.cpp
CPP_PARTITION_POLICY = ../placing_strategy/partition_policy/NoPartition.cpp ../placing_strategy/partition_policy/UsedFirst.cpp ../placing_strategy/partition_policy/UnusedFirst.cpp
CPP_BASIC = ../Cluster.cpp ../Task.cpp ../Job.cpp ../MachineManager.cpp ../SchedulerImpl.cpp ../Statistics.cpp ../Experiment.cpp

CPP_FILES = ../main.cpp $(CPP_BASIC) $(CPP_PLACING_STRATEGY) $(CPP_TASK_MANAGER) $(CPP_JOB_MANAGER) $(CPP_PARTITION_POLICY)

BUILTIN_DEBUG_OFF = -DNDEBUG -DNS_BLOCK_ASSERTIONS
DISABLE_NOTES = -DBOOST_ALLOW_DEPRECATED_HEADERS
SANITIZE = -fsanitize=address,undefined
OUT_NAME = -o simulator.out
WARN = -Wall -Wextra -Wconversion -Werror

Release:
	g++-11 -O3 $(WARN) $(DISABLE_NOTES) $(BUILTIN_DEBUG_OFF) $(CPP_FILES) $(OUT_NAME)

ReleaseASan:
	g++-11 -O3 $(WARN) $(SANITIZE) $(DISABLE_NOTES) $(BUILTIN_DEBUG_OFF) $(CPP_FILES) $(OUT_NAME)

Slow:
	g++-11 -O0 -g $(WARN) $(DISABLE_NOTES) $(CPP_FILES) $(OUT_NAME)

SlowASan:
	g++-11 -O0 -g $(WARN) $(SANITIZE) $(DISABLE_NOTES) $(CPP_FILES) $(OUT_NAME)

Profile:
	g++-11 -O3 -pg $(WARN) $(DISABLE_NOTES) $(BUILTIN_DEBUG_OFF) $(CPP_FILES) $(OUT_NAME)

ProfileMemory:
	g++-11 -O3 -g $(WARN) $(DISABLE_NOTES) $(BUILTIN_DEBUG_OFF) $(CPP_FILES) $(OUT_NAME)
