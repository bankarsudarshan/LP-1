#include <bits/stdc++.h>
using namespace std;

class Process
{
public:
    string name;
    int pid, at, bt, ct, tat, wt, priority;

    Process()
    {
        name = "";
        pid = at = bt = ct = tat = wt = priority = 0;
    }

    void create()
    {
        cout << "Enter process name: ";
        cin >> name;
        cout << "Enter Arrival Time: ";
        cin >> at;
        cout << "Enter Burst Time: ";
        cin >> bt;
    }
};

class Scheduler
{
public:
    vector<Process> process;
    int n;

    Scheduler()
    {
        cout << "Enter the number of processes: ";
        cin >> n;
        process.resize(n);
        for (int i = 0; i < n; i++)
        {
            cout << "For process " << i + 1 << " : " << endl;
            process[i].create();
            process[i].pid = i;
        }
    }

    int getId(Process p)
    {
        for (int i = 0; i < n; i++)
        {
            if (process[i].name == p.name)
                return i;
        }
    }

    void display(vector<Process> &process, int n)
    {
        cout << "Process\tAT\tBT\tCT\tTAT\tWT" << endl;
        for (int i = 0; i < n; i++)
        {
            cout << process[i].name << "\t" << process[i].at << "\t"
                 << process[i].bt << "\t" << process[i].ct << "\t"
                 << process[i].tat << "\t" << process[i].wt << endl;
        }
    }

    void average(float avg_tat, float avg_wt)
    {
        cout << "\nAverage Turnaround Time: " << avg_tat
             << "\nAverage Waiting Time: " << avg_wt << endl;
    }

    void FCFS()
    {
        vector<Process> temp(process.begin(), process.end());
        int compl_time = 0;

        sort(temp.begin(), temp.end(), [&](Process &a, Process &b)
             { return a.at < b.at; });

        vector<Process> gantt(temp.begin(), temp.end());

        for (int i = 0; i < n; i++)
        {
            compl_time = max(compl_time, temp[i].at) + temp[i].bt;
            gantt[i].ct = compl_time;
            gantt[i].tat = gantt[i].ct - gantt[i].at;
            gantt[i].wt = gantt[i].tat - gantt[i].bt;
        }

        float avg_tat = 0, avg_wt = 0;
        for (int i = 0; i < n; i++)
        {
            avg_tat += gantt[i].tat;
            avg_wt += gantt[i].wt;
        }

        avg_tat /= n;
        avg_wt /= n;

        display(gantt, n);
        average(avg_tat, avg_wt);
    }

    void SJF()
    {
        vector<Process> temp(process.begin(), process.end());
        vector<Process> readyQ;

        int totalTime = 0;
        for (int i = 0; i < n; i++)
        {
            totalTime += temp[i].bt;
        }

        sort(temp.begin(), temp.end(), [&](Process &a, Process &b)
             { return a.at < b.at; });

        int timestamp = 0;

        int idx = 0;
        while (timestamp < totalTime)
        {
            // Add all processes that have arrived by the current timestamp to the ready queue
            while (idx < n && temp[idx].at <= timestamp)
            {
                readyQ.push_back(temp[idx]);
                idx++;
            }

            // Sort the ready queue by remaining burst time (bt) to select the shortest job
            sort(readyQ.begin(), readyQ.end(), [&](Process &a, Process &b)
                 { return a.bt < b.bt; });

            if (!readyQ.empty())
            {
                // Process the shortest job at the front of readyQ
                readyQ[0].bt--;

                // Check if the process has completed execution
                if (readyQ[0].bt == 0)
                {
                    for (int i = 0; i < n; i++)
                    {
                        if (process[i].name == readyQ[0].name)
                        {
                            process[i].ct = timestamp + 1;
                            process[i].tat = process[i].ct - process[i].at;
                            process[i].wt = process[i].tat - process[i].bt; // Use original burst time
                        }
                    }

                    // Remove the completed process from the ready queue
                    readyQ.erase(readyQ.begin());
                }
            }

            timestamp++;
        }

        float avg_tat = 0, avg_wt = 0;
        for (int i = 0; i < n; i++)
        {
            avg_tat += process[i].tat;
            avg_wt += process[i].wt;
        }

        avg_tat /= n;
        avg_wt /= n;

        display(process, n);
        average(avg_tat, avg_wt);
    }

