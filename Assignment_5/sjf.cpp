#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>

using namespace std;

struct Process {
    string id;
    int bt, at, ct, tat, wt;
    bool done = false;
};

int main() {
    vector<Process> p = {
        {"P1", 10, 0}, {"P2", 5, 2}, {"P3", 8, 4}, {"P4", 6, 6},
        {"P5", 10, 10}, {"P6", 13, 1}, {"P7", 7, 9}, {"P8", 9, 7},
        {"P9", 2, 3}, {"P10", 8, 12}
    };

    int n = p.size(), completed = 0, currTime = 0;
    vector<pair<string, int>> gantt;

    while (completed < n) {
        int idx = -1, minBT = 1e9;
        for (int i = 0; i < n; i++) {
            if (p[i].at <= currTime && !p[i].done && p[i].bt < minBT) {
                minBT = p[i].bt;
                idx = i;
            }
        }

        if (idx != -1) {
            currTime += p[idx].bt;
            p[idx].ct = currTime;
            p[idx].tat = p[idx].ct - p[idx].at;
            p[idx].wt = p[idx].tat - p[idx].bt;
            p[idx].done = true;
            completed++;
            gantt.push_back({p[idx].id, currTime});
        } else {
            currTime++;
            if (gantt.empty() || gantt.back().first != "IDLE")
                gantt.push_back({"IDLE", currTime});
            else gantt.back().second = currTime;
        }
    }

    cout << "\n--- GANTT CHART ---\n ";
    for (auto g : gantt) cout << "-------";
    cout << "\n|";
    for (auto g : gantt) cout << setw(4) << g.first << " |";
    cout << "\n ";
    for (auto g : gantt) cout << "-------";
    cout << "\n0";
    for (auto g : gantt) cout << setw(7) << g.second;
    cout << "\n\n";

    float twt = 0, ttat = 0;
    cout << "PID\tAT\tBT\tCT\tTAT\tWT\n";
    for (auto proc : p) {
        twt += proc.wt; ttat += proc.tat;
        cout << proc.id << "\t" << proc.at << "\t" << proc.bt << "\t" 
             << proc.ct << "\t" << proc.tat << "\t" << proc.wt << endl;
    }
    cout << fixed << setprecision(2);
    cout << "\nAverage Waiting Time: " << twt / n << " ms"; 
    cout << "\nAverage Turnaround Time: " << ttat / n << " ms\n"; 

    return 0;
}
