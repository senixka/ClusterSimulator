#pragma once

#include "ITaskManager.h"
#include "../Registry.h"


class FactoryTaskManager {
public:
    static ITaskManager* Create(TaskManagerType taskManagerType);
};