    void RoundRobin(int tq)
    {
        vector<Process> temp(process.begin(), process.end()); // Copy of processes

        vector<Process> gantt = temp;
        queue<int> readyQ;
        vector<bool> inQueue(n, false); // Track whether a process is already in the ready queue

        int curr_time = 0;
        int comp_processes = 0;

        // Add processes to the ready queue if they have arrived and are not already in the queue
        for (int i = 0; i < n; i++)
        {
            if (temp[i].bt > 0 && temp[i].at <= curr_time && !inQueue[i])
            {
                readyQ.push(i);
                inQueue[i] = true;
            }
        }

        while (comp_processes < n) {
            if (readyQ.empty())
            {
                // Increment time if no process is ready yet
                curr_time++;
                continue;
            }

            int idx = readyQ.front();
            readyQ.pop();
            inQueue[idx] = false; // Mark the process as not in the queue after popping it

            if (temp[idx].bt > tq)
            {
                // Execute process for the time quantum
                curr_time += tq;
                temp[idx].bt -= tq;

                // Add the process back to the queue if it has remaining burst time
                for (int j = 0; j < n; j++)
                {
                    if (temp[j].bt > 0 && temp[j].at <= curr_time && !inQueue[j] && j != idx)
                    {
                        readyQ.push(j);
                        inQueue[j] = true;
                    }
                }

                readyQ.push(idx); // Re-add current process to the queue
                inQueue[idx] = true;
            }
            else
            {
                // Process completes within this time slice
                curr_time += temp[idx].bt;
                temp[idx].bt = 0;
                temp[idx].ct = curr_time;
                comp_processes++;
            }
        }

        float avg_tat = 0, avg_wt = 0;

        // Calculate turnaround time (TAT) and waiting time (WT)
        for (int i = 0; i < n; i++)
        {
            gantt[i].tat = gantt[i].ct - gantt[i].at;
            gantt[i].wt = gantt[i].tat - gantt[i].bt; // Use original burst time here
            avg_tat += gantt[i].tat;
            avg_wt += gantt[i].wt;
        }

        avg_tat /= n;
        avg_wt /= n;

        display(gantt, n);
        average(avg_tat, avg_wt);
    }

    void Priority()
    {
        // Create a copy of the process list
        vector<Process> temp(process.begin(), process.end());

        // Sort processes based on arrival time first
        sort(temp.begin(), temp.end(), [](const Process &a, const Process &b)
             {
                 return a.at < b.at; // Sort by arrival time
             });

        int current_time = 0;     // To track the current time
        vector<Process> gantt(n); // Gantt chart to store process completion data

        // Process execution and calculation
        for (int i = 0; i < n; i++)
        {
            // Sort by priority after arrival time
            sort(temp.begin(), temp.end(), [&](const Process &a, const Process &b)
                 {
                     if (a.at <= current_time && b.at <= current_time)
                     {
                         if (a.priority == b.priority)
                         {
                             return a.at < b.at; // If priority is the same, sort by arrival time
                         }
                         return a.priority < b.priority; // Sort by priority
                     }
                     return a.at < b.at; // Ensure we consider only processes that have arrived
                 });

            gantt[i] = temp[0];                                          // Select the process with the highest priority
            current_time = max(current_time, gantt[i].at) + gantt[i].bt; // Process execution starts after arrival, if needed
            gantt[i].ct = current_time;
            gantt[i].tat = gantt[i].ct - gantt[i].at; // Turnaround Time = Completion Time - Arrival Time
            gantt[i].wt = gantt[i].tat - gantt[i].bt; // Waiting Time = Turnaround Time - Burst Time

            // Remove the selected process from the list (completed)
            temp.erase(temp.begin());
        }

        // Calculate average Turnaround Time and Waiting Time
        float avg_tat = 0, avg_wt = 0;
        for (int i = 0; i < n; i++)
        {
            avg_tat += gantt[i].tat;
            avg_wt += gantt[i].wt;
        }

        avg_tat /= n; // Average Turnaround Time
        avg_wt /= n;  // Average Waiting Time

        // Display the results
        display(gantt, n);
        average(avg_tat, avg_wt);
    }
};

int main()
{
    Scheduler scheduler;
    scheduler.FCFS();
    return 0;
}