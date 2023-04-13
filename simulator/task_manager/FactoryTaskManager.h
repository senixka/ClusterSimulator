#ifndef CLUSTERSIMULATOR_FACTORYTASKMANAGER_H
#define CLUSTERSIMULATOR_FACTORYTASKMANAGER_H

#include "ITaskManager.h"

#include <string>


enum TaskManagerType {
    InJobOrder = 0,
};

class FactoryTaskManager {
public:
    static ITaskManager* Create(TaskManagerType taskManagerType);
};


#endif //CLUSTERSIMULATOR_FACTORYTASKMANAGER_H
