import os
import matplotlib.pyplot as plt

def GetFiles():
    dirPath = '../simulator/output/'
    files = []
    for path in os.listdir(dirPath):
        if os.path.isfile(os.path.join(dirPath, path)):
            files.append(dirPath + path)

    return list(filter(lambda x: x.endswith('.txt'), files))


def NumberType(value: str):
    if value.count('.') > 0:
        return float(value)
    return int(value)


def GetMetricsFromFile(inputFilePath: str):
    metrics = {}
    with open(inputFilePath, 'r') as fin:
        for _ in range(13):
            name = fin.readline().strip()
            value = NumberType(fin.readline().strip())
            metrics[name] = value

        for _ in range(2):
            names = fin.readline().strip().split(' ')
            for name in names:
                metrics[name] = []

            size = int(fin.readline().strip())
            for i in range(size):
                values = list(map(NumberType, fin.readline().strip().split(' ')))
                for name, value in zip(names, values):
                    metrics[name].append(value)

    return metrics


def BuildPlots(metrics: dict, outputPathPrefix: str):
    mTimes = metrics['UtilizationMeasurementsTime']

    ######################## Task Pending vs Working ######################

    plt.figure(figsize=(12, 5))
    plt.title('Task Pending vs Working')
    plt.xlabel('Time (in microseconds)')
    plt.ylabel('Task count')

    plt.plot(mTimes, metrics['WorkingTask'], label='Working task')
    plt.plot(mTimes, metrics['PendingTask'], label="Pending task")
    plt.plot(mTimes[0], metrics['PendingTask'][0], label="Slowdown: " + str(metrics['Slowdown']))
    plt.plot(mTimes[0], metrics['PendingTask'][0], label="Unfairness: " + str(metrics['Unfairness']))
    plt.plot(mTimes[0], metrics['PendingTask'][0], label="SNP: " + str(metrics['SNP']))
    plt.plot(mTimes[0], metrics['PendingTask'][0], label="Min ANP: " + str(metrics['MinANP']))
    plt.plot(mTimes[0], metrics['PendingTask'][0], label="Max ANP: " + str(metrics['MaxANP']))

    plt.legend()
    plt.savefig(outputPathPrefix + '_cluster_pending_working.png')
    plt.show()

    ############################ Utilization ##############################

    plt.figure(figsize=(12, 5))
    plt.title("Cluster resource utilization")
    plt.xlabel("Time (in microseconds)")
    plt.ylabel("Resource (in percent)")

    plt.plot(mTimes, metrics['UtilizationCPU'], label="CPU")
    plt.plot([mTimes[0], mTimes[-1]], [metrics['AverageUtilizationCPU']] * 2,
             label="CPU AVG " + str(metrics['AverageUtilizationCPU']))

    plt.plot(mTimes, metrics['UtilizationMemory'], label="Memory")
    plt.plot([mTimes[0], mTimes[-1]], [metrics['AverageUtilizationMemory']] * 2,
             label="Memory AVG " + str(metrics['AverageUtilizationMemory']))

    plt.plot(mTimes, metrics['UtilizationDisk'], label="Disk")
    plt.plot([mTimes[0], mTimes[-1]], [metrics['AverageUtilizationDisk']] * 2,
             label="Disk AVG " + str(metrics['AverageUtilizationDisk']))

    plt.legend()
    plt.savefig(outputPathPrefix + '_cluster_utilization.png')
    plt.show()


def main():
    for filePath in GetFiles():
        metrics = GetMetricsFromFile(filePath)
        prefix = filePath.removesuffix('.txt').split('/')[-1]
        BuildPlots(metrics, './plots/' + prefix)


if __name__ == '__main__':
    main()
