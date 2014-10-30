//Tutor: Thomas
//Bearbeiter: Jasmine Cavael, Maximilian Stendler

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define length(x) (sizeof(x)/sizeof(x[0]))

int len(char *c){
  int length = 0;
  while ((c[length]!=0) && (c[length]!=127)){
    printf("%c %d\n",c[length],c[length]);
    length++;
  }
  return length;
}

void cpCharArray(char *c1, char *c2){
  //DEBUG
  printf("\n%d\n%d\n",length(c1),length(c2));
  if(length(c1)==length(c2)){
    for(int i = 0;
        i<len(c1);
        i++){
        c1[i] = c2[i];
    }
  }
}

char charToUpper(char c){
  char ret = c;
  if((ret>=97) && (ret<=122)){
    ret -= 32;
  }
  return ret;
}

char* toUpper(char *c){

  char ret[length(c)];
  cpCharArray(ret,c);

  for(int i=0;
      i<length(ret)-1;
      i++)
  {
      ret[i] = charToUpper(ret[i]);
  }
  return ret;
}

int main(){
  char codewort[11];
  char text[101];


  printf("Text (max 100): ");
  scanf("%[^\n]s",text);

  //Laenge der reinen Texteingabe
  int leng = len(text);

  printf("Codewort (max. 10): ");
  scanf("%s",codewort);

  //DEBUG
  printf("\n %s \n %s len: %d \n %s\n",codewort, text,leng,toUpper(text));

  return 0;
}
