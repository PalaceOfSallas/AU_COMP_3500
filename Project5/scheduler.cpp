/* Stephen Sallas
 * 04/18/2021
 * COMP-3500
 * Project 5
*/

#include <iostream>
#include <string.h>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <climits>
using namespace std;

/**************************************************************************************/
/**************************** Global Variables ****************************************/
/**************************************************************************************/



/**************************************************************************************/
/**************************** Prototypes **********************************************/
/**************************************************************************************/
// First Come First Serve scheduler algorithm
void FCFS(vector<vector<int>> data);

// Round Robin scheduler algorithm
void RR(vector<vector<int>> data, int timeQuantum);

// Shortest Remaining Time First scheduler algorithm
void SRTF(vector<vector<int>> data);

// Reading an input file
vector<vector<int>> readFile(string inFile);

// Printing statistic information
void printInfo(string file, vector<vector<int>> data);


/**************************************************************************************/
/**************************** Main  ***************************************************/
/**************************************************************************************/
int main(int argc, char *argv[]){
    // Variables
    string file;
    string policy;
    int timeQuantum;
    vector<vector<int>> data;

    // Handling command-line arguments
    if (argc == 1 || argc > 4) {
        cout << "Usage: command file_name [FCFS|RR|SRFT] [time_quantum]" << endl;
        return 0;    
    }
    // Printing file name   
    cout << "File Name: " << argv[1] << endl;
    file = argv[1];    
    // Printing scheduling policy    
    cout << "Scheduling Policy: " << argv[2] << endl;
    policy = argv[2];    
    // Handling RR policy   
    if (strcmp(argv[2], "RR") == 0) {        
        if (argc == 4) {          
            cout << "Time Quantum: " << argv[3] << endl;
            timeQuantum = atoi(argv[3]);
        }
        else 
            cout << "Please enter time quantum for the RR policy!" << endl;    
    }

    // Retrieving info from input file
    data = readFile(file);

    // Printing task info
    printInfo(file, data);

    // Ruinning FCFS algorithm
    if(strcmp(argv[2], "FCFS") == 0)
        FCFS(data);
    else if(strcmp(argv[2], "RR") == 0)
        RR(data, timeQuantum);
    else if(strcmp(argv[2], "SRTF") == 0)
        SRTF(data);
    else
        cout << "Incorrect scheduling policy. Please choose one of the three: FCFS|RR|SRFT" << endl;

    return 0;
}


/**************************************************************************************/
/**************************** Functions  **********************************************/
/**************************************************************************************/
// First Come First Serve scheduler algorithm
void FCFS(vector<vector<int>> data){
    // Variables
    int currentTime = 0;
    int taskEndTime;
    vector<int> startTimes;
    vector<int> waitingTimes;
    double averageWT = 0;
    vector<int> responseTimes;
    double averageRT = 0;
    vector<int> turnaroundTimes;
    double averageTT = 0;
    double cpuUsage = 0;
    
    // Implementing scheduling
    for(int i=0; i<data.size(); i++) {
        if(data[i][1] <= currentTime){
            // Recording start time
            startTimes.push_back(currentTime);
            // Calculating when task should end
            taskEndTime = currentTime + data[i][2];
            // Simming through task
            while(currentTime != taskEndTime){
                cout << "<time " << currentTime << "> process " << data[i][0]
                    << " is running" << endl;
                currentTime++;
            }
            cout << "<time " << currentTime << "> process " << data[i][0]
                << " is finished..." << endl;
            // Calculating turnaround and waiting time for task
            turnaroundTimes.push_back(currentTime - data[i][1]);
            waitingTimes.push_back(turnaroundTimes[i] - data[i][2]);
            responseTimes.push_back(startTimes[i] - data[i][1]);
        }
        // Idling scheduler if needed
        else{
            while(currentTime != data[i][1]){
                cout << "<time " << currentTime << "> idle" << endl;
                currentTime++;
            }
        }
    }
    // Calculating averages
    for(int num : waitingTimes){
        averageWT += num;
    }
    averageWT /= waitingTimes.size();
    for(int num : responseTimes){
        averageRT += num;
    }
    averageRT /= responseTimes.size();
    for(int num : turnaroundTimes){
        averageTT += num;
    }
    averageTT /= turnaroundTimes.size();
    for(vector<int> vec : data){
        cpuUsage += vec[2];
    }
    cpuUsage /= currentTime;
    cpuUsage *= 100;
    // Final messages
    cout << "<time " << currentTime << "> All processes finished....." << endl; 
    cout << "==================================================================" << endl;
    cout << "Average waiting time: " << fixed << setprecision(2) << averageWT << endl;
    cout << "Average response time: " << averageRT << endl;
    cout << "Average turnaround time: " << averageTT << endl;
    cout << "Overall CPU Usage: " << cpuUsage << "%" << endl;
    cout << "==================================================================" << endl;    
}

