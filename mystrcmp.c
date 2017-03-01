#include <stdio.h>

int mystrcmp(char *string1, char *string2) {
/*This function compares two strings with each other*/
   while(*string1 == *string2) {
     if (*string1 == '\0' || *string2 == '\0') {
       break;
      }
       string1++;
       string2++;
    }   
   if(*string1=='\0' && *string2=='\0') {
     return 0;
   }
   else {
     return 1;
  }
}

int main(int argc, char **argv) {
   int result;
   if(argc != 3){
      printf("Usage: mystrcmp <input_two_strings_to_compare>\n\n");
      return 1;
   }
   result=mystrcmp( argv[1],argv[2]);
   if(result==0) {
   printf("The two strings are identical.\n");
   }
   else{
   printf("the two strings are different\n");
   }
   return 0;
}
   
    
