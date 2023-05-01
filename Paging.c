#include<stdio.h>

void main() {
  int page_size, memory_size, nop, process_no, process_size, remp, count = 0, index = 1;

  printf("\nEnter memory size -> ");
  scanf("%d", & memory_size);

  printf("enter frame size -> ");
  scanf("%d", & page_size);

  nop = memory_size / page_size;

  printf("\nno. of pages = %d", nop);
  printf("\nenter no. of process -> ");
  scanf("%d", & process_no);

  remp = nop;

  printf("\n");
  int frame[process_no], size[process_no], fragment[process_no];

  for (int i = 0; i < process_no; i++) {

    printf("enter the size of p[%d] -> ", i + 1);
    scanf("%d", & size[i]);
    fragment[i] = size[i] % page_size;

    if (fragment[i] != 0)
      frame[i] = size[i] / page_size + 1;
    else
      frame[i] = size[i] / page_size;

    if (frame[i] > remp) {
      printf("\nmemory full\n");
      break;
    } else {
      remp -= frame[i];
      count++;
    }
  }
  
  if (count == process_no) {
    printf("\nall processes stored in memory and executed \n");
    for (int i = 0; i < process_no; i++) {
      printf("\np[%d]\n----\n", i + 1);
      if (fragment[i] == 0)
        for (int j = 0; j < frame[i]; j++, index++)
          printf("page %d ----> frame %d\n", j + 1, index);
      else {
        int j;
        for (j = 0; j < frame[i] - 1; j++, index++)
          printf("page %d ----> frame %d\n", j + 1, index);
        printf("page %d ----> frame %d\t\tinternal fragmentation!\tunused space = %d\n", j + 1, index, page_size - fragment[i]);
        index++;
      }
    }
  }

}
