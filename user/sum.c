#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  char c; int ret_value;
  int MAX_LEN = 50;
  char buf[MAX_LEN];
  int i;
  for(i = 0; i+1 < MAX_LEN; ){
    ret_value = read(0, &c, 1);
    if(ret_value <= 0)
      break;
    buf[i++] = c;
    if (c == '\n' || c == '\r')
      break;
  }
  if (ret_value < 0){
    fprintf(2, "%s\n", "Reading error");
    exit(1);
  }
  if (i == 0){
    fprintf(2, "%s\n", "Nothing entered");
    exit(1);
  }
  if (i + 1 == MAX_LEN){
    fprintf(2, "Error: %s\n", "Buffer is overflowed");
    exit(1);
  }
  buf[i-1] = '\0';
  printf("|%s|\n", buf);
  
  char first[MAX_LEN]; char second[MAX_LEN];
  i = 0;
  while (buf[i] != ' ' && buf[i] != '\0'){
    first[i] = buf[i];
    i++;
  }
  if (buf[i] == '\0'){
    fprintf(2, "Error: %s\n", "Second number is empty");
    exit(1);  
  }
  if (strlen(first) == 0){
    fprintf(2, "Error: %s\n", "First number is empty");
    exit(1);
  }
  first[i] = '\0';
  int k = 0;
  while (first[k] != ' ' && first[k] != '\0'){
      if ('0' > first[k] || first[k] > '9')
         break;
     k++;
  }
  if (k != i){
     fprintf(2, "Error: %s\n", "There are letters in first number");
     exit(1);
  }

  int a = atoi(first);
  
  int j = 0; i ++;
  while (buf[i] != '\0'){
    second[j] = buf[i];
    i++; j++;
  }

  if (strlen(second) == 0){
    fprintf(2, "Error: %s\n", "Second number is empty");
    exit(1);
  }

  if (second[0] == ' ') {
     fprintf(2, "Error: %s\n", "More than 1 space");
     exit(1);
  }
  second[j] = '\0';
  k = 0;
  while (second[k] != ' ' && second[k] != '\0'){
      if ('0' > second[k] || second[k] > '9')
         break;
     k++;
  }
  if (k != j){
     fprintf(2, "Error: %s\n", "There are letters in second number");
     exit(1);
  }
  int b = atoi(second);

  printf("Sum: %d\n", add(a, b));

  exit(0);
}
