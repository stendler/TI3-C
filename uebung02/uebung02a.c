//Tutor: Thomas
//Bearbeiter: Jasmine Cavael, Maximilian Stendler

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//aendert einen Kleinbuchstaben in einen Großbuchstaben
/*
* @input: klein
* @return: GROSS
*/
char charToUpper(char c){
  char ret = c;
  if((ret>=97) && (ret<=122)){
    ret -= 32;
  }
  return ret;
}

//soll alle Kleinbuchstaben in einem Array in Großbuchstaben aendern
void toUpper(char *c, int len){
  for(int i=0;
      i<len;
      i++)
  {
      c[i] = charToUpper(c[i]);
  }
}

//entfernt alle Leerzeichen aus einem Array von chars
int killSpace(char *c, int len){
  int ret = len;
  int j = 0;
  for(int i = 0;
      i < len;
      i++)
  {
      if(c[j]==32){
        j++;
      }

      //DEBUG
      //printf("%d %c %d %c \n",i,c[i],j,c[j]);

      c[i] = c[j];
      j++;
      if(j > len){
        c[i] = 0;
      }
      if(j == len){
        ret = i+1;
        //len = i;
        //break;
      }
  }
  return ret;
}

int main(){
  char codewort[11];
  char text[101];


  printf("Text (max 100): ");
  scanf("%[^\n]s",text);

  printf("Codewort (max. 10): ");
  scanf("%s",codewort);

  //Laenge der reinen Texteingabe
  int leng = sizeof text / sizeof *text;
  int clen = sizeof codewort / sizeof *codewort;
  printf("%d %d\n",leng,clen);

  //DEBUG -- laengen Messung
  //text
  leng = 0;
  while(text[leng]!=0 && text[leng] != 127){
    leng++;
  }
  clen = 0;
  while(codewort[clen]!=0 && codewort[clen] != 127){
    clen++;
  }

  printf("%d %d\n",leng,clen);
  //DEBUG
  //printf("\n %s \n %s len: %d \n",codewort, text,leng);

  //Laut Aufgabenstellung keine Kleinbuchstaben
  toUpper(text,leng);
  toUpper(codewort,clen);

  //DEBUG
  //printf("%s\n",text);

  //Laut Aufgabenstellung keine Leerzeichen
  leng = killSpace(text, leng);

  //DEBUG
  printf("%s\n",text);
  printf("%s\n",codewort);
  printf("%d %d\n",leng,clen);


  //Codewort auf Länge des Textes bringen
  //»»
    char code[leng];
    for(int i = 0;i<leng;i++){
      int mod = i%clen;
      code[i] = codewort[mod];
      //DEBUG
      //printf("%d %d %c\n",i,mod,code[i]);
    }
    printf("%s\n",code);

  return 0;
}
