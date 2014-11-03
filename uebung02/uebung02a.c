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

//TEST killSpace mit rekursion:
/*
char testChar(char *c[], int i){
  printf("%c\n",c[i]);
  if(c[i] == 0 || c[i] == 127){
    return ;
  }else if((c[i]>=48 && c[i]<=57) || ((c[i]>=65) && (c[i]<=90))){
    return c[i];
  }else{
    return testChar(c[i++],i);
  }
}

int killChars(char *c[], int len){
  int j = 0;
  ret = len;
  testChar[c,j];
  return j;
}
*/


//entfernt alle Leerzeichen aus einem Array von chars
//TODO: alle Sonderzeichen
int killSpace(char *c, int len){
  int ret = len;
  int j = 0;
  for(int i = 0;
      i < len;
      i++)
  {
      /*
      if(c[j]==32){
        j++;
      }
      */
      if(!((c[j]>=48 && c[j]<=57) || ((c[j]>=65) && (c[j]<=90)))){
        j++;
      }

      //DEBUG
      printf("%d %c %d %c \n",i,c[i],j,c[j]);

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
  //leng = killChars(text,leng);
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
