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


def Main():
    # Init metrics
    subDir = input('Enter output sub dir name: ').strip()
    assert(len(subDir) > 0)

    dirPath = './stats_of_output/' + subDir + '/'
    assert(os.path.isdir(dirPath))

    metrics = GetStats(dirPath)

    # Push metrics to sheets
    with open('./stats_of_output/' + subDir + '.csv', 'w', newline='') as fout:
        writer = csv.DictWriter(fout, metrics[0].keys())
        writer.writeheader()
        writer.writerows(metrics)


if __name__ == '__main__':
    Main()
