//ASCII Table
//prints a Table of all Ascii codes in char range (-127 to 127)
//Hilfsklasse zum Testen

#include <stdio.h>

int main(){
  int i = 0;
  for(char c = -127;c<127;c++){
      printf(" | %d | %c | ",c,c);
      if((i % 4) == 0){
        printf("\n");
      }
    if(++i>256){
      break;
    }
  }

  return 0;
}
