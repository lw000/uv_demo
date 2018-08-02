#ifndef lw_queue_h
#define lw_queue_h


// Defines a circular buffer which acts as a FIFO queue.
typedef struct queue_t
{
    int    size;
    int    next;
    int    capacity;
    void** data;
} queue_t;

// Allocates and returns a new queue. The capacity specifies the maximum
// number of items that can be in the queue at one time. A capacity greater
// than INT_MAX / sizeof(void*) is considered an error. Returns NULL if
// initialization failed.
queue_t* lw_queue_init(size_t capacity);

// Releases the queue resources.
void lw_queue_dispose(queue_t* queue);

// Enqueues an item in the queue. Returns 0 if the add succeeded or -1 if it
// failed. If -1 is returned, errno will be set.z
int lw_queue_add(queue_t* queue, void* value);

// Dequeues an item from the head of the queue. Returns NULL if the queue is
// empty.
void* lw_queue_remove(queue_t* queue);

// Returns, but does not remove, the head of the queue. Returns NULL if the
// queue is empty.
void* lw_queue_peek(queue_t*);

#endif
