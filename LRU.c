#define MAX_CACHE_SIZE 4

typedef struct queue_l{
    int data;
    struct queue_l *next;
    struct queue_l *prev;
}queue_l;

typedef struct dummy{
    queue_l * front;
    queue_l * tail;
    int size; //Extra parameter to keep track of number of pages */
}dummy;

/* Initialize the queue */
void init_queue_l(dummy **q){
    *q  = (dummy *)malloc(sizeof(dummy));
    if(*q){
        (*q)->front = NULL;
        (*q)->tail = NULL;
        (*q)->size =0;
    }   
}   

/* Checks if queue is empty */
int is_empty_l(dummy *q){
    if( q->front == NULL && q->tail == NULL)
        return true;

    return false;
}

/* Enqueue the element and returns the pointer to be stored in hash */
queue_l * enqueue_l(dummy *q, int elem){
    queue_l *temp = (queue_l *) malloc(sizeof(queue_l));
    if(temp){
        temp->data = elem;
        temp->next = NULL;
        temp->prev = q->tail;
        if(q->tail){
            q->tail->next = temp;
        }
        q->tail = temp;
        if(q->front == NULL)
            q->front = temp;
    
        q->size++;
    }
    return temp;
}

/* Dequeue the element from queue */
int dequeue_l(dummy *d){

    if(is_empty_l(d)){
        printf("\n Queue is empty");
            return -1;
        }

    queue_l *q  = d->front;
    d->front = q->next;
    if(q->next)
        q->next->prev = NULL;
    else
        d->tail = NULL;

    int temp = q->data;
    free(q);
    d->size--;
    
    return temp;
}

/* This function modifies queue when there is cache hit */
void update_cache(dummy *d, queue_l *hash[], int page){

    /* If the page is at the front of queue, dequeue and 
    enqueue it again */    
    if(d->front == hash[page]){
        dequeue_l(d);
        enqueue_l(d, page);
    }
    /* If it is last in queue, do not do anything */
    else if(d->tail == hash[page]){
        return;
    }
    /* Update the pointers of neighbor nodes and tail in dummy node */
    else{
        queue_l * q = hash[page];
        q->prev->next =  q->next;
        q->next->prev = q->prev;
        q->prev = d->tail;
        d->tail->next = q;
        d->tail  = q;
        q->next = NULL;
    }
}
/* This function implements the LRU cache */
int lru_cache(dummy *d, queue_l *hash[], int page){

    /*Cache Miss and we can add the page in the cache */
    if(hash[page] == NULL && d->size < MAX_CACHE_SIZE){
        hash[page] = enqueue_l(d, page);
    }
    
    /* Cache Miss and we cannot add new page to cache, remove the LRU page */
    if(hash[page] == NULL && d->size == MAX_CACHE_SIZE){
        dequeue_l(d);
        enqueue_l(d, page);
    }

    /* Cache is Hit */
    if(hash[page] != NULL){
        update_cache(d,hash,page);
 }

 print_queue(d);
 return true;
}
void main(){

    dummy *d;
    queue_l * hash[10];

    int i, n;

    for(i=0; i<10; i++)
        hash[i] = NULL;

    init_queue_l(&d);

    int page_order[10] = {1,2,3,5,4,2,3,2,5,6};
    i =0;
    while(i<10) {
        printf("\n Page Requested : %d " , page_order[i]);
        lru_cache(d, hash, page_order[i]);
        i++;
    };
}