// Round Robin scheduler algorithm
void RR(vector<vector<int>> data, int timeQuantum){
    // Variables
    vector<vector<int>> dataCopy = data;
    int tempTimeQuantum = timeQuantum;
    vector<vector<int>> dataByFinish;
    int tasksFinished = 0;
    int currentTime = 0;
    vector<int> startTimes;
    vector<int> waitingTimes;
    double averageWT = 0;
    vector<int> responseTimes;
    double averageRT = 0;
    vector<int> turnaroundTimes;
    double averageTT = 0;
    double cpuUsage = 0;

    // Beginning round robin process
    while(tasksFinished != data.size()){
        for(int i=0; i<data.size(); i++) {
            // Continuing if current task has arrived
            if(dataCopy[i][1] <= currentTime){
                // Continuing if current task is not finished
                if(dataCopy[i][2] > 0){
                    if(dataCopy[i][2] == data[i][2]){
                        startTimes.push_back(currentTime);
                    }
                    // Continuing while task has bursts left and for time quantum
                    while((dataCopy[i][2] > 0) && (tempTimeQuantum != 0)){
                        cout << "<time " << currentTime << "> process " << dataCopy[i][0]
                            << " is running" << endl;
                            // Decrementing current tasks burst time and time quantum
                            // Incrementing time
                            dataCopy[i][2]--;
                            tempTimeQuantum--;
                            currentTime++;
                    }
                    // Displaying finished tasks
                    if(dataCopy[i][2] == 0){
                        cout << "<time " << currentTime << "> process " << dataCopy[i][0]
                            << " is finished..." << endl;
                        tasksFinished++;
                        turnaroundTimes.push_back(currentTime - dataCopy[i][1]);
                        waitingTimes.push_back((currentTime - dataCopy[i][1]) - data[dataCopy[i][0]-1][2]);
                        responseTimes.push_back(startTimes[i] - dataCopy[i][1]);
                    }
                }
            }
            // Idling until next arrival time
            else{
                while(currentTime != dataCopy[i][1]){
                    cout << "<time " << currentTime << "> idle" << endl;
                    currentTime++;
                }
            }
            // Resetting temporary time quantum
            tempTimeQuantum = timeQuantum;
        }
    }
    // Calculating averages
    for(int num : waitingTimes){
        //cout << num << endl;
        averageWT += num;
    }
    averageWT /= waitingTimes.size();
    for(int num : responseTimes){
        //cout << num << " ";
        averageRT += num;
    }
    averageRT /= responseTimes.size();
    for(int num : turnaroundTimes){
        averageTT += num;
    }
    averageTT /= turnaroundTimes.size();
    for(vector<int> vec : data){
        cpuUsage += vec[2];
    }
    cpuUsage /= currentTime;
    cpuUsage *= 100;
    // Final messages
    cout << "<time " << currentTime << "> All processes finished....." << endl; 
    cout << "==================================================================" << endl;
    cout << "Average waiting time: " << fixed << setprecision(2) << averageWT << endl;
    cout << "Average response time: " << averageRT << endl;
    cout << "Average turnaround time: " << averageTT << endl;
    cout << "Overall CPU Usage: " << cpuUsage << "%" << endl;
    cout << "==================================================================" << endl;
}

