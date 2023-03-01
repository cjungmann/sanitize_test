#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

const char* SArray[] = {
   "First String",
   "Second String",
   "Third String",
   "Fourth String",
   "Fifth String",
   "Sixth String",
   "Seventh String",
   "Eighth String",
   "Ninth String",
   "Tenth String"
};

int Len_SArray = sizeof(SArray) / sizeof(char*);


char get_numbered_char(long rnum)
{
   rnum %= 62;
   if (rnum < 26)
      return 65 + rnum;        // upper-case A-Z
   else if (rnum < 52)
      return 97 + rnum - 26;   // lower-case a-z
   else
      return 48 + rnum - 52;   // 0-9
}

void test_numbered_char(int value)
{
   printf("Character returned for %d is %c.\n", value, get_numbered_char(value));
}

void get_random_line(const char **line, int *len)
{
   // Random length between 5 and 25 characters:
   int loclen = (rand() % 20) + 5;
   // Buffer big enough to contain the longest generated string
   static char buffer[26];

   char *ptr = buffer;
   char *end = ptr + loclen;

   for ( ; ptr < end; ++ptr)
      *ptr = get_numbered_char(rand());
   *ptr = '\0';

   *line = buffer;
   *len = loclen;
}

void copy_array(void)
{
   int len = 8;
   char **array = (char **)malloc(len * sizeof(char*));

   char **tptr = array;
   char **tend = tptr + len;

   const char **sptr = SArray;
   const char **send = sptr + Len_SArray;

   for (; tptr < tend && sptr < send; ++tptr, ++sptr)
   {
      int slen = strlen(*sptr);
      char *buff = (char*)malloc(slen+1);
      if (buff)
      {
         memcpy(buff, *sptr, slen);
         buff[slen] = '\0';
         *tptr = buff;
      }
   }
}

void fill_array(void)
{
   int len = 12;
   char **array = (char**)malloc(len * sizeof(char*));
   char **ptr = array;
   char **end = ptr + len;

   const char *line;
   int line_len;

   for (; ptr < end; ++ptr)
   {
      get_random_line(&line, &line_len);
      char *buff = (char*)malloc(line_len+1);
      memcpy(buff, line, line_len);
      buff[line_len] = '\0';
      *ptr = buff;
   }
}

int main(int argc, const char **argv)
{
   // srand(time(NULL));
   // copy_array();
   fill_array();

   return 0;
}
