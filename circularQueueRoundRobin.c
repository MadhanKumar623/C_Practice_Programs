#include <stdio.h>
#include <stdint.h>

#define MAX_JOBS 5

uint32_t JobArray[MAX_JOBS];
int8_t front = -1, rear = -1;

void add_to_job_queue(uint32_t data)
{
    if(front==((rear+1)%MAX_JOBS))
    {
       printf("Job Queue Full, rear value = %d, front value = %d\n",rear,front);
       return;
    }
    rear = (rear+1)%MAX_JOBS;
    JobArray[rear] = data;
    if(front == -1)
        front = rear;
}

uint32_t remove_from_job_queue(void)
{
    uint32_t ret_value;

    if(front==-1)
    {
       printf("Job Queue Empty\n");
       return 9999;
    }
    ret_value = JobArray[front];
    if(front==rear)
      front = rear = -1;
    else
      front=(front+1)%MAX_JOBS;

    return ret_value;
}

void view_job_list(void)
{
    if (front == -1) {
        printf("Job Queue is Empty\n");
        return;
    }

    printf("Job Queue: ");
    int i = front;
    while (1) {
        printf("%u ", JobArray[i]);
        if (i == rear) break;
        i = (i + 1) % MAX_JOBS;
    }
    printf("\n");
}

int main()
{
    uint8_t ch;
    printf("Lets see on Round Robin scheduling\nEnter 1 to view Jobs List\nEnter 2 to Add a Job\nEnter 3 to Remove the Job\n");
    while(1)
    {
        printf("\nEnter the Option (1/2/3): ");
        scanf("%d",&ch);
        switch(ch)
        {
            case 1:
                view_job_list();
                break;
            case 2:
                uint32_t tempData;
                printf("Enter the Job ID to add: ");
                scanf("%d",&tempData);
                add_to_job_queue(tempData);
                break;
            case 3:
                uint32_t tempData1;
                tempData1 = remove_from_job_queue();
                if(tempData1 != 9999)
                {
                    printf("Removed Job ID is: %d\n",tempData1);
                }
                break;
            default:
                printf("Wrong Input. Enter valid Input.\n");
                break;
        }
    }
}
