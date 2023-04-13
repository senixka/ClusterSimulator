#include "FactoryTaskManager.h"

#include "InJobOrder.h"
#include "../Macro.h"


ITaskManager* FactoryTaskManager::Create(TaskManagerType taskManagerType) {
    switch (taskManagerType) {
        case TaskManagerType::InJobOrder:
            return (new (class InJobOrder){});

        default:
            UNREACHABLE("FactoryTaskManager");
    }
}
