
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
        ~CPUJob();
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

CPUJob::~CPUJob(){

    delete this;
}

template<class DT>
class Queue{
    public:
        DT* JobPointer; // Pointer to a job (e.g., CPUJob)
        Queue<DT>* next; // Pointer to the next node in the queue
        
        Queue();
        Queue(DT* JobPointer);
        ~Queue();
};

template<class DT>
Queue<DT>::Queue(){
    JobPointer = nullptr;
    next = nullptr;
}

template<class DT>
Queue<DT>::Queue(DT* JobPointer){
    this->JobPointer = JobPointer;
    next = nullptr;
}

template<class DT>
Queue<DT>::~Queue(){
    if(JobPointer != nullptr){
        delete JobPointer;
        JobPointer = nullptr;
    }
    if(next != nullptr){
        delete next;
        next = nullptr;
    }
}

template<class DT>
class NovelQueue {
        public:
            Queue<DT>* front; // Pointer to the front of the queue
            Queue<DT>** NodePtrs; // Array of pointers to Queue nodes
            int size; // Number of elements in the queue)

            NovelQueue();

            void Enqueue(DT* JobPointer);
            DT* Dequeue();
};

template<class DT>
NovelQueue<DT>::NovelQueue() {
    front = nullptr;
    NodePtrs = nullptr;
};

template <class DT>
void NovelQueue<DT>::Enqueue(DT* JobPointer) {
    if (front == nullptr){
        front = new Queue<DT>(JobPointer);
    }
    else {
        Queue<DT>* temp = front;
        front = new Queue<DT>(JobPointer);
        front->next = temp;
    }
    ++size;
}

template <class DT>
DT* NovelQueue<DT>::Dequeue() {
    // if(front == -1) {
    //     cout << "Queue is empty" << endl;
    //     return nullptr;
    // }

    // Queue<DT>* temp = NodePtrs[front];
    // DT* job_pointer = temp->JobPointer;
    // delete temp;
    // NodePtrs[front] = nullptr;

    // if(front == rear) {
    //     front = -1;
    //     rear  = -1;
    // }
    // else {
    //     front = (front + 1) % size;
    // }
    // if(front < 0){
    //     cout << "Queue is empty" << endl;
    //     return nullptr;
    // }
    // else {
    //     Queue<DT>* temp = NodePtrs[front];
    //     NodePtrs[front] = nullptr;
    //     front --;
    //     return temp->JobPointer;
    // }
}

int main() {
    CPUJob* job1 = new CPUJob(1, 1, 1, 1, 1);
    NovelQueue<CPUJob>* myNovelQueue = new NovelQueue<CPUJob>();
    myNovelQueue->Enqueue(job1);
    cout << myNovelQueue->front->JobPointer->job_id << endl;
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
 */