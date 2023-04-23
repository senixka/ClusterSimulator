#include "FactoryTaskManager.h"

#include "InJobOrder.h"
#include "MinTimeFirst.h"
#include "MaxTimeFirst.h"

#include "../Macro.h"


ITaskManager* FactoryTaskManager::Create(TaskManagerType taskManagerType) {
    switch (taskManagerType) {
        case TaskManagerType::InJobOrder:
            return (new task_manager::InJobOrder{});

        case TaskManagerType::MinTimeFirst:
            return (new task_manager::MinTimeFirst{});

        case TaskManagerType::MaxTimeFirst:
            return (new task_manager::MaxTimeFirst{});

        default:
            UNREACHABLE("FactoryTaskManager");
    }
}
