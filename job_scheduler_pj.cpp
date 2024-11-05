#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <cmath>
#include <fstream>

using namespace std;
void createCSVFile(const string& filename) {
    ofstream file(filename.c_str());
    
    // Write header
    file << "arrivalTime,coresRequired,memoryRequired,executionTime,id\n";

    // Example job data
    file << "0,4,16,2,1\n";
    file << "1,8,32,3,2\n";
    file << "2,2,8,1,3\n";
    file << "3,12,48,4,4\n";
    file << "4,6,24,3,5\n";

    file.close();
    cout << "CSV file created: " << filename << endl;
}

class Job {
public:
    int arrivalTime;
    int coresRequired;
    int memoryRequired;
    int executionTime;
    int id;

    Job(int at, int cr, int mr, int et, int i) 
        : arrivalTime(at), coresRequired(cr), memoryRequired(mr), executionTime(et), id(i) {}
};

class WorkerNode {
public:
    int id;
    int availableCores;
    int availableMemory;
    int busyUntil;

    WorkerNode(int i, int ac, int am):
     id(i), availableCores(ac), availableMemory(am), busyUntil(0) {}
};

bool compareByArrivalTime(const Job &a, const Job &b) {
    return a.arrivalTime < b.arrivalTime;
}

bool compareBySmallestJob(const Job &a, const Job &b) {
    return (a.executionTime * a.coresRequired * a.memoryRequired) < (b.executionTime * b.coresRequired * b.memoryRequired);
}

bool compareByShortDuration(const Job &a, const Job &b) {
    return a.executionTime < b.executionTime;
}

class JobScheduler {
public:
    JobScheduler(int numWorkers, int coresPerWorker, int memoryPerWorker) {
        for (int i = 0; i < numWorkers; ++i) {
            workers.push_back(WorkerNode(i, coresPerWorker, memoryPerWorker));
        }
    }

    void addJob(int arrivalTime, int coresRequired, int memoryRequired, int executionTime, int id) {
        jobs.push_back(Job(arrivalTime, coresRequired, memoryRequired, executionTime, id));
    }

    void scheduleJobs() {
        sort(jobs.begin(), jobs.end(), compareByArrivalTime);  // Change sorting function for different queues
        for (vector<Job>::iterator it = jobs.begin(); it != jobs.end(); ++it) {
            bool allocated = false;
            for (vector<WorkerNode>::iterator worker = workers.begin(); worker != workers.end(); ++worker) {
                if (worker->availableCores >= it->coresRequired && worker->availableMemory >= it->memoryRequired && worker->busyUntil <= it->arrivalTime) {
                    worker->availableCores -= it->coresRequired;
                    worker->availableMemory -= it->memoryRequired;
                    worker->busyUntil = it->arrivalTime + it->executionTime;
                    allocated = true;
                    break;
                }
            }
            if (!allocated) {
                pendingJobs.push(*it);
            }
        }
    }

    void printWorkerStatus() {
        for (vector<WorkerNode>::iterator worker = workers.begin(); worker != workers.end(); ++worker) {
            cout << "Worker " << worker->id << ": " << worker->availableCores << " cores, " << worker->availableMemory << " GB memory available, busy until " << worker->busyUntil << std::endl;
        }
    }

private:
    vector<Job> jobs;
    vector<WorkerNode> workers;
    queue<Job> pendingJobs;
};

int main() {
    JobScheduler scheduler(128, 24, 64);
    createCSVFile("jobs.csv");//Example jobs
    scheduler.addJob(0, 4, 16, 2, 1);
    scheduler.addJob(1, 8, 32, 3, 2);
    scheduler.addJob(2, 2, 8, 1, 3);

    scheduler.scheduleJobs();
    scheduler.printWorkerStatus();

    return 0;
}
