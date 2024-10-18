
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
        CPUJob* operator=(CPUJob* job);

        void display();
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

void CPUJob::display() {
    cout << "Job ID: " << job_id << ", Priority: " << priority << ", Job Type: " << job_type << ", CPU Time Consumed: " << cpu_time_consumed << ", Memory Consumed: " << memory_consumed << endl;
}

CPUJob* CPUJob::operator=(CPUJob* job){
    this->job_id = job->job_id;
    this->priority = job->priority;
    this->job_type = job->job_type;
    this->cpu_time_consumed = job->cpu_time_consumed;
    this->memory_consumed = job->memory_consumed;
    cout << "CPUJob assigned" << endl;
    return this;
}

template<class DT>
class Queue{
    public:
        DT* JobPointer; // Pointer to a job (e.g., CPUJob)
        Queue<DT>* previous; // Pointer to the previous node node in the queue
        Queue<DT>* next; // Pointer to the next node in the queue
        
        Queue();
        Queue(DT* JobPointer);
        Queue(Queue<DT>* q);
        ~Queue();
        Queue<DT>* operator=(Queue<DT>* q);

        void displayQueue();
};

template<class DT>
Queue<DT>::Queue(){
    JobPointer = nullptr;
    previous = nullptr;
    next = nullptr;
}

template<class DT>
Queue<DT>::Queue(DT* JobPointer){
    this->JobPointer = JobPointer;
    previous = nullptr;
     next = nullptr;
}

