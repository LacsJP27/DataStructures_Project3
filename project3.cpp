
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
    // cout << "Job ID: " << job_id << ", Priority: " << priority << ", Job Type: " << job_type << ", CPU Time Consumed: " << cpu_time_consumed << endl;
    // Job ID: 112, Priority: 6, Job Type: 3, CPU Time Consumed: 340, Memory Consumed: 1024
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

            bool enqueue(DT* JobPointer);
            DT* dequeue();
            void modify(int job_id, int new_priority, int new_job_type, int new_cpu_time_consumed, int new_memory_consumed);
            void change(int job_id, int field_index, int new_value); //new Value??
            void promote(int job_id, int positions);
            void reorder(int attribute_index);
            int count();
            void listJobs();
            void display();

            // additional methods
            DT* binarySearchJob(int job_id, int left, int right);
            void sortJobsByField(int field_index);
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
bool NovelQueue<DT>::enqueue(DT* JobPointer) {
    if(binarySearchJob(JobPointer->job_id, 0, size - 1) != nullptr){
        cout << "Job ID " << JobPointer->job_id << " already exists!" << endl;
        return false;
    }
    cout<< "Enqueued Job: " << endl;
    JobPointer->display();
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
    

    return true;
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
        
        this->sortJobsByField(1);
        DT* removedJob = binarySearchJob(JobPointer->job_id,0 , size - 1);
        removedJob->job_id = -1;

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
        cout << "Job with ID "<< job_id <<" not found in the queue." << endl;
        return;
    }
   
    promoteNodeHelper(current, positions);

    cout << "Promoted Job ID "<< job_id <<" by " << positions << " Position(s):" << endl;
    current->JobPointer->display();
    cout << "Jobs after promotion:" << endl;
    this->display();
}

template <class DT>
void NovelQueue<DT>::promoteNodeHelper(Queue<DT>* node, int positions) {
    if(positions <= 0 || node == front){
        return;
    }
    Queue<DT>*prevNode = node->previous;
    Queue<DT>* nextNode = node->next;

    //detach the node from the current position
    if(prevNode != nullptr){
        prevNode->next = nextNode; //link previous node to next node
    }
    if(nextNode != nullptr){
        nextNode->previous = prevNode; //link next node to previous node
    }

    //if node was at the rear update the rear pointer
    if(rear == node){
        rear = nextNode;
    }
    
    Queue<DT>* afterNext = nextNode->next;
    node->next = afterNext;
    node->previous = nextNode;

    if(afterNext != nullptr){
        afterNext->previous = node;
    }
    else{
        front = node; //if node is doesn't have a next node then it is the front
    }

    promoteNodeHelper(node, positions - 1);
}

/***MODIFY******************* */

template <class DT>
void NovelQueue<DT>::modify(int job_id, int new_priority, int new_job_type, int new_cpu_time_consumed, int new_memory_consumed) {
    DT* targetJob = binarySearchJob(job_id, 0, size - 1);
    if(targetJob == nullptr){
        cout << "Job not found" << endl;
        return;
    }
    targetJob->priority = new_priority;
    targetJob->job_type = new_job_type;
    targetJob->cpu_time_consumed = new_cpu_time_consumed;
    targetJob->memory_consumed = new_memory_consumed;
    targetJob->display();

    cout << "Jobs after modification:" << endl;
    this->display();
}

template <class DT>
void NovelQueue<DT>::change(int job_id, int field_index, int value) {
    DT* targetJob = binarySearchJob(job_id, 0, size - 1);
    if(targetJob == nullptr){
        cout << "Job with ID "<< job_id <<" not found in the queue." << endl;
        return;
    }
    cout << "Changed Job ID " << job_id << " field " << field_index << " to " << value << ":" << endl;
    

    switch(field_index){
        case 1:
            targetJob->priority = value;
            break;
        case 2:
            targetJob->job_type = value;
            break;
        case 3:
            targetJob->cpu_time_consumed = value;
            break;
        case 4:
            targetJob->memory_consumed = value;
            break;
        default:
            cout << "Invalid field index" << endl;
    }
    targetJob->display();
    cout << "Jobs after changing field:" << endl;
    this->display();
}

/***** REORDER***************** */
template <class DT>
void NovelQueue<DT>::reorder(int attribute_index) {
    sortJobsByField(attribute_index);
    for(int i = 0; i < size; ++i){
        if(i == 0){
            front = NodePtrs[i];
            NodePtrs[i]->next = nullptr;
            if(size > 1) {
                NodePtrs[i]->previous = NodePtrs[i + 1];
            }
            else {
                NodePtrs[i]->previous = nullptr;
                rear = NodePtrs[i];
            }
        
        }
        else if(i == size - 1){
            rear = NodePtrs[i];
            NodePtrs[i]->previous = nullptr;
            NodePtrs[i]->next = NodePtrs[i - 1];
        }
        else {
            NodePtrs[i]->next = NodePtrs[i - 1];
            NodePtrs[i]->previous = NodePtrs[i + 1];
        }
    }
}

