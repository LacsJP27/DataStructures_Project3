
#include <iostream>
using namespace std;
   
class CPUJob {
    public:
        int job_id; // Unique identifier for the job
        int priority; // Priority level of the job (1-10)
        int job_type; // Job type (1-10)
        int cpu_time_consumed; // Total CPU time consumed by the job
        int memory_consumed; // Total memory consumed thus far

        CPUJob();
        CPUJob(int job_id, int priority, int job_type, int cpu_time_consumed, int memory_consumed);
};

CPUJob::CPUJob(){
    job_id = 0;
    priority = 0;
    job_type = 0;
    cpu_time_consumed = 0;
    memory_consumed = 0;
}

CPUJob::CPUJob(int job_id, int priority, int job_type, int cpu_time_consumed, int memory_consumed){
    this->job_id = job_id;
    this->priority = priority;
    this->job_type = job_type;
    this->cpu_time_consumed = cpu_time_consumed;
    this->memory_consumed = memory_consumed;
}


template<class DT>
class Queue{
    public:
        DT* JobPointer; // Pointer to a job (e.g., CPUJob)
        Queue<DT>* previous; // Pointer to the next node in the queue
        
        Queue();
        Queue(DT* JobPointer);
        ~Queue();
};

template<class DT>
Queue<DT>::Queue(){
    JobPointer = nullptr;
    previous = nullptr;
}

template<class DT>
Queue<DT>::Queue(DT* JobPointer){
    this->JobPointer = JobPointer;
    previous = nullptr;
}

template<class DT>
Queue<DT>::~Queue(){
    if(JobPointer != nullptr){
        delete JobPointer;
        JobPointer = nullptr;
    }
    if(previous != nullptr){
        delete previous;
        previous = nullptr;
    }
}

template<class DT>
class NovelQueue {
        public:
            Queue<DT>* front; // Pointer to the front of the queue
            Queue<DT>* rear; // Pointer to the rear of the queue
            Queue<DT>** NodePtrs; // Array of pointers to Queue nodes
            int size; // Number of elements in the queue)

            NovelQueue();

            void Enqueue(DT* JobPointer);
            DT* Dequeue();
};

template<class DT>
NovelQueue<DT>::NovelQueue() {
    front = nullptr;
    rear = nullptr;
    NodePtrs = nullptr;
};

template <class DT>
void NovelQueue<DT>::Enqueue(DT* JobPointer) {
    if(!front) {
        front = new Queue<DT>(JobPointer);
        rear = front;
        ++size;
        return;
    }
    Queue<DT>* temp = rear;
    rear = new Queue<DT>(JobPointer);
    temp->previous = rear;
    ++size;
}

template <class DT>
DT* NovelQueue<DT>::Dequeue() {
    if(front == nullptr) {
        cout << "Queue is empty" << endl;
        return nullptr;
    }
    else {
        Queue<DT>* temp = front;
        front = front->previous;
        DT* JobPointer = temp->JobPointer;
        cout << "Dequeueing" << endl;
        
        --size;
        return JobPointer;
    }
}

int main() {
    int n; // Number of commands
    cin >> n; // Read the number of commands
    
    // Instantiate a NovelQueue for CPUJob pointers
    NovelQueue<CPUJob>* myNovelQueue = new NovelQueue<CPUJob>();

    char command; // Command to be executed

    // Variables for job attributes
    int job_id, priority, job_type, cpu_time_consumed, memory_consumed;

    for (int i = 0; i < n; ++i) {
        cin >> command; // Read the command
        switch(command) {
            case 'A': { // Add a job to the queue
                cin >> job_id >> priority >> job_type >> cpu_time_consumed >> memory_consumed;
                CPUJob* newJob = new CPUJob(job_id, priority, job_type, cpu_time_consumed, memory_consumed);
                myNovelQueue->Enqueue(newJob);
                break;
            }
            case 'R': {
                CPUJob* dequeuedJob = myNovelQueue->Dequeue();
                if (dequeuedJob) {
                    //need displat function
                    cout << "Job ID: " << dequeuedJob->job_id << endl;
                    cout << "Priority: " << dequeuedJob->priority << endl;
                    cout << "Job Type: " << dequeuedJob->job_type << endl;
                    cout << "CPU Time Consumed: " << dequeuedJob->cpu_time_consumed << endl;
                    cout << "Memory Consumed: " << dequeuedJob->memory_consumed << endl;
                    delete dequeuedJob;
                }
                break;
            }
        }   
    }

    return 0;
}

/**
 * first we create a class CPUJob with 5 attributes
 * tested that this was creating correctly
 * 
 * next test enqueing that job
 * that worked prompt for LLM
 *  I am getting errors with my declaration of NodePtrs. Is the highllighted declaration of NodePtrs correct?
 *      It was not correct and i needed a for loop to initialize the array of pointers
 * LLm create a function to enqueue an item into an array implementation of a queue
 * LLM create a destructor for the CPU (doesn't need one since only prim data types)
 */