template<class DT>
Queue<DT>::Queue(Queue<DT>* q){
    this->JobPointer = q->JobPointer;
    this->previous = q->previous;
    this->next = q->next;
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

// template<class DT>
// Queue<DT>* Queue<DT>::operator=(Queue<DT>* q){
//     delete this->JobPointer;
//     this->JobPointer = new CPUJob(q->JobPointer->job_id, q->JobPointer->priority, q->JobPointer->job_type, q->JobPointer->cpu_time_consumed, q->JobPointer->memory_consumed);
//     delete this->previous;
//     this->previous = q->previous;
// }

template<class DT>
void Queue<DT>::displayQueue(){
    if(JobPointer != nullptr){
        JobPointer->display();
    }
    if(previous != nullptr){
        previous->displayQueue();
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
            ~NovelQueue();

            NovelQueue<DT>* operator=(NovelQueue<DT>* nq);

            void enqueue(DT* JobPointer);
            DT* dequeue();
            void modify(int job_id, int new_priority, int new_job_type, int new_cpu_time_consumed, int new_memory_consumed);
            void change(int job_id, int new_priority, int field_index, int new_value); //new Value??
            void promote(int job_id, int positions);
            void reorder(int attribute_index);
            int count();
            void listJobs();
            void display();

            // additional methods
            DT* binarySearchJob(int job_id, int left, int right);
            void sortJobsByID();
            void promoteNodeHelper(Queue<DT>* node, int positions);

};

/*******CONSTRUCTORS AND DESTRUCTORS********************* */

template<class DT>
NovelQueue<DT>::NovelQueue() {
    front = nullptr;
    rear = nullptr;
    NodePtrs = nullptr;
};

template <class DT>
NovelQueue<DT>::~NovelQueue(){
    if(front != nullptr){
        delete front;
        front = nullptr;
    }
    if(rear != nullptr){
        delete rear;
        rear = nullptr;
    }
    if(NodePtrs != nullptr){
        for(int i = 0; i < size; ++i){
            delete NodePtrs[i];
        }
        delete[] NodePtrs;
    }
}

/*******OPERATOR OVERLOADS********************* */

template <class DT>
NovelQueue<DT>* NovelQueue<DT>::operator=(NovelQueue<DT>* dq) {
    this->size = dq->size;
    this->rear = dq->rear;
    this->front = dq->front;
    this->size = dq->size;
    
    for(int i = 0; i < dq->size; ++i){
        this->NodePtrs[i] = dq->NodePtrs[i];
    }
}

/*******QUEUE METHODS********************* */

template <class DT>
void NovelQueue<DT>::enqueue(DT* JobPointer) {
    if(!front) {
        front = new Queue<DT>(JobPointer);
        rear = front;
        ++size;
        NodePtrs = new Queue<DT>*[1];
        NodePtrs[0] = rear;
    }
    else{
        //Enqueue the new job
        Queue<DT>* temp = rear;
        rear = new Queue<DT>(JobPointer);
        temp->previous = rear;
        rear->next = temp;

        //Update the NodePtrs array
        Queue<DT>** tempNodePtrs = new Queue<DT>*[size + 1];
        for(int i = 0; i < size; ++i){
            tempNodePtrs[i] = NodePtrs[i];
        }
        tempNodePtrs[size] = rear;
        delete[] NodePtrs;
        NodePtrs = tempNodePtrs;
        ++size;
    }
}

template <class DT>
DT* NovelQueue<DT>::dequeue() {
     //FIXME
    if(front == nullptr) {
        cout << "Queue is empty" << endl;
        return nullptr;
    }
    else { 
        Queue<DT>* temp = new Queue<DT>(front);
        // delete front;
        front = new Queue<DT>(temp->previous);
        front -> next = nullptr;
        DT* JobPointer = new CPUJob(temp->JobPointer->job_id, temp->JobPointer->priority, temp->JobPointer->job_type, temp->JobPointer->cpu_time_consumed, temp->JobPointer->memory_consumed);



        //Update the NodePtrs array
        Queue<DT>** tempNodePtrs = new Queue<DT>*[size - 1];
        
        this->sortJobsByID();
        DT* removedJob = binarySearchJob(JobPointer->job_id,0 , size - 1);
        removedJob->job_id = -1;

        cout << JobPointer->job_id << endl;

        //j is for only incrementing if we find a job that is not -1
        int j = 0;
        for(int i = 0; i < size; ++i){
            if(NodePtrs[i]->JobPointer->job_id != -1){
                tempNodePtrs[j++] = NodePtrs[i];
            }
        }


        delete[] NodePtrs;

        --size;
        NodePtrs = new Queue<DT>*[size];
        for(int i = 0; i < size; ++i){
            NodePtrs[i] = new Queue<DT>(tempNodePtrs[i]);
        }
        // cout << "dequeued" << endl;
        delete[] tempNodePtrs;
   
        return JobPointer;
    }
}

template <class DT>
void NovelQueue<DT>::promote(int job_id, int positions) {
    Queue<DT>* current = front;
    while(current != nullptr && current->JobPointer->job_id != job_id){
        current = current->previous;
    }

    if(current == nullptr){
        cout << "Job not found" << endl;
        return;
    }
    // cout << current->JobPointer->job_id<< "********" << endl;
    // cout << current->next->JobPointer->job_id << endl;
    promoteNodeHelper(current, positions);
    cout << front->JobPointer->job_id << endl;
}

template <class DT>
void NovelQueue<DT>::promoteNodeHelper(Queue<DT>* node, int positions) {
    cout << "Current Node: " << node->JobPointer->job_id << " " << positions << endl;
    if(positions <= 0 || node->JobPointer->job_id == front->JobPointer->job_id){
        cout << "final promotion" << endl;
        return;
    }
    Queue<DT>*prevNode = nullptr;
    Queue<DT>* nextNode = nullptr;
    if(node->previous != nullptr){
        prevNode = new Queue<DT>(node->previous);
        prevNode->next = nextNode;
    }
    if(node->next != nullptr){
       nextNode = new Queue<DT>(node->next);
       node->next = nextNode->next;
       cout << "Next Temp Node: " << nextNode->JobPointer->job_id << endl;
       nextNode->previous = prevNode;
       cout << "Next Temp Node Previous: " << nextNode->previous->JobPointer->job_id << endl;
       nextNode->next = node;
       cout << "Next Temp Node Next: " << nextNode->next->JobPointer->job_id << endl;
    }
    
    node->previous = nextNode;
    cout << "Previous Node: " << node->previous->JobPointer->job_id << endl;
    
    cout << "Next Node: " << node->next->JobPointer->job_id << endl;
    if(node->next == nullptr){
        cout<< "node next is null" << endl;
        front = node;
    }
    if(node->previous == nullptr){
        rear = node;
    }
    cout << "Front Node: " << front->JobPointer->job_id << endl;

    promoteNodeHelper(node, positions - 1);
}

template <class DT>
void NovelQueue<DT>::modify(int job_id, int new_priority, int new_job_type, int new_cpu_time_consumed, int new_memory_consumed) {

}

/*******ADDITIONAL METHODS********************* */
template <class DT>
DT* NovelQueue<DT>::binarySearchJob(int job_id, int left, int right) {
    if(left > right){
        return nullptr;
    }

    int mid = left + (right - left) / 2;

    if(NodePtrs[mid]->JobPointer->job_id == job_id){
        return NodePtrs[mid]->JobPointer;
    }
    if(NodePtrs[mid]->JobPointer->job_id > job_id){
        return this->binarySearchJob(job_id, left, mid - 1);
    }
    else{
        return this->binarySearchJob(job_id, mid + 1, right);
    }
    
}

template <class DT>
void NovelQueue<DT>::sortJobsByID() {
    if(size <= 1){
        return;
    }
    bool swapped;
    for(int i = 0; i < size -1; ++i) {
        if((NodePtrs[i]->JobPointer->job_id) == -1){
        } 
        swapped = false;
        for(int j = 0; j < size - i - 1; ++j) {
            
            if((NodePtrs[j]->JobPointer->job_id) > NodePtrs[j + 1]->JobPointer->job_id){
            //Swap the pointers
            Queue<DT>* temp = NodePtrs[j];
            NodePtrs[j] = NodePtrs[j + 1];
            NodePtrs[j + 1] = temp;
            swapped = true;
            }
        }
        if(!swapped){
            break;
        }
    }
}

/***********************DISPLAY AND LIST METHODS **********************/
template <class DT>
void NovelQueue<DT>::display(){
    if(front== nullptr){
        cout << "Queue is empty" << endl;
    }
    else {
        front->displayQueue();

    }
}

template <class DT>
void NovelQueue<DT>::listJobs() {
    this->sortJobsByID();
    
    for(int i = 0; i < size; ++i){
        NodePtrs[i]->JobPointer->display();
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
    int new_priority, new_job_type, new_cpu_time_consumed, new_memory_consumed;
    int positions;

    for (int i = 0; i < n; ++i) {
        cin >> command; // Read the command
        switch(command) {
            case 'A': { // Add a job to the queue
                cin >> job_id >> priority >> job_type >> cpu_time_consumed >> memory_consumed;
                CPUJob* newJob = new CPUJob(job_id, priority, job_type, cpu_time_consumed, memory_consumed);
                myNovelQueue->enqueue(newJob);
                // cout<< "Enqueued Job: " << endl;
                // newJob->display();
                // cout << "Jobs after enqueue:" << endl;
                // myNovelQueue->display();
                break;
            }
            case 'R': {
                cout << "Dequeueing a job" << endl;
                CPUJob* dequeuedJob = myNovelQueue->dequeue();
                if (dequeuedJob) {
                    //need displat function
                    // dequeuedJob->display();
                    // cout << "Dequeued job: " << dequeuedJob->job_id << endl;
                    delete dequeuedJob;
                }
                break;
            }
            case 'P': {
                cout << "Promoting a job" << endl;
                cin >> job_id >> positions;
                myNovelQueue->promote(job_id, positions);
                myNovelQueue->display();
                break;
            }
        }   
    }
    // cout << "Displaying the queue" << endl;
    // myNovelQueue->display();
    // cout << "Listing jobs" << endl;
    // myNovelQueue->listJobs();
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
 * LLM For the enqueue method in project3.cpp, I want enqueue like I am already doing, but I also want to update the NodePtrs array with the new JobPointer.
 * LLM How do i create a sort method for NovelQueue that sorts the nodePtrs array by job_id in increasing order
 */