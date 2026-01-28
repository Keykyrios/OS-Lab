#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
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
    int start;
    int end;
};

bool compareAT(const Process& a, const Process& b) {
    if (a.at != b.at)
        return a.at < b.at;
    return a.pid < b.pid;
}

bool compareID(const Process& a, const Process& b) {
    return a.pid < b.pid;
}

int main() {
    int n, tq;
    cout << "Enter Number of processes: ";
    cin >> n;
    cout << "Enter Time Quantum: ";
    cin >> tq;

    vector<Process> p(n);
    for (int i = 0; i < n; i++) {
        p[i].pid = i + 1;
        cout << "Enter Arrival Time for P" << p[i].pid << ": ";
        cin >> p[i].at;
        cout << "Enter Burst Time for P" << p[i].pid << ": ";
        cin >> p[i].bt;
        p[i].rt = p[i].bt;
    }

    sort(p.begin(), p.end(), compareAT);

    queue<int> q;
    vector<bool> in_queue(n, false);
    vector<GanttUnit> history;
    
    int current_time = 0;
    int completed = 0;
    int idx = 0;

    if (p[0].at > 0) {
        history.push_back({-1, 0, p[0].at});
        current_time = p[0].at;
    }

    q.push(0);
    in_queue[0] = true;

    while (completed < n) {
        if (q.empty()) {
            int next_arrival_idx = -1;
            for(int i = 0; i < n; i++) {
                if(!in_queue[i] && p[i].rt > 0) {
                    next_arrival_idx = i;
                    break;
                }
            }
            
            if (next_arrival_idx != -1) {
                history.push_back({-1, current_time, p[next_arrival_idx].at});
                current_time = p[next_arrival_idx].at;
                q.push(next_arrival_idx);
                in_queue[next_arrival_idx] = true;
            }
        }

        int curr_idx = q.front();
        q.pop();

        int exec_time = min(tq, p[curr_idx].rt);
        int start_t = current_time;
        
        p[curr_idx].rt -= exec_time;
        current_time += exec_time;

        if (!history.empty() && history.back().pid == p[curr_idx].pid) {
            history.back().end = current_time;
        } else {
            history.push_back({p[curr_idx].pid, start_t, current_time});
        }

        for (int i = 0; i < n; i++) {
            if (p[i].at <= current_time && p[i].rt > 0 && !in_queue[i]) {
                q.push(i);
                in_queue[i] = true;
            }
        }

        if (p[curr_idx].rt > 0) {
            q.push(curr_idx);
        } else {
            p[curr_idx].ct = current_time;
            p[curr_idx].tat = p[curr_idx].ct - p[curr_idx].at;
            p[curr_idx].wt = p[curr_idx].tat - p[curr_idx].bt;
            if(p[curr_idx].wt < 0) p[curr_idx].wt = 0;
            completed++;
        }
    }

    sort(p.begin(), p.end(), compareID);

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
        
        cout << history[0].start;
        for (const auto& unit : history) {
            string timeStr = to_string(unit.start);
            string nextTimeStr = to_string(unit.end);
            int spaceNeed = 7 - nextTimeStr.length() + (nextTimeStr.length() - timeStr.length());
            
            if (unit.pid == history[0].pid && unit.start == history[0].start) {
                spaceNeed = 7 - nextTimeStr.length();
            }
            
            for(int k=0; k<spaceNeed; k++) cout << " ";
            cout << unit.end;
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