// Shortest Remaining Time First scheduler algorithm
void SRTF(vector<vector<int>> data){
    // Variables
    vector<vector<int>> dataCopy = data;
    vector<vector<int>> arrived;
    vector<int> priorityTask = {0,0,INT_MAX};
    int currentTime = 0;
    int tasksFinished = 0;
    vector<int> startTimes;
    vector<int> waitingTimes;
    double averageWT = 0;
    vector<int> responseTimes;
    double averageRT = 0;
    vector<int> turnaroundTimes;
    double averageTT = 0;
    int cpuUsage = 0;

    // Beginning SRFT process
    while(tasksFinished != data.size()){
        // Looping through tasks
        for(vector<int> vec : dataCopy) {
            // Checking if task has arrived
            if(vec[1] <= currentTime){
                // Checking if task has bursts left
                if(vec[2] > 0)
                    arrived.push_back(vec);
            }
        }
        // Idling if no available tasks
        if(arrived.size() == 0){
            cout << "<time " << currentTime << "> idle" << endl;
            currentTime++;
            continue;
        }
        // Finding priority task
        for(vector<int> currentTask : arrived){
            if(currentTask[2] < priorityTask[2]){
                priorityTask.clear();
                priorityTask.push_back(currentTask[0]);
                priorityTask.push_back(currentTask[1]);
                priorityTask.push_back(currentTask[2]);
            }
        }
        // Adding start time
        if(dataCopy[priorityTask[0]-1][2] == data[priorityTask[0]-1][2]){
            startTimes.push_back(currentTime);
        }
        // Printing statements
        cout << "<time " << currentTime << "> process " << priorityTask[0]
            << " is running" << endl;
        // Decrementing bursts and incrementing time
        dataCopy[priorityTask[0]-1][2]--;
        currentTime++;
        // Checking if task finished
        if(dataCopy[priorityTask[0]-1][2] == 0){
            cout << "<time " << currentTime << "> process " << priorityTask[0]
                << " is finished..." << endl;
            tasksFinished++;
            // Adding to average vectors
            turnaroundTimes.push_back(currentTime - dataCopy[priorityTask[0]-1][1]);
            waitingTimes.push_back((currentTime - dataCopy[priorityTask[0]-1][1]) - data[priorityTask[0]-1][2]);
            responseTimes.push_back(startTimes[priorityTask[0]-1] - data[priorityTask[0]-1][1]);
        }
        arrived.clear();
        priorityTask.clear();
        priorityTask = {0,0,INT_MAX};
    }
    // Calculating averages
    for(int num : waitingTimes){
        averageWT += num;
    }
    averageWT /= waitingTimes.size();
    for(int num : responseTimes){
        averageRT += num;
    }
    averageRT /= responseTimes.size();
    for(int num : turnaroundTimes){
        averageTT += num;
    }
    averageTT /= turnaroundTimes.size();
    for(vector<int> vec : data){
        cpuUsage += vec[2];
    }
    cpuUsage /= currentTime;
    cpuUsage *= 100;
    // Final messages
    cout << "<time " << currentTime << "> All processes finished....." << endl; 
    cout << "==================================================================" << endl;
    cout << "Average waiting time: " << fixed << setprecision(2) << averageWT << endl;
    cout << "Average response time: " << averageRT << endl;
    cout << "Average turnaround time: " << averageTT << endl;
    cout << "Overall CPU Usage: " << cpuUsage << "%" << endl;
    cout << "==================================================================" << endl;
}

// Reading an input file
vector<vector<int>> readFile(string inFile){
    // Variables
    vector<vector<int>> data;
    int x;
    ifstream input;
    // Opening file and error handling
    input.open(inFile);
    if (!input){
        cout << "Unable to open file. Exiting." << endl;
        exit(1);
    }
    // Reading data
    vector<int> tempVec;
    int i = 0;
    while (input >> x){
        tempVec.push_back(x);
        i++;
        if (i == 3){
            data.push_back(tempVec);
            tempVec.clear();
            i = 0;
        }
    }
    // Returning data
    return data;
}

// Printing statistic information
void printInfo(string file, vector<vector<int>> data){
    cout << "There are " << data.size() << " tasks loaded from \"" << file << "\"." << endl;
    cout << "PID\tArrival Time\tBurst Time" << endl;
    for(int i = 0; i<data.size(); i++) {
        cout << data[i].at(0) << "\t" << data[i].at(1) << "\t\t" << data[i].at(2) << "\t" << endl; 
    }
    cout << "Press any key to continue..." << endl;
    cout << "==================================================================" << endl;
}