/***COUNT ***** */
template <class DT>
int NovelQueue<DT>::count() {
    cout << "Number of elements in the queue: " << size << endl;
    return size;
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
void NovelQueue<DT>::sortJobsByField(int field_index) {
    if(field_index < 1 || field_index > 5){
        cout << "Invalid field index" << endl;
        return;
    }
    if(size <= 1){
        return;
    }

    bool swapped;
    for(int i = 0; i < size -1; ++i) {
        swapped = false;
        for(int j = 0; j < size - i - 1; ++j) {
            bool shouldSwap = false;
            switch(field_index) {
                case 1:
                    shouldSwap = (NodePtrs[j]->JobPointer -> job_id) > (NodePtrs[j + 1] -> JobPointer -> job_id);
                    break;
                 case 2:
                    shouldSwap = (NodePtrs[j]->JobPointer->priority > NodePtrs[j + 1]->JobPointer->priority);
                    break;
                case 3:
                    shouldSwap = (NodePtrs[j]->JobPointer->job_type > NodePtrs[j + 1]->JobPointer->job_type);
                    break;
                case 4:
                    shouldSwap = (NodePtrs[j]->JobPointer->cpu_time_consumed > NodePtrs[j + 1]->JobPointer->cpu_time_consumed);
                    break;
                case 5:
                    shouldSwap = (NodePtrs[j]->JobPointer->memory_consumed > NodePtrs[j + 1]->JobPointer->memory_consumed);
                    break;
            }
            if(shouldSwap){
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
    this->sortJobsByField(1);
    
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
               
                if(myNovelQueue->enqueue(newJob)){
                    cout << "Jobs after enqueue:" << endl;
                    myNovelQueue->display();
                }
                break;
            }
            case 'R': {
                cout << "Dequeued Job: " << endl;
                CPUJob* dequeuedJob = myNovelQueue->dequeue();
                if (dequeuedJob) {
                    //need displat function
                    dequeuedJob->display();
                    delete dequeuedJob;
                }
                cout << "Jobs after dequeue:" << endl;
                myNovelQueue->display();
                break;
            }
            case 'P': {
                cin >> job_id >> positions;
                myNovelQueue->promote(job_id, positions);

                break;
            }
            case 'M': {
                cin >> job_id >> new_priority >> new_job_type >> new_cpu_time_consumed >> new_memory_consumed;
                cout << "Modified Job ID " << job_id << ":" << endl;
                myNovelQueue->modify(job_id, new_priority, new_job_type, new_cpu_time_consumed, new_memory_consumed);
                
                break;
            }
            case 'C': {
                cin >> job_id;
                int field_index, new_value;
                cin >> field_index >> new_value;
                myNovelQueue->change(job_id, field_index, new_value);
                
                break;
            }
            case 'O': {
                
                int field_index;

                cin >> field_index;
                cout << "Reordered Queue by attribute " << field_index << ":" << endl;

                myNovelQueue->reorder(field_index);
                myNovelQueue->display();
                break;
            }
            case 'N': {
                myNovelQueue->count();
                break;
            }
            case 'L' : {
                cout << "List of jobs sorted by job IDs:" << endl; 
                myNovelQueue->listJobs();
                break;
            }
            case 'D': {
                cout << "Displaying all jobs in the queue:" << endl;
                myNovelQueue->display();
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
 * LLM For the enqueue method in project3.cpp, I want enqueue like I am already doing, but I also want to update the NodePtrs array with the new JobPointer.
 * LLM How do i create a sort method for NovelQueue that sorts the nodePtrs array by job_id in increasing order
 * LLM I am getting an error with this code segment. I am trying to promote a node towards the front of a queue with a given number of positions. Can you find an error in my method?
 *     I was not updating the front pointer correctly, and I was creating duplicate pointers when it wasn't necessary.
 * LLM for the sortJobsByfield method I want to sort all the jobs based on the field_index. If the field index is 0, then the method should sort by job_id. If 1, it should sort by priority etc... I already have bubble sort for sorting by job_id. how do I implement this for the other fields?
 * LLM I am trying to reorder my queue based on my new NodePtrs array after sorting based on an attribute index. However I think I'm creating an infinite queue. how do I fix this?
 */