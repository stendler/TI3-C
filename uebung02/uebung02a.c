//Tutor: Thomas
//Bearbeiter: Jasmine Cavael, Maximilian Stendler
/*Uebung 02
****Aufgabe 2a)
****Vigenere-Chiffre
*/
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
  while(j < len && i < ret)
  {
      if(!((c[j]>=48 && c[j]<=57) || ((c[j]>=65) && (c[j]<=90)))){
        ret--;
        //DEBUG
        //printf("%d %c %d %c len: %d\n",i,c[i],j,c[j],ret);
      }else{
        c[i] = c[j];
        i++;
        //DEBUG
        //printf("%d %c %d %c len: %d\n",i,c[i],j,c[j],ret);
      }
      j++;
  }
  while(i<ret){
    c[i++] = 32;
  }
  //DEBUG
  //printf("%d %c %d %c len: %d\n",i,c[i],j,c[j],ret);

  return ret;
}

//Normaliesieren:
//A-Z = 0-25 | 0-9 = 26-35
void norm(char *c, int len){
  //printf("Normaliesieren: \n");
  for(int i = 0;
      i<len;
      i++){
        //DEBUG
        //printf("%c %d -> ",c[i],c[i]);
        if(c[i]>=65 && c[i]<=90){
          c[i] -= 65;
        }else{
          c[i] -= 22;
        }
        //DEBUG
      //  printf("%c %d\n",c[i],c[i]);
      }
}

//Zuruecknormalisieren
//0-25 -> 65-90 | 26-35 -> 48-57
void deNorm(char *c, int len){
//  printf("DeNormalisieren: \n");
  for(int i = 0;
      i<len;
      i++){
        //DEBUG
      //  printf("%c %d-> ",c[i],c[i]);
        if(c[i] <= 25){
          c[i] += 65;
        }else{
          c[i] += 22;
        }
        //DEBUG
      //  printf("%c %d\n",c[i],c[i]);
      }
}

void encrypt(char *text,char *code, char *chiffre,int len){
  norm(text,len);
  norm(code,len);
  for(int i = 0;
      i<len;
      i++){
        chiffre[i] = (text[i]+code[i])%36;
      }
  deNorm(chiffre,len);
}

void decrypt(char *chiffre, char *code,char *text, int len){
  norm(chiffre,len);
  norm(code,len);
  for(int i = 0;
      i<len;
      i++){
        int help  = (chiffre[i]-code[i])%36;
        if(help < 0){
            help+=36;
        }
        text[i] = help;
        //printf("%d - %d mod 36 = %d\n",chiffre[i],code[i],text[i]);
      }
  deNorm(text,len);
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
    //printf(" %d ",text[leng]);
  }
  //--codewort
  int clen = 0; //Code length
  while(codewort[clen]!=0 && codewort[clen] != 127){
    clen++;
  }
  if(leng>100 || clen > 10){
    printf("ERROR: Eingaben zu lang.\n");
    return -11;
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
  clen = killChars(codewort,clen);

  //DEBUG
  //  printf("%s\n",text);
  //  printf("%s\n",codewort);
  //printf("%d %d\n",leng,clen);

  //Codewort auf Länge des Textes bringen
  char code[leng];
  for(int i = 0;i<leng;i++){
    int mod = i%clen;
    code[i] = codewort[mod];
    //DEBUG
    //printf("%d %d %c\n",i,mod,code[i]);
  }

  //DEBUG
  //  printf("%s\n",code);

  //String fuer das ver-/entschluesselte ergebnis
  char chiffre[leng];

  //ent- oder verschluesseln
  if(boolean == 0){
    encrypt(text,code,chiffre,leng);
  }else{
    decrypt(text,code,chiffre,leng);
  }

  //ergebnis ausgeben
  for(int i=0;i<leng;i++){
    printf("%c",chiffre[i]);
  }
  printf("\n");

  //printf("\n %s \n",chiffre); warum auch immer, aber %s ist unzuverlaessig: manchemal wird ein zeichen nicht ausgegebn und manchmal eins zu viel

  return 0;
}
