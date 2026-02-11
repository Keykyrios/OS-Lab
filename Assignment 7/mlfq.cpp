#include <iostream>
#include <vector>
#include <deque>
#include <algorithm>
#include <iomanip>

using namespace std;

struct Process {
    int id;
    int arrivalTime;
    int burstTime;
    int remainingTime;
    int completionTime;
    int waitingTime;
    int turnAroundTime;
    int currentQueue;
    int timeInCurrentQueue;
    int waitTimeInQueue;

    Process(int pid, int at, int bt)
        : id(pid), arrivalTime(at), burstTime(bt), remainingTime(bt),
          completionTime(0), waitingTime(0), turnAroundTime(0),
          currentQueue(1), timeInCurrentQueue(0), waitTimeInQueue(0) {}
};

struct GanttSlice {
    int processId;
    int startTime;
    int endTime;
};

const int Q1_QUANTUM = 4;
const int Q2_QUANTUM = 8;
const int AGING_THRESHOLD = 20;

void checkAging(deque<int>& q, int currentQueueLevel, vector<Process>& processes, deque<int>& higherQ) {
    auto it = q.begin();
    while (it != q.end()) {
        if (processes[*it].waitTimeInQueue > AGING_THRESHOLD) {
            int pid = *it;
            processes[pid].currentQueue = currentQueueLevel - 1;
            processes[pid].timeInCurrentQueue = 0;
            processes[pid].waitTimeInQueue = 0;
            higherQ.push_back(pid);
            it = q.erase(it);
        } else {
            ++it;
        }
    }
}

int main() {
    int n;
    cout << "Enter number of processes: ";
    if (!(cin >> n)) return 0;

    vector<Process> processes;
    for (int i = 0; i < n; i++) {
        int id, at, bt;
        cout << "Enter Process ID, Arrival Time, Burst Time for process " << i + 1 << ": ";
        cin >> id >> at >> bt;
        processes.emplace_back(id, at, bt);
    }

    deque<int> q1, q2, q3;
    vector<GanttSlice> ganttHistory;

    int currentTime = 0;
    int completedCount = 0;

    vector<int> arrivalOrder(n);
    for(int i=0; i<n; i++) arrivalOrder[i] = i;
    sort(arrivalOrder.begin(), arrivalOrder.end(), [&](int a, int b) {
        return processes[a].arrivalTime < processes[b].arrivalTime;
    });
    int arrivalPtr = 0;

    while (completedCount < n) {
        while (arrivalPtr < n && processes[arrivalOrder[arrivalPtr]].arrivalTime <= currentTime) {
            int pidIdx = arrivalOrder[arrivalPtr];
            q1.push_back(pidIdx);
            processes[pidIdx].currentQueue = 1;
            arrivalPtr++;
        }

        for (int idx : q1) processes[idx].waitTimeInQueue = 0;
        for (int idx : q2) processes[idx].waitTimeInQueue++;
        for (int idx : q3) processes[idx].waitTimeInQueue++;

        checkAging(q2, 2, processes, q1);
        checkAging(q3, 3, processes, q2);

        int runningProcessIdx = -1;
        if (!q1.empty()) runningProcessIdx = q1.front();
        else if (!q2.empty()) runningProcessIdx = q2.front();
        else if (!q3.empty()) runningProcessIdx = q3.front();

        if (runningProcessIdx != -1) {
            Process& p = processes[runningProcessIdx];

            if (ganttHistory.empty() || ganttHistory.back().processId != p.id) {
                ganttHistory.push_back({p.id, currentTime, currentTime + 1});
            } else {
                ganttHistory.back().endTime++;
            }

            p.remainingTime--;
            p.timeInCurrentQueue++;
            p.waitTimeInQueue = 0;
            currentTime++;

            if (p.remainingTime == 0) {
                p.completionTime = currentTime;
                p.turnAroundTime = p.completionTime - p.arrivalTime;
                p.waitingTime = p.turnAroundTime - p.burstTime;
                completedCount++;

                if (p.currentQueue == 1) q1.pop_front();
                else if (p.currentQueue == 2) q2.pop_front();
                else q3.pop_front();
            } else {
                if (p.currentQueue == 1 && p.timeInCurrentQueue >= Q1_QUANTUM) {
                    q1.pop_front();
                    q2.push_back(runningProcessIdx);
                    p.currentQueue = 2;
                    p.timeInCurrentQueue = 0;
                } else if (p.currentQueue == 2 && p.timeInCurrentQueue >= Q2_QUANTUM) {
                    q2.pop_front();
                    q3.push_back(runningProcessIdx);
                    p.currentQueue = 3;
                    p.timeInCurrentQueue = 0;
                }
            }
        } else {
            if (ganttHistory.empty() || ganttHistory.back().processId != -1) {
                ganttHistory.push_back({-1, currentTime, currentTime + 1});
            } else {
                ganttHistory.back().endTime++;
            }
            currentTime++;
        }
    }

    sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
        return a.id < b.id;
    });

    cout << "\nPID\tAT\tBT\tCT\tTAT\tWT\n";
    cout << string(50, '-') << "\n";
    double totalTAT = 0, totalWT = 0;
    for (const auto& p : processes) {
        cout << "P" << p.id << "\t" << p.arrivalTime << "\t" << p.burstTime << "\t"
             << p.completionTime << "\t" << p.turnAroundTime << "\t" << p.waitingTime << "\n";
        totalTAT += p.turnAroundTime;
        totalWT += p.waitingTime;
    }
    cout << string(50, '-') << "\n";
    cout << fixed << setprecision(2);
    cout << "Average Turnaround Time: " << totalTAT / n << " ms" << endl;
    cout << "Average Waiting Time:    " << totalWT / n << " ms" << endl;

    cout << "\n--- Gantt Chart ---\n|";
    for (const auto& slice : ganttHistory) {
        if (slice.processId == -1) cout << " IDLE |";
        else cout << "  P" << slice.processId << "  |";
    }
    cout << "\n0";
    for (const auto& slice : ganttHistory) cout << setw(7) << slice.endTime;
    cout << "\n";

    return 0;
}
