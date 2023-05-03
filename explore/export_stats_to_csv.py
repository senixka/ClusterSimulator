import csv
import os


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
        algoName = (filePath.split('/')[-1])[:-len('_stat.txt')]
        metrics.append({'Algorithm': algoName})

        with open(filePath, 'r') as fin:
            while True:
                name = fin.readline().strip()
                value = fin.readline().strip()

                if len(name) == 0:
                    break

                metrics[-1][name] = NumberType(value)

    return metrics


def PrintBest(metrics: list, kBest: int):
    metrics = metrics.copy()
    namesASC = ['AvgPendingTask', 'Unfairness', 'SlowdownNorm2', 'UnfinishedJobCounter', 'PendingTaskCounter']
    namesDESC = ['AvgCpuUtilization', 'AvgMemoryUtilization', 'AvgWorkingTask', 'SNP', 'TaskFinishedCounter']

    maxMakeSpan = max([x['MakeSpan'] for x in metrics])
    metrics = list(filter(lambda x: x['MakeSpan'] == maxMakeSpan, metrics))
    algoNames = set()

    for name in namesASC + namesDESC:
        metrics.sort(key=lambda x: x[name], reverse=(name in namesDESC))
        print('--------------', name, '--------------')
        for i in range(kBest):
            print(metrics[i]['Algorithm'], metrics[i][name])
            algoNames.add(metrics[i]['Algorithm'])
        print()

    print('-------------- Best algorithms --------------')
    print(*algoNames, sep='\n')


def Main():
    # Init metrics
    subDir = input('Enter output sub dir name: ').strip()
    assert(len(subDir) > 0)

    dirPath = './stats_of_output/' + subDir + '/'
    assert(os.path.isdir(dirPath))

    metrics = GetStats(dirPath)

    # Push metrics to csv
    with open('./stats_of_output/' + subDir + '.csv', 'w', newline='') as fout:
        writer = csv.DictWriter(fout, metrics[0].keys())
        writer.writeheader()
        writer.writerows(metrics)

    PrintBest(metrics, 10)


if __name__ == '__main__':
    Main()
