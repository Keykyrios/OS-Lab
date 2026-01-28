#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <iomanip>
#include <string>

using namespace std;

struct Process {
    int pid;
    int at;
    int bt;
    int rt;
    int ct;
    int tat;
    int wt;
};

struct GanttUnit {
    int pid;
    int start_time;
    int end_time;
};

int main() {
    int n;
    cout << "Enter Number of processes: ";
    cin >> n;

    vector<Process> p(n);
    for (int i = 0; i < n; i++) {
        p[i].pid = i + 1;
        cout << "Enter Arrival Time for P" << p[i].pid << ": ";
        cin >> p[i].at;
        cout << "Enter Burst Time for P" << p[i].pid << ": ";
        cin >> p[i].bt;
        p[i].rt = p[i].bt;
    }

    int completed = 0;
    int current_time = 0;
    int min_rt = INT_MAX;
    int shortest = -1;
    bool check = false;
    
    vector<GanttUnit> history;

    while (completed != n) {
        for (int i = 0; i < n; i++) {
            if ((p[i].at <= current_time) && (p[i].rt < min_rt) && p[i].rt > 0) {
                min_rt = p[i].rt;
                shortest = i;
                check = true;
            }
        }

        if (!check) {
            if (!history.empty() && history.back().pid == -1) {
                history.back().end_time = current_time + 1;
            } else {
                history.push_back({-1, current_time, current_time + 1});
            }
            current_time++;
            continue;
        }

        if (history.empty() || history.back().pid != p[shortest].pid) {
            history.push_back({p[shortest].pid, current_time, current_time + 1});
        } else {
            history.back().end_time = current_time + 1;
        }

        p[shortest].rt--;
        min_rt = p[shortest].rt;
        
        if (min_rt == 0) {
            min_rt = INT_MAX;
            completed++;
            check = false;
            p[shortest].ct = current_time + 1;
            p[shortest].tat = p[shortest].ct - p[shortest].at;
            p[shortest].wt = p[shortest].tat - p[shortest].bt;
            if (p[shortest].wt < 0) p[shortest].wt = 0;
        }
        current_time++;
    }

    cout << "\n--- Gantt Chart ---\n";
    
    if (!history.empty()) {
        cout << "+";
        for (const auto& unit : history) {
            cout << string(6, '-') << "+";
        }
        cout << "\n|";
        
        for (const auto& unit : history) {
            if (unit.pid == -1) cout << " IDLE |";
            else cout << "  P" << unit.pid << (unit.pid < 10 ? "  " : " ") << "|";
        }
        cout << "\n+";
        
        for (const auto& unit : history) {
            cout << string(6, '-') << "+";
        }
        cout << "\n";
        
        cout << history[0].start_time;
        for (const auto& unit : history) {
            string timeStr = to_string(unit.start_time);
            string nextTimeStr = to_string(unit.end_time);
            int spaceNeed = 7 - nextTimeStr.length() + (nextTimeStr.length() - timeStr.length());
            
            if (unit.pid == history[0].pid && unit.start_time == history[0].start_time) {
                spaceNeed = 7 - nextTimeStr.length();
            }
            
            for(int k=0; k<spaceNeed; k++) cout << " ";
            cout << unit.end_time;
        }
        cout << "\n";
    }

    double total_wt = 0, total_tat = 0;
    cout << "\nPID\tAT\tBT\tCT\tTAT\tWT\n";
    cout << string(50, '-') << "\n";
    
    for (int i = 0; i < n; i++) {
        total_wt += p[i].wt;
        total_tat += p[i].tat;
        cout << "P" << p[i].pid << "\t" 
             << p[i].at << "\t" 
             << p[i].bt << "\t" 
             << p[i].ct << "\t" 
             << p[i].tat << "\t" 
             << p[i].wt << "\n";
    }

    cout << "\nAverage Waiting Time: " << fixed << setprecision(2) << (total_wt / n) << "\n";
    cout << "Average Turnaround Time: " << fixed << setprecision(2) << (total_tat / n) << "\n";

    return 0;
}
