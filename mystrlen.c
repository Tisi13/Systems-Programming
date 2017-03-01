
#include <stdio.h>

int mystrlen(char *input_string) {
   /* This function returns the length of the input string */
   
   int i = 0;
   while(*input_string != '\0') {
       i++;
       input_string++;
   }
   return i;
}

int main(int argc, char ** argv) {
  int length;
  if (argc!=2) {
    printf("Usage: mystrlen <input_string_with_no_space_inside_it>\n\n");
    return 1;
  }
  length = mystrlen(argv[1]);
  printf("The length is: %d characters\n",length);
  return 0;
}
