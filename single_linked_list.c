#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct Node{
   uint32_t data;
   uint8_t id;
   struct Node *nextNode;
}node;

node *head, *prev;
uint8_t headNodeFlag = 1;

void add_node(uint32_t data, uint8_t id)
{
    if(headNodeFlag == 1)
    {
       headNodeFlag = 0;
       head -> data = data;
       head -> id = id;
       head -> nextNode = NULL;
       prev = head;
    }
    else if(headNodeFlag == 0)
    {
        node *temp = (node*)malloc(sizeof(node));
        prev -> nextNode = temp;
        temp -> data = data;
        temp -> id = id;
        temp -> nextNode = NULL;
        prev = temp;
    }
}

uint32_t get_data_from_id(uint32_t id)
{
    node *temp_head = head;
    while(temp_head -> nextNode != NULL)
    {
        if(temp_head->id == id)
        {
            return temp_head -> data;
        }
        temp_head = temp_head -> nextNode;
    }
    if(temp_head->id == id)
    {
        return temp_head -> data;
    }
    else return 0;
}

uint8_t get_id_from_data(uint32_t data)
{
    node *temp_head = head;
    while(temp_head -> nextNode != NULL)
    {
        if(temp_head->data == data)
        {
            return temp_head -> id;
        }
        temp_head = temp_head -> nextNode;
    }
    if(temp_head->id == data)
    {
        return temp_head -> id;
    }
    else return 0;
}

void print_linkedList(void)
{
    node *temp_head = head;
    while(temp_head != NULL)
    {
        printf("Data = %d, id = %d\n",temp_head->data, temp_head->id);
        temp_head = temp_head -> nextNode;
    }
}

void reverse_linked_list(void)
{
    node *prev = NULL;
    node *curr = head;
    node *next = NULL;

    while(curr->nextNode != NULL)
    {
        next = curr->nextNode;
        curr->nextNode = prev;
        prev = curr;
        curr = next;
    }
    curr->nextNode = prev;
    head = curr;
}

void printLinkedListStructure(void)
{
    node *temp_head = head;
    printf("Head Address : %p\n",temp_head);
    while(temp_head != NULL)
    {
        printf("Data = %d, id = %d, next Node Address = %p\n",temp_head->data, temp_head->id, temp_head->nextNode);
        temp_head = temp_head -> nextNode;
    }
}

void deleteNode(uint8_t id)
{
    node *temp_head = head;
    node *prev = NULL;
    while(temp_head != NULL)
    {
        if(temp_head->id == id)
        {
            break;
        }
        prev = temp_head;
        temp_head = temp_head -> nextNode;
    }
    if(temp_head == head)
    {
        head = temp_head -> nextNode;
    }
    else if(temp_head != head)
    {
        prev -> nextNode = temp_head -> nextNode;
    }
    free(temp_head);
}


void insertNode(uint8_t position, uint32_t data, uint8_t id)    //Indexed from 0
{
    uint8_t counter = 0;
    node *temp_head = head;
    node *prev = NULL;
    while(temp_head != NULL)
    {
        if(counter == position)
        {
            break;
        }
        prev = temp_head;
        temp_head = temp_head -> nextNode;
        counter++;
    }
    node *newNode = (node*)malloc(sizeof(node));
    newNode -> data = data;
    newNode -> id = id;
    newNode -> nextNode = temp_head;
    if(position != 0)
    {
        prev -> nextNode = newNode;
    }
    if(position == 0)
    {
        head = newNode;
    }
}

node* printMiddleOfLinkedList()
{
    node *fast =  head;
    node *slow = head;
    uint8_t LocCounter = 0;
    while (fast && fast->nextNode) {
        slow = slow->nextNode;
        fast = fast->nextNode->nextNode;
        LocCounter+=2;
    }
    LocCounter += 1;
    printf("Local Counter = %d\n",LocCounter);
    return slow;
}

int main()
{
    head = (node*)malloc(sizeof(node));
    add_node(100,1);
    add_node(200,2);
    add_node(300,3);
    add_node(400,4);
    add_node(500,5);
    printLinkedListStructure();
    printf("\n");
    deleteNode(1);
    insertNode(0,1000,6);
    insertNode(6,2000,61);
    insertNode(3,1300,62);
    printLinkedListStructure();
    printf("\n");
    reverse_linked_list();
    printLinkedListStructure();
    printf("\n");
    node *middleNode = printMiddleOfLinkedList();
    printf("Middle of LinkedList is:\n");
    printf("Data = %d\nId = %d\n",middleNode->data,middleNode->id);
    //printf("ID of data 300 is %d\n",get_id_from_data(300));
    //printf("ID of data 100 is %d\n",get_id_from_data(100));
    //printf("Data of ID 3 is %d\n",get_data_from_id(3));
    //printf("Data of ID 1 is %d\n",get_data_from_id(1));
}
