import csv
import os
from decimal import Decimal
import matplotlib.pyplot as plt


NAMES_IN_SCORE = ['100-AvgCpuUtilization', '100-AvgMemoryUtilization', '1-SNP',
                  'AvgPendingTask', 'Unfairness', 'SlowdownNorm2']

METRIC_GROUPS = [('G_Utilization', ['100-AvgCpuUtilization', '100-AvgMemoryUtilization'], 1),
                 ('G_ANP', ['1-SNP', 'Unfairness', 'SlowdownNorm2'], 1),
                 ('G_Pending', ['AvgPendingTask'], 1)]

OUT_NAMES = NAMES_IN_SCORE + ['AvgCpuUtilization', 'AvgMemoryUtilization', 'SNP']
OUT_NAMES += list(map(lambda x: x + '_STDDEV', OUT_NAMES))
OUT_NAMES += list(map(lambda x: x[0], METRIC_GROUPS))
OUT_NAMES += ['Score_scaled_sum', 'G_Score_scaled_sum']


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


def CalculateScore(metrics: list):
    for name in NAMES_IN_SCORE:
        minValue = Decimal(min([row[name] for row in metrics]))
        for row in metrics:
            row[name + '_scaled'] = row[name] - minValue

        maxValue = Decimal(max([row[name + '_scaled'] for row in metrics]))
        for row in metrics:
            row[name + '_scaled'] = row[name + '_scaled'] / maxValue

    for row in metrics:
        scoreScaledSum = Decimal(0)
        for name in NAMES_IN_SCORE:
            scoreScaledSum += row[name + '_scaled']

        row['Score_scaled_sum'] = scoreScaledSum

    for row in metrics:
        for group in METRIC_GROUPS:
            g_name = group[0]
            row[g_name] = Decimal(0)
            for name in group[1]:
                row[g_name] += row[name + '_scaled']
            row[g_name] /= Decimal(len(group[1]))

    for row in metrics:
        g_ScoreScaledSum = Decimal(0)
        for group in METRIC_GROUPS:
            g_name = group[0]
            alpha = Decimal(group[2])

            g_ScoreScaledSum += alpha * row[g_name]

        row['G_Score_scaled_sum'] = g_ScoreScaledSum

    return metrics


def Main():
    # Init metrics
    subDir = input('Enter output sub dir name: ').strip()
    assert (len(subDir) > 0)

    dirPath = './stats_of_output/' + subDir + '/'
    assert (os.path.isdir(dirPath))

    filterWithMakeSpan = int(input('Enter 1 to filter with MakeSpan, 0 otherwise: '))

    metrics = GetStats(dirPath)

    if filterWithMakeSpan == 1:
        maxMakeSpan = max([x['MakeSpan'] for x in metrics])
        metrics = list(filter(lambda x: x['MakeSpan'] == maxMakeSpan, metrics))

    metrics = CalculateScore(metrics)

    outKeys = ['Algorithm'] + list(reversed(OUT_NAMES))

    plt.plot(list(sorted([row['Unfairness_scaled'] for row in metrics])))
    plt.plot(list(sorted([row['1-SNP_scaled'] for row in metrics])))
    plt.plot(list(sorted([row['SlowdownNorm2_scaled'] for row in metrics])))
    plt.plot(list(sorted([row['100-AvgCpuUtilization_scaled'] for row in metrics])))
    plt.plot(list(sorted([row['100-AvgMemoryUtilization_scaled'] for row in metrics])))
    plt.plot(list(sorted([row['AvgPendingTask_scaled'] for row in metrics])))
    plt.show()

    plt.plot(list(sorted([row['Score_scaled_sum'] for row in metrics])))
    plt.plot(list(sorted([row['G_Score_scaled_sum'] for row in metrics])))
    plt.show()

    for i in range(len(metrics)):
        metrics[i] = {key: metrics[i][key] for key in outKeys}

    with open('./stats_of_output/' + subDir + '.csv', 'w', newline='') as fout:
        writer = csv.DictWriter(fout, outKeys)
        writer.writeheader()
        writer.writerows(metrics)


if __name__ == '__main__':
    Main()
