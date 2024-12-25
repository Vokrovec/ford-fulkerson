#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#define MIN(a,b) (((a)<(b))?(a):(b))
#define SIZE 3

typedef struct{
  int max_size;
  int top;
  int* array;
} Stack;

typedef struct {
  int max;
  int flow;
} pipe;

Stack stack_init(int max_size){
  Stack stack;
  stack.top = -1;
  stack.max_size = max_size;
  stack.array = (int*) calloc(max_size, sizeof(int));
  return stack;
}

void push(Stack *stack, int value){
  if (stack->top >= stack->max_size-1){
    puts("Stack overflow");
    return;
  }
  stack->top++;
  stack->array[stack->top] = value;
}

int pop(Stack *stack){
  if (stack->top <= -1){
    puts("Stack underflow");
    return -1;
  }
  int value = stack->array[stack->top];
  stack->top--;
  return value;
}

void add_edge(int a, int b, int v, pipe sou_mat[]){
  sou_mat[SIZE*a+b].max = v;
  sou_mat[SIZE*b+a].max = v;
  sou_mat[SIZE*b+a].flow = v;
}

int DFS(int start, int end, pipe sou_mat[], int visited[]){
  for (int i=0; i<SIZE; i++){
    visited[i] = -1;
  }
  Stack stack = stack_init(SIZE);
  push(&stack, start);
  while (stack.top >= 0){
    int v = pop(&stack);
    for (int i=0; i<SIZE; i++){
      pipe val = sou_mat[SIZE*v+i];
      if (visited[i] == -1 && (val.max - val.flow) > 0){
        visited[i] = v;
        push(&stack, i);
        if (i==end) {
          free(stack.array);
          return 1;
        }
      }
    }
  }
  free(stack.array);
  return 0;
}

int ford_fulkerson(pipe sou_mat[], int s, int t){
  int* visited = (int*) calloc(SIZE*SIZE, sizeof(int));
  int max_flow = 0;
  int curr_flow;
  while (DFS(s, t, sou_mat, visited)){
    curr_flow = 1000000000;
    int i = t;
    int p;
    while (i>=0){
      p = visited[i];
      if (p == -1) break;
      curr_flow = MIN((sou_mat[SIZE*i+p].flow), curr_flow);
      i = p;
    }
    i = t;
    while (i>=0){
      p = visited[i];
      if (p == -1) break;
      sou_mat[SIZE*p+i].flow += curr_flow;
      sou_mat[SIZE*i+p].flow -= curr_flow;
      i = p;
    }
    max_flow += curr_flow;
  }
  free(visited);
  return max_flow;
}


int main(){
  pipe* sou_mat = (pipe*) calloc(SIZE*SIZE, sizeof(pipe));
  if (sou_mat==NULL) exit(0);

  //add adges to graph
  add_edge(0, 1, 5, sou_mat);
  add_edge(1, 2, 4, sou_mat);

  int s = 0; //starting point
  int t = 2; //end point
  for (int i=0; i<SIZE; i++){
    for (int j=0; j<SIZE; j++){
      printf("%d ", sou_mat[SIZE*i+j].flow);
    }
    printf("\n");
  }
  int a = ford_fulkerson(sou_mat, s, t);
  for (int i=0; i<SIZE; i++){
    for (int j=0; j<SIZE; j++){
      printf("%d ", sou_mat[SIZE*i+j].flow);
    }
    printf("\n");
  }
  free(sou_mat);
  printf("Maximální tok je %d.",a);
}
