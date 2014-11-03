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
int killChars(char *c, int len){
  int ret = len;
  int j = 0;
  int i = 0;
  for(i;
      i < len;
      i++)
  {
      if(!((c[j]>=48 && c[j]<=57) || ((c[j]>=65) && (c[j]<=90)))){
        i--;
        ret--;
      }else{
        c[i] = c[j];
        //DEBUG
        //printf("%d %c %d %c len: %d\n",i,c[i],j,c[j],ret);
      }
      j++;
      if(j > len){
        c[i] = 0;
      }
      if(j >= len){
        //DEBUG
        //printf("BREAK: j >= %d, ret=%d\n",len,ret);
        break;
      }
  }
  //DEBUG
  //printf("i=%d \n",i);
  if(i<len){
    c[i+1] = 0;
  }
  return ret;
}

//Normaliesieren:
//A-Z = 0-25 | 0-9 = 26-35
void norm(char *c, int len){
  for(int i = 0;
      i<len;
      i++){
        if(c[i]>=65 && c[i]<=90){
          c[i] -= 65;
        }else{
          c[i] -= 12;
        }
      }
}

//Zuruecknormalisieren
//0-25 -> 65-90 | 26-35 -> 48-57
void normB(char *c, int len){
  for(int i = 0;
      i<len;
      i++){
        if(c[i] <= 25){
          c[i] += 65;
        }else{
          c[i] += 12;
        }
      }
}

//encrypt: Indizes von text & code addieren
//decrypt: Indizes von text & code subtrahieren

int main(){
  char codewort[11];
  char text[101];

  printf("Text (max 100): ");
  scanf("%[^\n]s",text);

  printf("Codewort (max. 10): ");
  scanf("%s",codewort);

  //Laenge der reinen Texteingabe -- gibt nur die Gesamtlaenge des Arrays zurück
  //int leng = sizeof text / sizeof *text;
  //int clen = sizeof codewort / sizeof *codewort;
  //printf("%d %d\n",leng,clen);

  //Laengen Messung
  //text
  leng = 0; //text length
  while(text[leng]!=0 && text[leng] != 127){
    leng++;
  }
  //codewort
  clen = 0; //Code length
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

  //Laut Aufgabenstellung keine Sonderzeichen
  leng = killChars(text,leng);
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
