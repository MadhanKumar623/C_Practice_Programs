#include <stdint.h>
#include <stdio.h>

#define RING_BUFFER_SIZE 5

int8_t ring_buffer[RING_BUFFER_SIZE];
uint8_t head = 0, tail = 0;
uint8_t ring_buffer_counter = 0;

void push_to_ringBuffer(int8_t data)
{
    if(ring_buffer_counter == RING_BUFFER_SIZE)
    {
        printf("Ring Buffer is Full, Can't Push More\n");
        return;
    }
    ring_buffer[head] = data;
    head = (head+1)%RING_BUFFER_SIZE;
    ring_buffer_counter++;
}

int8_t pop_from_ringBuffer(void)
{
    int8_t ret_val;

    if(ring_buffer_counter == 0)
    {
        printf("No Elements in the Ring Buffer\n");
        return -99;
    }
    ret_val = ring_buffer[tail];
    tail = (tail+1)%RING_BUFFER_SIZE;
    ring_buffer_counter--;

    return ret_val;
}

void printRingBuffer(void)
{
    if(ring_buffer_counter == 0)
    {
        printf("No elements to Print\n");
        return;
    }
    uint8_t j=0;
    while(j < ring_buffer_counter)
    {
        printf("%d ",ring_buffer[(tail+j)%RING_BUFFER_SIZE]);
        j++;
    }
    printf("\n");
}

int main(void)
{
   int8_t data;
   push_to_ringBuffer(1);
   push_to_ringBuffer(2);
   push_to_ringBuffer(3);
   push_to_ringBuffer(4);
   push_to_ringBuffer(5);
   printRingBuffer();
   //push_to_ringBuffer(6);
   printf("Popped value: %d\n",pop_from_ringBuffer());
   printf("Popped value: %d\n",pop_from_ringBuffer());
   printf("Popped value: %d\n",pop_from_ringBuffer());
   printf("Popped value: %d\n",pop_from_ringBuffer());
   //printf("Popped value: %d\n",pop_from_ringBuffer());
   //printf("Popped value: %d\n",pop_from_ringBuffer());

   printRingBuffer();

}

