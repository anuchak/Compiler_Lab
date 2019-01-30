#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

int isKeyword(char buffer[])
{
   char keywords[][15] = {"printf","scanf","gets","puts","fgets","fputs","break","case","char","const","continue","default","do","double","else","enum","float","for","goto","if","int","long","return","short","signed","sizeof","static","struct","switch","typedef","union","unsigned","void","volatile","while"};
   int i, flag = 0;
   for(i = 0; i < 36; ++i)
   {
     if(strcmp(keywords[i], buffer) == 0)
     {
       flag = 1;
       break;
      }
    }
   return flag;
}

int isNum(char buffer[])
{
   int i;
   for(i=0; buffer[i]!='\0'; i++)
   {
     if(!isdigit(buffer[i]))
       return 0;
   }
   return 1;
}

int main()
{
  char ch, buffer[15], operators[] = "+-*/%=><";
  FILE *fp;
  int i,j=0;
  int line=1,col=0;
  fp = fopen("program.txt","r");
  printf("\n");
  while((ch = fgetc(fp)) != EOF)
  {
    if(isalnum(ch))
    {
       buffer[j++] = ch;
       col++;
    }
    else if((ch == ' ' || ch == '\n' || ch == ',' || ch == '{' || ch == '(' || ch == ';' || ch == ')'|| ch == '}' || ch == '+' || ch == '-' || ch == '=' || ch == '*' || ch == '>' || ch == '<'))
    {
       if(j != 0)
       buffer[j] = '\0';

       if(ch == '\n')
       {
          line++;
          col=0;
       }
       else
          col++;
       int t=col;
       if(j != 0) {
       if(isKeyword(buffer))
         printf("%7s       keyword      Line %5d  Col %5d\n", buffer, line, t-1);
       else if(isNum(buffer))
         printf("%7s       constant     Line %5d  Col %5d\n", buffer, line, t-1); 
       else
         printf("%7s       identifier   Line %5d  Col %5d\n", buffer, line, t-1);
         j=0;
       }

       if(ch == ',' || ch == '(' || ch == '{' || ch == ';' || ch ==')' || ch == '}')
         printf("%7c       punctuation  Line %5d  Col %5d\n", ch, line, t);
    }
    for(i = 0; i < 8; i++)
    {
       if(ch == operators[i])
       {
         printf("%7c       operator     Line %5d  Col %5d\n", ch, line, col);
       }
    }
  }
  printf("\n");
  fclose(fp);
  return 0;
}
