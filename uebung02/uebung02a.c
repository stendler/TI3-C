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
  while(i < len)
  {
      if(!((c[j]>=48 && c[j]<=57) || ((c[j]>=65) && (c[j]<=90)))){
        ret--;
      }else{
        c[i] = c[j];
        i++;
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
  printf("Normaliesieren: \n");
  for(int i = 0;
      i<len;
      i++){
        //DEBUG
        printf("%c %d -> ",c[i],c[i]);
        if(c[i]>=65 && c[i]<=90){
          c[i] -= 65;
        }else{
          c[i] -= 22;
        }
        //DEBUG
        printf("%c %d\n",c[i],c[i]);
      }
}

//Zuruecknormalisieren
//0-25 -> 65-90 | 26-35 -> 48-57
void deNorm(char *c, int len){
  printf("DeNormalisieren: \n");
  for(int i = 0;
      i<len;
      i++){
        //DEBUG
        printf("%c %d-> ",c[i],c[i]);
        if(c[i] <= 25){
          c[i] += 65;
        }else{
          c[i] += 22;
        }
        //DEBUG
        printf("%c %d\n",c[i],c[i]);
      }
}

//encrypt: Indizes von text & code addieren
char* encrypt(char *text,char *code,int len){
  char ret[len];
  norm(text,len);
  norm(code,len);
  for(int i = 0;
      i<len;
      i++){
        ret[i] = (text[i]+code[i])%36;
      }
  deNorm(ret,len);
  return ret;
}
//decrypt: Indizes von text & code subtrahieren
char* decrypt(char *chiffre, char *code, int len){
  char ret[len];
  norm(chiffre,len);
  norm(code,len);
  for(int i = 0;
      i<len;
      i++){
        ret[i] = (chiffre[i]-code[i])%36;
      }
  deNorm(ret,len);
  return ret;
}

int main(){
  char codewort[11];
  char text[101];
  int boolean = 0; //0 = encrypt; 1 = decrypt

  printf("Text (max 100): ");
  scanf("%[^\n]s",text);

  printf("Codewort (max. 10): ");
  scanf("%s",codewort);

  printf("Soll verschluesselt oder entschluesselt werden? [0 = encrypt|1 = decrypt]");
  scanf("%d",&boolean);

  //Laengen Messung
  //--text
  int leng = 0; //text length
  while(text[leng]!=0 && text[leng] != 127){
    leng++;
  }
  //--codewort
  int clen = 0; //Code length
  while(codewort[clen]!=0 && codewort[clen] != 127){
    clen++;
  }

  //printf("%d %d\n",leng,clen);
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

  if(boolean == 0){
    printf("%s\n",encrypt(text,code,leng));
  }else{
    printf("%s\n",decrypt(text,code,leng));
  }


  return 0;
}
