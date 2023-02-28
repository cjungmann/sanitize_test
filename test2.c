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

void null_init_strings_array(const char **array, int len)
{
   int asize = len * sizeof(char*);
   memset(array, 0, asize);
}

void free_array(char **array, int len)
{
   char **ptr = array;
   char **end = ptr + len;
   while (ptr < end)
   {
      free(*ptr);
      *ptr = NULL;

      ++ptr;
   }
}

/**
 * @brief Mallocs and assigns to target array strings found in source strings array.
 *
 * @param "target"      Array of pointers to string to be filled
 * @param "target_len"  Number of elements in @p target array
 * @param "source"      Array of pointer to string from which copies will be made
 * @param "source_len"  Number of elements in @p source array
 */
void copy_arrays(const char **target, int target_len, const char **source, int source_len)
{
   const char **tptr = target;
   const char **tend = target + target_len;

   const char **sptr = source;
   const char **send = source + source_len;

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

int return_copied_array(const char ***array, int *len)
{
   const char **arr = (const char**)malloc(Len_SArray * sizeof(char*));
   if (arr)
   {
      null_init_strings_array(arr, Len_SArray);
      copy_arrays(arr, Len_SArray, SArray, Len_SArray);

      *array = arr;
      *len = Len_SArray;

      return 1;
   }

   return 0;
}

/**
 * @brief Malloc for target array then malloc for copies of strings.
 */
void assign_method_one(void)
{
   int len = 4;
   char **array = (char **)malloc(len * sizeof(char*));

   printf("Locally-malloced target array, "
          "local code to malloc-copy-assign source strings.\n");

   char **tptr = array;
   char **tend = array + len;

   const char **sptr = SArray;
   const char **send = SArray + Len_SArray;

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

   free_array(array, len);
   free(array);
}

/**
 * @brief Malloc for target array then call function to malloc for copies of strings.
 */
void assign_method_two(void)
{
   int len = 6;
   const char **array = (const char**)malloc(len * sizeof(char*));

   printf("Locally-malloced target array, "
          "function call to malloc-copy-assign source string.\n");
   if (array)
      copy_arrays(array, len, SArray, Len_SArray);
}

/**
 * @brief Malloc string copies for each element of array malloced elsewhere.
 *
 * @param "array"   Array of pointers to char* to in which newly-malloced
 *                  strings will be saved
 * @param "len"     Number of elements in the @p array array
 */
void assign_method_three(const char **array, int len)
{
   const char **tptr = array;
   const char **tend = array + len;

   const char **sptr = SArray;
   const char **send = SArray + Len_SArray;

   printf("Calling function-malloced target array, "
          "local code to malloc-copy-assign source strings.\n");

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

/**
 * @brief Malloc string copies for each element of array malloced elsewhere,
 *        calling out to do the malloc-copy-save.
 *
 * @param "array"   Array of pointers to char* to in which newly-malloced
 *                  strings will be saved
 * @param "len"     Number of elements in the @p array array
 */
void assign_method_four(const char **array, int len)
{
   printf("Calling function-malloced target array, "
          "function call to malloc-copy-assign source string.\n");

   copy_arrays(array, len, SArray, Len_SArray);
}

void assign_method_five(void)
{
   const char **array;
   int len;
   if (return_copied_array(&array, &len))
   {
      const char **ptr = array;
      const char **end = array + len;

      while (ptr < end)
      {
         printf("%s\n", *ptr);
         ++ptr;
      }
   }
}




int main(int argc, const char **argv)
{
   assign_method_one();

   // assign_method_two();

   // int len_array = 8;
   // const char **array = (const char **)malloc( len_array * sizeof(const char*));
   // assign_method_three(array, len_array);

   // int len_array2 = 10;
   // const char **array2 = (const char **)malloc( len_array2 * sizeof(const char*));
   // assign_method_four(array2, len_array2);

   // assign_method_five();

   return 0;
}
