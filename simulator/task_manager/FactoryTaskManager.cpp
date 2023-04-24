#include "FactoryTaskManager.h"

#include "InJobOrderB.h"
#include "MinTaskTimeB.h"
#include "MaxTaskTimeB.h"

#include "../Macro.h"


ITaskManager* FactoryTaskManager::Create(TaskManagerType taskManagerType) {
    switch (taskManagerType) {
        case TaskManagerType::InJobOrderB:
            return (new task_manager::InJobOrderB{});

        case TaskManagerType::MinTaskTimeB:
            return (new task_manager::MinTaskTimeB{});

        case TaskManagerType::MaxTaskTimeB:
            return (new task_manager::MaxTaskTimeB{});

        default:
            UNREACHABLE("FactoryTaskManager");
    }
}
