#include "FactoryTaskManager.h"

#include "RoundRobinNB.h"
#include "MinTaskTimeNB.h"
#include "MaxTaskTimeNB.h"

#include "../Macro.h"


ITaskManager* FactoryTaskManager::Create(TaskManagerType taskManagerType) {
    switch (taskManagerType) {
        case TaskManagerType::RoundRobinNB:
            return (new task_manager::RoundRobinNB{});

        case TaskManagerType::MinTaskTimeNB:
            return (new task_manager::MinTaskTimeNB{});

        case TaskManagerType::MaxTaskTimeNB:
            return (new task_manager::MaxTaskTimeNB{});

        default:
            UNREACHABLE("FactoryTaskManager");
    }
}
