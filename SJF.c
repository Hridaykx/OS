// Program to input an array of processes with their arrival times & burst times, and schedule them using to Shortest Job First CPU Scheduling
#include <stdio.h>
#include <limits.h>

typedef struct
{
  int id;
  int arrivalTime;
  int burstTime;
  int completionTime;
  int turnAroundTime;
  int waitingTime;
  int isDone; // flag to mark a process once it has been executed
} pcb;

pcb p[10]; // array of processes.

void swap(pcb *a, pcb *b)
{
  pcb temp;
  temp = *a;
  *a = *b;
  *b = temp;
}

void sort(pcb a[], int l)
{
  for (int i = 0; i < l; i++)
    for (int j = 0; j < l - i - 1; j++)
    {
      if (a[j].arrivalTime > a[j + 1].arrivalTime)
      {
        swap(&a[j], &a[j + 1]);
      }
    }
}

pcb finalOrder[10]; // array to store the processes in correct order
int f = 0;          // index of the final array

void execute(int processIndex, int n)
{
  p[processIndex].isDone = 1;
  finalOrder[f] = p[processIndex];
  printf("\nExecuting Process %d for %d seconds", p[processIndex].id, p[processIndex].burstTime);
  f++;
}

void sjf(pcb p[], int n)
{
  int currTime = 0;
  int index = 0;

  // loop to ensure that all the processes gets executed
  for (int i = 0; i < n; i++)
  {
    int minBurstTime = INT_MAX; // assume Infinite burst time
    int minBurstTimeProcessIndex;

    // loop to find the process of minimum burst time among the process having arrival time less than currentTime
    for (int index = 0; index < n; index++)
    {
      // ignore all the processes which are marked as done
      if (p[index].isDone == 0 && p[index].arrivalTime <= currTime && p[index].burstTime < minBurstTime)
      {
        minBurstTimeProcessIndex = index;
        minBurstTime = p[minBurstTimeProcessIndex].burstTime;
      }
    }
    // execute the process with minimum burst time and mark it as done
    execute(minBurstTimeProcessIndex, n);
    // cpu will take the (burst time of the process) seconds from the current time to execute the process.
    currTime += p[minBurstTimeProcessIndex].burstTime;
  }
}

void calculateValues(pcb a[], int l)
{
  a[0].completionTime = a[0].arrivalTime + a[0].burstTime;
  for (int i = 0; i < l; i++)
  {
    a[i].completionTime = a[i - 1].completionTime + a[i].burstTime;

    int diff = a[i].arrivalTime - a[i - 1].completionTime;
    if (diff > 0)
      a[i].completionTime += diff;

    a[i].turnAroundTime = a[i].completionTime - a[i].arrivalTime;
    a[i].waitingTime = a[i].turnAroundTime - a[i].burstTime;
  }
}

void display(pcb a[], int l)
{
  printf("\nProces Id | Arrival Time | Burst Time | Completion Time | TurnAroundTime | WaitingTime");
  for (int i = 0; i < l; i++)
    printf("\n%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d", a[i].id, a[i].arrivalTime, a[i].burstTime, a[i].completionTime, a[i].turnAroundTime, a[i].waitingTime);
}

void getData(int n)
{
  for (int i = 0; i < n; i++)
  {
    printf("\nEnter arrival time of process %d: ", i + 1);
    scanf("%d", &p[i].arrivalTime);
    printf("Enter burst time of process %d: ", i + 1);
    scanf("%d", &p[i].burstTime);
    p[i].id = i + 1;
    p[i].isDone = 0;
  }
}

int main()
{
  int n;
  printf("----SJF CPU SCHEDULING ALGORITHM----\n");
  printf("Enter the number of processes: ");
  scanf("%d", &n);
  getData(n);

  printf("\nInput order:");
  display(p, n);
  sort(p, n); // when 2 processes may have same burst times, the one with the earliest arrival time will be selected
  sjf(p, n);
  calculateValues(finalOrder, f);

  printf("\n\nAfter applying SJF algorithm:");
  display(finalOrder, f);
  printf("\n");
}
