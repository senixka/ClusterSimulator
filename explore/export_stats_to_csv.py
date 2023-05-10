import csv
import os
from decimal import Decimal

NAMES_IN_SCORE = ['1-AvgCpuUtilization', '1-AvgMemoryUtilization', '1-SNP',
                  'AvgPendingTask', 'Unfairness', 'SlowdownNorm2', 'MaxPendingTask']

OUT_NAMES = NAMES_IN_SCORE + ['AvgCpuUtilization', 'AvgMemoryUtilization', 'SNP']
OUT_NAMES = OUT_NAMES + list(map(lambda x: x + '_STDDIV', OUT_NAMES))


def GetFiles(dirPath):
    files = []
    for path in os.listdir(dirPath):
        if os.path.isfile(os.path.join(dirPath, path)):
            files.append(dirPath + path)

    return list(filter(lambda x: x.endswith('_stat.txt'), files))


def NumberType(value: str):
    if value.count('.') > 0:
        return Decimal(value)
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
    maxMakeSpan = max([x['MakeSpan'] for x in metrics])
    metrics = list(filter(lambda x: x['MakeSpan'] == maxMakeSpan, metrics))
    algoNames = set()

    for i in range(len(metrics)):
        for name in NAMES_IN_SCORE:
            metrics[i][name + '_std'] = Decimal(0)

    for name in NAMES_IN_SCORE:
        mean = Decimal(0)
        for row in metrics:
            mean += row[name]
        mean /= Decimal(len(metrics))

        stdDiv = Decimal(0)
        for row in metrics:
            stdDiv += (row[name] - mean) ** Decimal(2)
        stdDiv /= Decimal(len(metrics) - 1)

        for row in metrics:
            row['STD_' + name] = (row[name] - mean) / stdDiv
            row['SIGMOID_STD_' + name] = Decimal(1) / (Decimal(1) + (-row['STD_' + name]).exp())

        metrics.sort(key=lambda x: x['SIGMOID_STD_' + name])

        print('--------------', 'SIGMOID_STD_' + name, '--------------')
        for i in range(kBest):
            print(metrics[i]['Algorithm'], metrics[i]['SIGMOID_STD_' + name])
            algoNames.add(metrics[i]['Algorithm'])
        print()

    for row in metrics:
        scoreNorm2, scoreSum = Decimal(0), Decimal(0)
        for name in NAMES_IN_SCORE:
            scoreNorm2 += row['SIGMOID_STD_' + name] ** Decimal(2)
            scoreSum += row['SIGMOID_STD_' + name]

        row['Score_Norm2'] = scoreNorm2.sqrt()
        row['Score_Sum'] = scoreSum

    # Best by score
    for name in ['Score_Norm2', 'Score_Sum']:
        metrics.sort(key=lambda x: x[name])
        print('--------------', name, '--------------')
        for i in range(kBest):
            print(metrics[i]['Algorithm'], metrics[i][name])
            algoNames.add(metrics[i]['Algorithm'])
        print()

    # All best
    print('-------------- Best algorithms --------------')
    print(*algoNames, sep='\n', end='\n\n')

    return metrics


def Main():
    # Init metrics
    subDir = input('Enter output sub dir name: ').strip()
    assert (len(subDir) > 0)

    dirPath = './stats_of_output/' + subDir + '/'
    assert (os.path.isdir(dirPath))

    metrics = GetStats(dirPath)
    metrics = CalculateBest(metrics, 1)

    metrics.sort(key=lambda x: x['Score_Norm2'])

    outKeys = ['Algorithm', 'Score_Norm2', 'Score_Sum'] + \
              list(map(lambda x: 'SIGMOID_STD_' + x, NAMES_IN_SCORE)) + \
              list(map(lambda x: 'STD_' + x, NAMES_IN_SCORE)) + \
              OUT_NAMES

    for i in range(len(metrics)):
        metrics[i] = {key: metrics[i][key] for key in outKeys}

    with open('./stats_of_output/' + subDir + '.csv', 'w', newline='') as fout:
        writer = csv.DictWriter(fout, outKeys)
        writer.writeheader()
        writer.writerows(metrics)


if __name__ == '__main__':
    Main()
