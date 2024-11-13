#include<bits/stdc++.h>
using namespace std;

class MemoryManagement {
public:
    vector<pair<string, int>> mainMemory;
    int nextLocation;

    MemoryManagement() {
        // # of memory block cha input ghetla - n
        // for(i=0; i<n; i++) {
        //     // cin>>size;
        //     // push n pairs into mainMemory with value <"free", size>;
        // }
    }

    void firstfit() {
        /*
        scan process name and process size
        start from i=0 to n
            if mainMemory[i].first == "free" && mainMemory[i].second>=process_size then
                assign the process name to the mainMemory.first;
                assign process_size to mainMemory.second;
                if(some memory is still remaining)
                    then push the remaining memory below in the mainMemory
                break;
            i++;
        if i==n
            print("no mem block allocted")

        */
    }

    void nextfit() {
        /*
        scan process name and process size
        start from i=(nextLocation+1)%n till i!=nextLocation
            if mainMemory[i].first == "free" && mainMemory[i].second>=process_size then
                assign the process_name to the mainMemory.first;
                assign process_size to mainMemory.second;
                nextLocation = i;
                if(some memory is still remaining)
                    then push the remaining memory below in the mainMemory
                break;
            i = i+1 % n;
        */
    }

    void bestfit() {
        /*
        scan process_name and process_size
        int bestLocation = -1
        int bestSize = INT_MAX
        from i=0 to i=n
            if(mainMemory.first == "free" && process_size <= mainMemory.second && mainMemory.second < bestSize)
                bestLocation = i;
                bestsize = mainMemory[i].second;
            i++;
        if bestLocation != -1
            assign the process_name to the mainMemory[bestLocation].first;
            assign process_size to mainMemory[i].second;
        else
            no mem block found for process_name;
        */
    }

    void worstfit() {
        /*
        scan process_name and process_size
        int worstLocation = -1
        int worstSize = INT_MIN
        from i=0 to i=n
            if(mainMemory.first == "free" && process_size <= mainMemory.second && mainMemory.second > worstSize)
                worstLocation = i;
                worstSize = mainMemory[i].second;
            i++;
        if bestLocation != -1
            assign the process_name to the mainMemory[bestLocation].first;
            assign process_size to mainMemory[i].second;
            if any memory is left after process allocation
                then add pair<"free", remainingSpace> at the end of mainMemory
        else
            no mem block found for process_name;
        */
    }


};

int main() {

    return 0;
}