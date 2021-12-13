

/* Если используем UNIX, то подключаем <stdlib.h>*/
#ifndef __unix__

#include <stdlib.h>
#include <string.h>

/* Если не используем UNIX или Win */
#else

#endif

#include "ringbuffer.h"


void cb_init(circular_buffer *cb, size_t capacity, size_t sz)
{
    cb->buffer = malloc(capacity * sz);
    if(cb->buffer == NULL){
    }
        // handle error
    cb->buffer_end = (char *)cb->buffer + capacity * sz;
    cb->capacity = capacity;
    cb->count = 0;
    cb->sz = sz;
    cb->write_counter = cb->buffer;
    cb->read_counter = cb->buffer;
}


void cb_free(circular_buffer *cb)
{
    free(cb->buffer);
    // clear out other fields too, just to be safe
}


void cb_write_item(circular_buffer *cb, void *item)
{
    if(cb->count == cb->capacity){
        // handle error
    }
    memcpy(cb->write_counter, item, cb->sz);
    cb->write_counter = (char*)cb->write_counter + cb->sz;
    if(cb->write_counter == cb->buffer_end)
        cb->write_counter = cb->buffer;
    cb->count++;
}


void cb_read_item(circular_buffer *cb, void *item)
{
    if(cb->count == 0){
        // handle error
    }
    memcpy(item, cb->read_counter, cb->sz);
    cb->read_counter = (char*)cb->read_counter + cb->sz;
    if(cb->read_counter == cb->buffer_end)
        cb->read_counter = cb->buffer;
    cb->count--;
}

void cb_skip_all(circular_buffer *cb)
{
    cb->read_counter = cb->write_counter;
    cb->count = 0;
}
