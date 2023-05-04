import csv
import os
from decimal import Decimal


def GetFiles(dirPath):
    files = []
    for path in os.listdir(dirPath):
        if os.path.isfile(os.path.join(dirPath, path)):
            files.append(dirPath + path)

    return list(filter(lambda x: x.endswith('_stat.txt'), files))


def NumberType(value: str):
    if value.count('.') > 0:
        return float(value)
    return int(value)


def GetStats(dirPath: str):
    metrics = []

    for filePath in GetFiles(dirPath):
        with open(filePath, 'r') as fin:
            fin.readline().strip()
            jobManagerName = fin.readline().strip()
            fin.readline().strip()
            taskManagerName = fin.readline().strip()
            fin.readline().strip()
            placingStrategyName = fin.readline().strip()

            metrics.append({})

            metrics[-1]['JobManagerName'] = jobManagerName
            metrics[-1]['TaskManagerName'] = taskManagerName
            metrics[-1]['PlacingStrategyName'] = placingStrategyName

            algoName = jobManagerName + '-' + taskManagerName + '-' + placingStrategyName
            metrics[-1]['Algorithm'] = algoName

            while True:
                name = fin.readline().strip()
                value = fin.readline().strip()

                if len(name) == 0:
                    break

                metrics[-1][name] = NumberType(value)

    return metrics


def CalculateBest(metrics: list, kBest: int):
    namesASC = ['AvgPendingTask', 'Unfairness', 'SlowdownNorm2', 'MaxPendingTask']
    namesDESC = ['AvgCpuUtilization', 'AvgMemoryUtilization', 'AvgWorkingTask', 'SNP']

    maxMakeSpan = max([x['MakeSpan'] for x in metrics])
    metrics = list(filter(lambda x: x['MakeSpan'] == maxMakeSpan, metrics))
    algoNames = set()

    for i in range(len(metrics)):
        metrics[i]['Score'] = Decimal(0)

    for name in namesASC + namesDESC:
        metrics.sort(key=lambda x: x[name], reverse=(name in namesDESC))
        bestValue = Decimal(metrics[0][name])

        for row in metrics:
            row['Score'] += abs(Decimal(row[name]) - bestValue) / bestValue

        print('--------------', name, '--------------')
        for i in range(kBest):
            print(metrics[i]['Algorithm'], metrics[i][name])
            algoNames.add(metrics[i]['Algorithm'])
        print()

    # Best by score
    metrics.sort(key=lambda x: x['Score'])
    print('-------------- Score --------------')
    for i in range(kBest):
        print(metrics[i]['Algorithm'], metrics[i]['Score'])
        algoNames.add(metrics[i]['Algorithm'])
    print()

    # All best
    print('-------------- Best algorithms --------------')
    print(*algoNames, sep='\n', end='\n\n')

    return metrics


def Main():
    # Init metrics
    subDir = input('Enter output sub dir name: ').strip()
    assert(len(subDir) > 0)

    dirPath = './stats_of_output/' + subDir + '/'
    assert(os.path.isdir(dirPath))

    metrics = GetStats(dirPath)
    metrics = CalculateBest(metrics, 10)

    metrics.sort(key=lambda x: x['Score'])

    outKeys = ['Algorithm', 'Score', 'AvgCpuUtilization', 'AvgMemoryUtilization',
               'AvgPendingTask', 'AvgWorkingTask', 'MaxPendingTask', 'Unfairness', 'SlowdownNorm2', 'SNP']

    for i in range(len(metrics)):
        metrics[i] = {key: metrics[i][key] for key in outKeys}

    with open('./stats_of_output/' + subDir + '.csv', 'w', newline='') as fout:
        writer = csv.DictWriter(fout, outKeys)
        writer.writeheader()
        writer.writerows(metrics)


if __name__ == '__main__':
    Main()
