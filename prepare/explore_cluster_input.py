import matplotlib.pyplot as plt
import numpy as np

UINT64_MAX = 18446744073709551615


def BoundedSum(a, b):
    if a + b > UINT64_MAX:
        return UINT64_MAX
    return a + b


def TaskWorkloadInClusterInput():
    # ///////////////////////// Prepare Events /////////////////////////

    with open("../simulator/input/job_and_task.txt", "r") as fin:
        jobN = int(fin.readline())
        fin.readline()

        taskCounter = 0
        events = []

        for i in range(jobN):
            jobSize, jobTime = map(int, fin.readline().split())
            nTask = int(fin.readline())
            taskCounter += nTask

            for j in range(nTask):
                line = fin.readline().split()
                taskIndex, taskEstimate = map(int, line[:2])
                taskCpu, taskMem, taskDisk = map(float, line[2:])

                events.append((jobTime, False, taskCpu, taskMem, taskDisk))
                events.append((BoundedSum(jobTime, taskEstimate), True, taskCpu, taskMem, taskDisk))

            fin.readline()

    assert (taskCounter * 2 == len(events))
    events.sort()
    print('Events size:', len(events))

    # ///////////////////////// Stat Task /////////////////////////

    taskDist = {}
    currentCpu, currentMem, currentDisk, currentTaskCounter = 0, 0, 0, 0
    statCpu, statMem, statDisk, statTime, statTaskCounter = [], [], [], [], []
    updateStatEach, nextUpdateTime = 1_000_000, 0

    for time, isFinished, cpu, mem, disk in events:
        if isFinished:
            currentTaskCounter -= 1

            currentCpu -= cpu
            currentMem -= mem
            currentDisk -= disk
        else:
            currentTaskCounter += 1

            currentCpu += cpu
            currentMem += mem
            currentDisk += disk

            cpuBucket = round(cpu, 1)
            memBucket = round(mem, 1)

            if (cpuBucket, memBucket) not in taskDist:
                taskDist[(cpuBucket, memBucket)] = 1
            else:
                taskDist[(cpuBucket, memBucket)] += 1

        if time == UINT64_MAX:
            break

        while time > nextUpdateTime:
            statTime.append(nextUpdateTime)
            statTaskCounter.append(currentTaskCounter)
            statCpu.append(currentCpu)
            statMem.append(currentMem)
            statDisk.append(currentDisk)

            nextUpdateTime += updateStatEach

    # ///////////////////////// Stat Machines /////////////////////////

    with open("../simulator/input/machine.txt", "r") as fin:
        machineN = int(fin.readline())
        machines = {}

        for i in range(machineN):
            cpu, mem, disk = map(float, fin.readline().split())
            if (cpu, mem, disk) not in machines:
                machines[(cpu, mem, disk)] = 1
            else:
                machines[(cpu, mem, disk)] += 1

    # ///////////////////////// Machines /////////////////////////

    sumCpu, sumMem, sumDisk = 0, 0, 0

    print("count\tcpu\tmem\tdisk")
    for key, value in machines.items():
        print(f"{value}\t{key[0]}\t{key[1]}\t{key[2]}")
        sumCpu += value * key[0]
        sumMem += value * key[1]
        sumDisk += value * key[2]

    # ///////////////////////// Task distribution ////////////////////

    xTicks = np.arange(0, 1.001, 0.1)
    yTicks = np.arange(0, 1.001, 0.1)
    xTickLabels = ['{:.1f}'.format(x) for x in xTicks]
    yTickLabels = ['{:.1f}'.format(y) for y in yTicks]

    Z = np.zeros((len(xTicks), len(yTicks)))
    for i, cpu in enumerate(xTicks):
        for j, mem in enumerate(yTicks):
            if (cpu, mem) in taskDist:
                Z[i, j] = int(taskDist[(cpu, mem)])

    Z.astype(int)

    fig, ax = plt.subplots(figsize=(7, 7))
    ax.imshow(Z, cmap="YlGn")

    ax.set_xticks(np.arange(len(xTicks)), labels=xTickLabels)
    ax.set_yticks(np.arange(len(yTicks)), labels=yTickLabels)

    for i in range(len(xTicks)):
        for j in range(len(yTicks)):
            ax.text(j, i, int(Z[i, j]), ha="center", va="center", color="red", size="x-small")

    ax.set_title("Task count (in cpu/memory demand)")
    ax.set_xlabel("Memory")
    ax.set_ylabel("Cpu")
    fig.tight_layout()

    plt.savefig("./stat/input_task_distribution.png")
    plt.show()

    # ///////////////////////// Task working /////////////////////////

    plt.figure(figsize=(12, 5))
    plt.title("Task Pending vs Working in Time")
    plt.xlabel("Time (in microseconds)")
    plt.ylabel("Count")

    plt.plot(statTime, statTaskCounter, label="Working task")

    plt.legend()
    plt.savefig("./stat/input_working_task.png")
    plt.show()

    # //////////////////////////// Resource demand ///////////////////

    plt.figure(figsize=(12, 5))
    plt.title("Task resource demand in Time")
    plt.xlabel("Time (in microseconds)")
    plt.ylabel("Unit")

    plt.plot(statTime, statCpu, label="Task CPU")
    plt.plot(statTime, np.full_like(statTime, sumCpu), label="Machine CPU")
    plt.plot(statTime, statMem, label="Task Memory")
    plt.plot(statTime, np.full_like(statTime, sumMem), label="Machine Memory")
    plt.plot(statTime, statDisk, label="Task Disk")
    plt.plot(statTime, np.full_like(statTime, sumDisk), label="Machine Disk")

    plt.legend()
    plt.savefig("./stat/input_task_resources.png")
    plt.show()

    # //////////////////////////// Recourse ratio ////////////////////

    plt.figure(figsize=(12, 5))
    plt.title("Cpu / Memory")
    plt.xlabel("Time (in microseconds)")
    plt.ylabel("Ratio")

    ratio = [statCpu[i] / statMem[i] for i in range(len(statCpu))]
    plt.plot(statTime, ratio, label="Ratio")

    plt.legend()
    plt.savefig("./stat/input_ratio_cpu_memory.png")
    plt.show()


if __name__ == "__main__":
    TaskWorkloadInClusterInput()
