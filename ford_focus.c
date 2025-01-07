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

void add_edge(int a, int b, int v, pipe matice_souslednosti[]){
  matice_souslednosti[SIZE*a+b].max = v;
  matice_souslednosti[SIZE*b+a].max = v;
  matice_souslednosti[SIZE*b+a].flow = v;
}

int DFS(int start, int end, pipe matice_souslednosti[], int visited[]){
  for (int i=0; i<SIZE; i++){
    visited[i] = -1;
  }
  Stack stack = stack_init(SIZE);
  push(&stack, start);
  while (stack.top >= 0){
    int v = pop(&stack);
    for (int i=0; i<SIZE; i++){
      pipe val = matice_souslednosti[SIZE*v+i];
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

int ford_fulkerson(pipe matice_souslednosti[], int s, int t){
  int* visited = (int*) calloc(SIZE*SIZE, sizeof(int));
  int max_flow = 0;
  int curr_flow;
  while (DFS(s, t, matice_souslednosti, visited)){
    curr_flow = 1000000000;
    int i = t;
    int p;
    while (i>=0){
      p = visited[i];
      if (p == -1) break;
      curr_flow = MIN((matice_souslednosti[SIZE*i+p].flow), curr_flow);
      i = p;
    }
    i = t;
    while (i>=0){
      p = visited[i];
      if (p == -1) break;
      matice_souslednosti[SIZE*p+i].flow += curr_flow;
      matice_souslednosti[SIZE*i+p].flow -= curr_flow;
      i = p;
    }
    max_flow += curr_flow;
  }
  free(visited);
  return max_flow;
}

int main(){
  pipe* matice_souslednosti = (pipe*) calloc(SIZE*SIZE, sizeof(pipe));
  if (matice_souslednosti==NULL) exit(0);

  //add adges to graph
  add_edge(0, 1, 5, matice_souslednosti);
  add_edge(1, 2, 4, matice_souslednosti);
  add_edge(0, 2, 4, matice_souslednosti);

  int s = 0; //starting point
  int t = 2; //end point
  for (int i=0; i<SIZE; i++){
    for (int j=0; j<SIZE; j++){
      printf("%d ", matice_souslednosti[SIZE*i+j].flow);
    }
    printf("\n");
  }
  int max_flow = ford_fulkerson(matice_souslednosti, s, t);
  for (int i=0; i<SIZE; i++){
    for (int j=0; j<SIZE; j++){
      printf("%d ", matice_souslednosti[SIZE*i+j].flow);
    }
    printf("\n");
  }
  free(matice_souslednosti);
  printf("Maximální tok je %d.", max_flow);
}
