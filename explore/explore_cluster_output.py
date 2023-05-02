import multiprocessing
import os
import matplotlib.pyplot as plt
import matplotlib
import decimal
from decimal import Decimal
import multiprocessing


matplotlib.use('Agg')
plt.ioff()

OUT_PREC = 20


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

    # ////////////////////// Average Pending, Working //////////////
    sumPending, sumWorking, eventCnt = 0, 0, 0
    for pending, working in zip(metrics['PendingTask'], metrics['WorkingTask']):
        sumPending += pending
        sumWorking += working
        eventCnt += 1

    metrics['AvgPendingTask'] = Decimal(sumPending) / Decimal(eventCnt)
    metrics['AvgWorkingTask'] = Decimal(sumWorking) / Decimal(eventCnt)

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
        for name in ['AvgCpuUtilization', 'AvgMemoryUtilization', 'AvgPendingTask', 'AvgWorkingTask',
                     'SNP', 'Unfairness', 'SlowdownNorm2']:
            fout.write(name + '\n')
            fout.write(str(metrics[name])[:OUT_PREC] + '\n')

        for name in ['MakeSpan', 'TotalAvailableCPU', 'TotalAvailableMemory', 'PendingTaskCounter',
                     'UnfinishedJobCounter', 'nJobInSimulation', 'nTaskInSimulation', 'TaskFinishedCounter']:
            fout.write(name + '\n')
            fout.write(str(metrics[name]) + '\n')


def BuildPlots(metrics: dict, outputPathPrefix: str):
    mTimes = metrics['UtilizationMeasurementsTime']

    # ////////////////////// Task Pending vs Working //////////////////////

    plt.figure(figsize=(12, 5))
    plt.title(outputPathPrefix.split('/')[-1] + '_pending_working')
    plt.xlabel('Time (in seconds)')
    plt.ylabel('Task count')

    plt.plot(mTimes, metrics['WorkingTask'], label='Working task')
    plt.plot(mTimes[0], metrics['PendingTask'][0], label='AvgWorkingTask: ' + str(metrics['AvgWorkingTask'])[:OUT_PREC])
    plt.plot(mTimes, metrics['PendingTask'], label='Pending task')
    plt.plot(mTimes[0], metrics['PendingTask'][0], label='AvgPendingTask: ' + str(metrics['AvgPendingTask'])[:OUT_PREC])

    plt.plot(mTimes[0], metrics['PendingTask'][0], label='SlowdownNorm2: ' + str(metrics['SlowdownNorm2'])[:OUT_PREC])
    plt.plot(mTimes[0], metrics['PendingTask'][0], label='Unfairness: ' + str(metrics['Unfairness'])[:OUT_PREC])
    plt.plot(mTimes[0], metrics['PendingTask'][0], label='SNP: ' + str(metrics['SNP'])[:OUT_PREC])

    plt.legend()
    plt.savefig(outputPathPrefix + '_pending_working.png')
    plt.close()

    # ////////////////////// Utilization //////////////////////

    plt.figure(figsize=(12, 5))
    plt.title(outputPathPrefix.split('/')[-1] + '_utilization')
    plt.xlabel('Time (in seconds)')
    plt.ylabel('Resource (in percent)')

    plt.plot(mTimes, metrics['UtilizationCPU'], label='CPU')
    plt.plot([mTimes[0], mTimes[-1]], [metrics['AvgCpuUtilization']] * 2,
             label='CPU AVG ' + str(metrics['AvgCpuUtilization'])[:OUT_PREC])

    plt.plot(mTimes, metrics['UtilizationMemory'], label='Memory')
    plt.plot([mTimes[0], mTimes[-1]], [metrics['AvgMemoryUtilization']] * 2,
             label='Memory AVG ' + str(metrics['AvgMemoryUtilization'])[:OUT_PREC])

    plt.legend()
    plt.savefig(outputPathPrefix + '_utilization.png')
    plt.close()


def Worker(filePath: str, subDir: str, removeInput: int):
    print(f'Process path: {filePath}')

    prefix = filePath[:-len('.txt')].split('/')[-1]

    print(f'Metrics from file: {filePath}')
    metrics = GetMetricsFromFile(filePath)

    print(f'Build stats: {filePath}')
    BuildStats(metrics, './stats_of_output/' + subDir + '/' + prefix)

    print(f'Build plots: {filePath}')
    BuildPlots(metrics, './plots_of_output/' + subDir + '/' + prefix)

    if removeInput == 1:
        os.remove(filePath)

    print(f'End of process: {filePath}')


def main():
    subDir = input('Enter output sub dir name: ').strip()
    assert(len(subDir) > 0)

    os.mkdir('./stats_of_output/' + subDir)
    os.mkdir('./plots_of_output/' + subDir)

    removeInput = int(input('Enter 1 to remove input after use, 0 otherwise: '))

    pool = multiprocessing.Pool(int(input('Enter max thread count: ').strip()))

    for filePath in GetFiles():
        if not filePath.endswith('.txt'):
            continue
        pool.apply_async(Worker, [filePath, subDir, removeInput])

    pool.close()
    pool.join()


if __name__ == '__main__':
    main()
