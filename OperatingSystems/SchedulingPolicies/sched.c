/**
* Name: Drew Garrison
* HW2
* 7/8/2022
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 27

struct task{
    int
    task_id,
    arrival_time,
    service_time,
    remaining_time,
    completion_time,
    response_time,
    wait_time;
struct task *next;
};

struct task taskArray[MAX];
int time = 0;
int tasks = 0;
int curr = 0;

void printTask(FILE *output)
{
    fprintf(output, "\n\n\t  arrival  service  completion  response  wait\n");
    fprintf(output, "tid   time     time     time        time      time\n");
    fprintf(output, "-------------------------------------------------\n");
    for(int i = 0; i < tasks; i++)
    {
        fprintf(output, "%c %6d %7d %9d %10d %10d\n", 'A'+taskArray[i].task_id, taskArray[i].arrival_time, taskArray[i].service_time, taskArray[i].completion_time, taskArray[i].response_time, taskArray[i].wait_time);
    }
}

void sortServiceTime()
{
    for (int i = 0; i < tasks; i++)
    {
        for (int j = 0; j < tasks-i-1; j++)
        {
            if (taskArray[j].service_time > taskArray[j+1].service_time)
            {
                struct task temp = taskArray[j];
                taskArray[j] = taskArray[j+1];
                taskArray[j+1] = temp;
            }
        }
    }
}

void sortID()
{
    for (int i = 0; i < tasks; i++)
    {
        for (int j = 0; j < tasks-i-1; j++)
        {
            if (taskArray[j].task_id > taskArray[j+1].task_id)
            {
                struct task temp = taskArray[j];
                taskArray[j] = taskArray[j+1];
                taskArray[j+1] = temp;
            }
        }
    }
}

void printServiceTime(FILE *output)
{
    sortServiceTime();
    fprintf(output, "\n\nService\twait\ntime\ttime\n-------\t----\n");
    for(int i = 0; i < tasks; i++)
    {
        fprintf(output, "%d\t\t%d\n", taskArray[i].service_time, taskArray[i].wait_time);
    }
}

void sjf(FILE *output)
{
    fprintf(output, "SJF(preemptive) scheduling results\n\n");
    fprintf(output, "time\tcpu\t ready\tqueue\t(tid/rst)\n");
    fprintf(output, "--------------------------------------------\n");
    while(curr < tasks)
    {
        if(taskArray[curr].arrival_time <= time)
        {
            for(int i = 0; i < taskArray[curr].service_time; i++)
            {
                
                if(taskArray[curr+1].arrival_time <= time && taskArray[curr+1].remaining_time != 0)
                {
                    if(taskArray[curr].remaining_time > taskArray[curr+1].service_time)
                    {
                        struct task temp = taskArray[curr];
                        taskArray[curr] = taskArray[curr+1];
                        taskArray[curr+1] = temp;
                    }
                    
                    if(taskArray[curr+2].arrival_time <= time && taskArray[curr+2].remaining_time != 0)
                    {
                        if(taskArray[curr].remaining_time > taskArray[curr+2].service_time)
                        {
                            struct task temp = taskArray[curr];
                            taskArray[curr] = taskArray[curr+2];
                            taskArray[curr+2] = temp;
                        }
                        fprintf(output, "%d\t\t%c%d\t\t", time, 'A'+taskArray[curr].task_id, taskArray[curr].remaining_time--);
                        fprintf(output, "%c%d",'A'+taskArray[curr+2].task_id, taskArray[curr+2].remaining_time);
                        taskArray[curr+2].wait_time++;
                        fprintf(output, ", %c%d\n",'A'+taskArray[curr+1].task_id, taskArray[curr+1].remaining_time);
                        taskArray[curr+1].wait_time++;
                    }
                    else
                    {
                        fprintf(output, "%d\t\t%c%d\t\t", time, 'A'+taskArray[curr].task_id, taskArray[curr].remaining_time--);
                        fprintf(output, "%c%d",'A'+taskArray[curr+1].task_id, taskArray[curr+1].remaining_time);
                        taskArray[curr+1].wait_time++;
                        fprintf(output, "\n");
                    }
                }
                else
                {
                    fprintf(output, "%d\t\t%c%d\t\t", time, 'A'+taskArray[curr].task_id, taskArray[curr].remaining_time--);
                    fprintf(output, "--\n");
                }
                time++;
            }
            taskArray[curr].response_time = taskArray[curr].service_time + taskArray[curr].wait_time;
            taskArray[curr].completion_time = time;
            curr++;
        }
        else
        {
            fprintf(output, "%d\t\t--\t\t--\n", time);
            time++;
        }
    }

    sortID();
    
    
}

void fifo(FILE *output)
{
    fprintf(output, "FIFO scheduling results\n\n");
    fprintf(output, "time\tcpu\t ready\tqueue\t(tid/rst)\n");
    fprintf(output, "--------------------------------------------\n");
    
    while(curr < tasks)
    {
        if(taskArray[curr].arrival_time <= time)
        {
            taskArray[curr].response_time = taskArray[curr].service_time + taskArray[curr].wait_time;

            for(int i = 0; i < taskArray[curr].service_time; i++)
            {
                fprintf(output, "%d\t\t%c%d\t\t", time, 'A' + taskArray[curr].task_id, taskArray[curr].remaining_time--);
                if(taskArray[curr+1].arrival_time <= time && taskArray[curr+1].remaining_time != 0)
                {
                    fprintf(output, "%c%d",'A'+taskArray[curr+1].task_id, taskArray[curr+1].remaining_time);
                    taskArray[curr+1].wait_time++;
                    
                    if(taskArray[curr+2].arrival_time <= time && taskArray[curr+2].remaining_time != 0)
                    {
                        fprintf(output, ", %c%d\n",'A'+taskArray[curr+2].task_id, taskArray[curr+2].remaining_time);
                        taskArray[curr+2].wait_time++;
                    }
                    else
                    {
                        fprintf(output, "\n");
                    }
                }
                else
                {
                    fprintf(output, "--\n");
                }
                time++;
            }
            taskArray[curr].completion_time = time;
            curr++;
        }
        else
        {
            fprintf(output, "%d\t\t--\t\t--\n", time);
            time++;
        }
    }
    
}

void rr(FILE *output)
{
    fprintf(output, "RR scheduling results\n\n");
    fprintf(output, "time\tcpu \tready\tqueue\t(tid/rst)\n");
    fprintf(output, "--------------------------------------------\n");

    int beg = 1;
    int fin = 0;
    int j = 0;

    while(!fin)
    {
        if(taskArray[curr].arrival_time <= time)
        {
            if(taskArray[curr].remaining_time > 0)
            {
                fprintf(output, "%d\t\t%c%d\t\t", time, 'A' + taskArray[curr].task_id, taskArray[curr].remaining_time--);

                j = 0;
                for(int i = curr + 1; 1; i++)
                {
                    if(i >= tasks)
                    {
                        i = 0;
                    }
                    if((taskArray[i].arrival_time <= time) && (beg == 1) && (taskArray[i].task_id != taskArray[curr].task_id) && (taskArray[i].remaining_time > 0))
                    {
                        fprintf(output, "%c%d",'A'+taskArray[i].task_id, taskArray[i].remaining_time);
                        taskArray[i].wait_time++;
                        beg = 0;    
                    }
                    else if((taskArray[i].arrival_time <= time) && (taskArray[i].task_id != taskArray[curr].task_id) && (taskArray[i].remaining_time > 0))
                    {
                        fprintf(output, ", %c%d",'A'+taskArray[i].task_id, taskArray[i].remaining_time);
                        taskArray[i].wait_time++;
                    }
                    j++;
                    if(j >= tasks)
                    {
                        break;
                    }
                }
                if(beg == 1)
                {
                    fprintf(output, "--");
                }
                beg = 1;
                fprintf(output, "\n");
                time++;

                if((taskArray[curr].remaining_time == 0) && (taskArray[curr].completion_time == 0))
                {
                    taskArray[curr].completion_time = time;
                    taskArray[curr].response_time = taskArray[curr].service_time + taskArray[curr].wait_time;
                }
            }
            else
            {
                fprintf(output, "%d\t\t--\t\t--\n", time++); 
            }
            
            j = 0;
            for(int i = curr + 1; 1; i++)
            {
                
                if(i >= tasks)
                {
                    i = 0;
                }
                if((taskArray[i].arrival_time <= time) && (taskArray[i].remaining_time > 0))
                {
                    curr = i;
                    break;
                }
                j++;
                if(j >= tasks)
                {
                    break;
                }
            }

            for(int i = 0; i < tasks; i++)
            {
                if(taskArray[i].remaining_time > 0)
                {
                    fin = 0;
                    break;
                }
                else
                {
                    fin = 1;
                }
            }
        }
        else
        {
            fprintf(output, "%d\t\t--\t\t--\n", time++);
        }

    }

    sortID();
    
}


int main(int argc, char* argv[])
{
    if(argc != 4)
        printf("Please enter correct arguments\n");

    char *input = argv[2];
    char *output = argv[3];
    FILE *infile = fopen(input, "r");
    FILE *outfile = fopen(output, "w");
    
    
    for(int i = 0; fscanf(infile, "%d %d", &taskArray[i].arrival_time, &taskArray[i].service_time) != EOF; i++)
    {
        taskArray[i].task_id = i;
        taskArray[i].remaining_time = taskArray[i].service_time;
        taskArray[i].completion_time = 0;
        taskArray[i].response_time = 0;
        taskArray[i].wait_time = 0;
        tasks = i + 1;
    }

    fclose(infile);
    
    if (strcmp(argv[1],"-fifo") == 0)
    {
        fifo(outfile);
    }    
    else if (strcmp(argv[1],"-sjf") == 0)
    {
        sjf(outfile);
    }
    else if (strcmp(argv[1],"-rr") == 0)
    {
        rr(outfile);
    }    
    else
    {
        printf("Please enter correct arguments\n");
        return -1;
    }     

    printTask(outfile);
    printServiceTime(outfile);
    fclose(outfile);
    return 0;
}
