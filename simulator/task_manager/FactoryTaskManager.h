#pragma once

#include "ITaskManager.h"


enum TaskManagerType {
    InJobOrder = 0,
};


class FactoryTaskManager {
public:
    static ITaskManager* Create(TaskManagerType taskManagerType);
};
