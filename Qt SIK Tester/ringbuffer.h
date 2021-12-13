#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#ifdef __cplusplus
extern "C"
{
#endif

//typedef unsigned int size_t;

typedef struct circular_buffer
{
    void *buffer;     // data buffer
    void *buffer_end; // end of data buffer
    unsigned int capacity;  // maximum number of items in the buffer
    unsigned int count;     // number of items in the buffer
    unsigned int sz;        // size of each item in the buffer
    void *write_counter;       // pointer to head
    void *read_counter;       // pointer to tail
} circular_buffer;

void cb_write_item(circular_buffer *cb, void *item);
void cb_read_item(circular_buffer *cb, void *item);
void cb_free(circular_buffer *cb);
void cb_init(circular_buffer *cb, size_t capacity, size_t sz);
void cb_skip_all(circular_buffer *cb);

#ifdef __cplusplus
}
#endif

#endif // RINGBUFFER_H
