#pragma once

#include "ITaskManager.h"
#include "../Registry.h"


struct FactoryTaskManager {
    static ITaskManager* Create(TaskManagerType taskManagerType);
};
