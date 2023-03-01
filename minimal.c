#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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

void copy_array(void)
{
   int len = 4;
   char **array = (char **)malloc(len * sizeof(char*));

   printf("Locally-malloced target array, "
          "local code to malloc-copy-assign source strings.\n");

   char **tptr = array;
   char **tend = tptr + len;

   const char **sptr = SArray;
   const char **send = sptr + Len_SArray;

   while (tptr < tend && sptr < send)
   {
      int slen = strlen(*sptr);
      char *buff = (char*)malloc(slen+1);
      if (buff)
      {
         memcpy(buff, *sptr, slen);
         buff[slen] = '\0';
         *tptr = buff;
      }

      ++tptr;
      ++sptr;
   }
}

int main(int argc, const char **argv)
{
   copy_array();
   return 0;
}
