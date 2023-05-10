import matplotlib.pyplot as plt
import matplotlib
import numpy as np
import seaborn as sns


matplotlib.use('Agg')
plt.ioff()

UINT32_MAX = 4294967295

CPU_BUCKET_CNT, MEM_BUCKET_CNT = 15, 15
MAX_CPU_VALUE, MAX_MEM_VALUE = 1000, 1000

CPU_STEP = (MAX_CPU_VALUE + CPU_BUCKET_CNT - 1) // CPU_BUCKET_CNT
MEM_STEP = (MAX_MEM_VALUE + MEM_BUCKET_CNT - 1) // MEM_BUCKET_CNT


def BoundedSum(a, b):
    if a + b > UINT32_MAX:
        return UINT32_MAX
    return a + b


def TaskWorkloadInClusterInput():
    # ///////////////////////// Prepare Events /////////////////////////

    with open("../simulator/input/job_and_task.txt", "r") as fin:
        jobN, totalTask = map(int, fin.readline().split())
        fin.readline()

        taskCounter = 0
        events = []

        for i in range(jobN):
            jobTime, entryCount = map(int, fin.readline().split())

            for j in range(entryCount):
                taskCount, taskEstimate, taskCpu, taskMem = map(int, fin.readline().split())

                taskCounter += taskCount

                for k in range(taskCount):
                    events.append((jobTime, False, taskCpu, taskMem))
                    events.append((BoundedSum(jobTime, taskEstimate), True, taskCpu, taskMem))

            fin.readline()

        assert(taskCounter == totalTask)
        assert(taskCounter * 2 == len(events))

    events.sort()
    print('Events size:', len(events))

    # ///////////////////////// Stat Task /////////////////////////

    Z = np.zeros((CPU_BUCKET_CNT, MEM_BUCKET_CNT))
    currentCpu, currentMem, currentTaskCounter = 0, 0, 0
    statCpu, statMem, statTime, statTaskCounter = [], [], [], []
    updateStatEach, nextUpdateTime = 10, 0

    for time, isFinished, cpu, mem in events:
        if isFinished:
            currentTaskCounter -= 1
            currentCpu -= cpu
            currentMem -= mem
        else:
            currentTaskCounter += 1
            currentCpu += cpu
            currentMem += mem

            Z[cpu // CPU_STEP][mem // MEM_STEP] += 1

        if time == UINT32_MAX:
            break

        while time > nextUpdateTime:
            statTime.append(nextUpdateTime)
            statTaskCounter.append(currentTaskCounter)
            statCpu.append(currentCpu)
            statMem.append(currentMem)

            nextUpdateTime += updateStatEach

    Z.astype(int)
    FirstD = np.zeros((CPU_BUCKET_CNT, MEM_BUCKET_CNT))

    for i in range(CPU_BUCKET_CNT):
        for j in range(MEM_BUCKET_CNT):
            if int(Z[i, j]) != 0:
                FirstD[i, j] = str(int(Z[i, j]))[0]
                Z[i, j] = len(str(int(Z[i, j])))

    # ///////////////////////// Task distribution ////////////////////

    xTicks = list(range(CPU_STEP // 2, MAX_CPU_VALUE + 1, CPU_STEP))
    yTicks = list(range(MEM_STEP // 2, MAX_MEM_VALUE + 1, MEM_STEP))
    xTickLabels = list(map(str, xTicks))
    yTickLabels = list(map(str, yTicks))

    fig, ax = plt.subplots(figsize=(7, 7))
    ax.imshow(Z, cmap="YlGn")

    ax.set_xticks(np.arange(len(xTicks)), labels=xTickLabels)
    ax.set_yticks(np.arange(len(yTicks)), labels=yTickLabels)

    for i in range(len(xTicks)):
        for j in range(len(yTicks)):
            if int(Z[i, j]) != 0:
                ax.text(j, i, str(int(FirstD[i, j])) + 'e' + str(int(Z[i, j]) - 1), ha="center", va="center", color="red")
            else:
                ax.text(j, i, 0, ha="center", va="center", color="red")

    ax.set_title("Task count (in cpu/memory demand)")
    ax.set_xlabel("Memory")
    ax.set_ylabel("Cpu")
    fig.tight_layout()

    plt.savefig("./plots_of_input/input_task_distribution.png")
    plt.close()

    # ///////////////////////// Stat Machines /////////////////////////
    sns.set_theme()

    sumCpu, sumMem = 0, 0
    with open("../simulator/input/machine_orig.txt", "r") as fin:
        totalMachine, machineN = map(int, fin.readline().split())
        machines = {}
        machineCnt = 0

        for _ in range(machineN):
            cnt, cpu, mem = map(int, fin.readline().split())
            sumCpu += cpu * cnt
            sumMem += mem * cnt
            machineCnt += cnt

            if (cpu, mem) not in machines:
                machines[(cpu, mem)] = cnt
            else:
                machines[(cpu, mem)] += cnt

        assert(machineCnt == totalMachine)

    # ///////////////////////// Task working /////////////////////////

    plt.figure(figsize=(8, 5))
    plt.title("Task working in Time")
    plt.xlabel("Time (in microseconds)")
    plt.ylabel("Count")

    plt.plot(statTime, statTaskCounter, label="Working task")

    plt.legend()
    plt.tight_layout()
    plt.savefig("./plots_of_input/input_working_task.png")
    plt.close()

    # //////////////////////////// Resource demand ///////////////////

    plt.figure(figsize=(8, 5))
    plt.title("Task resource demand in Time")
    plt.xlabel("Time (in seconds)")
    plt.ylabel("Unit")

    plt.plot(statTime, statCpu, label="Task CPU")
    plt.plot(statTime, np.full_like(statTime, sumCpu), label="Machine CPU")
    plt.plot(statTime, statMem, label="Task Memory")
    plt.plot(statTime, np.full_like(statTime, sumMem), label="Machine Memory")

    plt.legend()
    plt.tight_layout()
    plt.savefig("./plots_of_input/input_task_resources.png")
    plt.close()

    # //////////////////////////// Recourse ratio ////////////////////

    plt.figure(figsize=(8, 5))
    plt.title("Cpu / Memory")
    plt.xlabel("Time (in seconds)")
    plt.ylabel("Ratio")

    ratio = [statCpu[i] / statMem[i] for i in range(len(statCpu))]
    plt.plot(statTime, ratio, label="Ratio")

    plt.legend()
    plt.tight_layout()
    plt.savefig("./plots_of_input/input_ratio_cpu_memory.png")
    plt.close()


if __name__ == "__main__":
    TaskWorkloadInClusterInput()
