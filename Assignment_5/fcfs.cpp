#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>

using namespace std;

struct Process {
    string id;
    int bt, at, ct, tat, wt;
};

bool compareArrival(Process a, Process b) {
    return a.at < b.at;
}

int main() {
    vector<Process> p = {
        {"P1", 10, 0}, {"P6", 13, 1}, {"P2", 5, 2}, {"P9", 2, 3},
        {"P3", 8, 4}, {"P4", 6, 6}, {"P8", 9, 7}, {"P7", 7, 9},
        {"P5", 10, 10}, {"P10", 8, 12}
    };

    sort(p.begin(), p.end(), compareArrival);

    int currentTime = 0;
    vector<pair<string, int>> gantt;

    for (int i = 0; i < p.size(); i++) {
        if (currentTime < p[i].at) {
            gantt.push_back({"IDLE", p[i].at});
            currentTime = p[i].at;
        }
        currentTime += p[i].bt;
        p[i].ct = currentTime;
        p[i].tat = p[i].ct - p[i].at;
        p[i].wt = p[i].tat - p[i].bt;
        gantt.push_back({p[i].id, currentTime});
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
    cout << "\nAverage Waiting Time: " << twt / 10 << " ms"; 
    cout << "\nAverage Turnaround Time: " << ttat / 10 << " ms\n"; 

    return 0;
}
