import os
import matplotlib.pyplot as plt
import matplotlib
import decimal
from decimal import Decimal

matplotlib.use('Agg')
plt.ioff()


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
        for _ in range(8):
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


def BuildStats(metrics: dict, outputPathPrefix: str):
    decimal.getcontext().prec = 100

    # ////////////////////// Average Utilization //////////////
    metrics['UtilizationCPU'], metrics['UtilizationMemory'] = [], []

    sumCpuNumerator, sumMemoryNumerator, eventCnt = 0, 0, 0
    for cpuNumerator, memNumerator in zip(metrics['UtilizationCPUNumerator'], metrics['UtilizationMemoryNumerator']):
        sumCpuNumerator += cpuNumerator
        sumMemoryNumerator += memNumerator
        eventCnt += 1

        metrics['UtilizationCPU'].append(float(cpuNumerator) / float(metrics['TotalAvailableCPU']))
        metrics['UtilizationMemory'].append(float(memNumerator) / float(metrics['TotalAvailableMemory']))

    metrics['AvgCpuUtilization'] = Decimal(sumCpuNumerator) / Decimal(eventCnt * metrics['TotalAvailableCPU'])
    metrics['AvgMemoryUtilization'] = Decimal(sumMemoryNumerator) / Decimal(eventCnt * metrics['TotalAvailableMemory'])

    # ////////////////////// Job's ANP ////////////////////////
    metrics['JobANP'] = []
    for jobStartTime, jobEndTime, jobIdleTime in zip(metrics['JobStartTime'], metrics['JobEndTime'],
                                                     metrics['JobIdealEstimateTime']):
        metrics['JobANP'].append(Decimal(jobIdleTime) / Decimal(jobEndTime - jobStartTime))
        assert (0 <= metrics['JobANP'][-1] <= 1)

    # ////////////////////// SNP //////////////////////////////
    snp = Decimal(0)
    for anp in metrics['JobANP']:
        snp += anp.ln()
    jobANPLen = Decimal(len(metrics['JobANP']))

    metrics['SNP'] = (snp / jobANPLen).exp()

    # ////////////////////// Unfairness ///////////////////////
    meanANP = Decimal(0)
    for anp in metrics['JobANP']:
        meanANP += anp
    meanANP /= jobANPLen

    stdDeviation = Decimal(0)
    for anp in metrics['JobANP']:
        stdDeviation += (anp - meanANP) ** 2
    stdDeviation = (stdDeviation / jobANPLen).sqrt()

    metrics['Unfairness'] = stdDeviation * Decimal(100) / meanANP

    # ////////////////////// Slowdown 2-norm /////////////////////
    slowdown = Decimal(0)
    for anp in metrics['JobANP']:
        slowdown += Decimal(1) / anp ** 2

    metrics['SlowdownNorm2'] = (slowdown / jobANPLen).sqrt()

    # ////////////////////// Write out /////////////////////
    with open(outputPathPrefix + '_stat.txt', 'w') as fout:
        for name in ['AvgCpuUtilization', 'AvgMemoryUtilization', 'SNP', 'Unfairness', 'SlowdownNorm2']:
            fout.write(name + '\n')
            fout.write(str(metrics[name])[:20] + '\n')

        for name in ['MakeSpan', 'TotalAvailableCPU', 'TotalAvailableMemory', 'PendingTaskCounter',
                     'UnfinishedJobCounter', 'nJobInSimulation', 'nTaskInSimulation', 'TaskFinishedCounter']:
            fout.write(name + '\n')
            fout.write(str(metrics[name]) + '\n')


def BuildPlots(metrics: dict, outputPathPrefix: str):
    mTimes = metrics['UtilizationMeasurementsTime']

    ######################## Task Pending vs Working ######################

    plt.figure(figsize=(12, 5))
    plt.title(outputPathPrefix.split('/')[-1] + '_pending_working')
    plt.xlabel('Time (in microseconds)')
    plt.ylabel('Task count')

    plt.plot(mTimes, metrics['WorkingTask'], label='Working task')
    plt.plot(mTimes, metrics['PendingTask'], label='Pending task')
    plt.plot(mTimes[0], metrics['PendingTask'][0], label='SlowdownNorm2: ' + str(metrics['SlowdownNorm2']))
    plt.plot(mTimes[0], metrics['PendingTask'][0], label='Unfairness: ' + str(metrics['Unfairness']))
    plt.plot(mTimes[0], metrics['PendingTask'][0], label='SNP: ' + str(metrics['SNP']))

    plt.legend()
    plt.savefig(outputPathPrefix + '_pending_working.png')
    plt.close()

    ############################ Utilization ##############################

    plt.figure(figsize=(12, 5))
    plt.title(outputPathPrefix.split('/')[-1] + '_utilization')
    plt.xlabel('Time (in microseconds)')
    plt.ylabel('Resource (in percent)')

    plt.plot(mTimes, metrics['UtilizationCPU'], label='CPU')
    plt.plot([mTimes[0], mTimes[-1]], [metrics['AvgCpuUtilization']] * 2,
             label='CPU AVG ' + str(metrics['AvgCpuUtilization']))

    plt.plot(mTimes, metrics['UtilizationMemory'], label='Memory')
    plt.plot([mTimes[0], mTimes[-1]], [metrics['AvgMemoryUtilization']] * 2,
             label='Memory AVG ' + str(metrics['AvgMemoryUtilization']))

    plt.legend()
    plt.savefig(outputPathPrefix + '_utilization.png')
    plt.close()


def main():
    for filePath in GetFiles():
        if not filePath.endswith('.txt'):
            continue
        print('---> Current path:', filePath)

        prefix = filePath[:-len('.txt')].split('/')[-1]

        print('Init metrics')
        metrics = GetMetricsFromFile(filePath)

        print('Build stats')
        BuildStats(metrics, './stats_of_output/' + prefix)

        print('Build plots')
        BuildPlots(metrics, './plots_of_output/' + prefix)


if __name__ == '__main__':
    main()
