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
   const char **tend = tptr + target_len;

   const char **sptr = source;
   const char **send = sptr + source_len;

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

int return_copied_array_local_populate(const char ***array, int *len)
{
   const char **arr = (const char**)malloc(Len_SArray * sizeof(char*));
   if (arr)
   {
      null_init_strings_array(arr, Len_SArray);

      const char **tptr = arr;
      const char **tend = tptr + Len_SArray;;

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

      *array = arr;
      *len = Len_SArray;

      return 1;
   }

   return 0;
}

int return_copied_array_called_populate(const char ***array, int *len)
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
   const char **tend = tptr + len;

   const char **sptr = SArray;
   const char **send = sptr + Len_SArray;

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

/**
 * @brief Call function that returns populated char** to char*** argument.
 *
 * This function this function calls does all of the mallocs in one
 * stack frame to copy the one unique characteristic of @ref assign_method_one.
 *
 * Sadly, depending on your objectives, this function results in
 * memory leak warnings.  Not a match to @ref assign_method_one.e
 */
void assign_method_five(void)
{
   const char **array;
   int len;

   printf("Calling function-malloced target array, "
          "assigning malloc-ed values to elements in same stack frame.\n");

   if (return_copied_array_local_populate(&array, &len))
   {
      const char **ptr = array;
      const char **end = ptr + len;

      while (ptr < end)
      {
         printf("%s\n", *ptr);
         ++ptr;
      }
   }
}

/**
 * @brief For completion, sets array values to memory malloc-ed in a called function.
 */
void assign_method_six(void)
{
   const char **array;
   int len;

   printf("Calling function-malloced target array, "
          "assigning malloc-ed values to elements in called stack frame.\n");

   if (return_copied_array_called_populate(&array, &len))
   {
      const char **ptr = array;
      const char **end = ptr + len;

      while (ptr < end)
      {
         printf("%s\n", *ptr);
         ++ptr;
      }
   }
}



int main(int argc, const char **argv)
{
   const char **ptr = argv;
   const char **end = ptr + argc;

   int len_array = 0;
   const char **array = NULL;

   const char methods[] = "123456";
   int flag_count = sizeof(methods) + 1; // Include unused 0th position to make space
                                         // all the numbers in the methods[] string.
   char *flags = (char*)malloc(flag_count);
   memset(flags, 0, flag_count);

   while (ptr < end)
   {
      char *selection = strchr(methods, (int)**ptr);
      if (selection)
      {
         int ndx = *selection - '0';
         if (flags[ndx] == 0)
         {
            printf("About to run method #%d.\n", ndx);
            switch(ndx)
            {
               case 1:
                  assign_method_one();
                  break;
               case 2:
                  assign_method_two();
                  break;
               case 3:
                  len_array = 8;
                  array = (const char **)malloc( len_array * sizeof(const char*));
                  assign_method_three(array, len_array);
                  array = NULL;
                  break;
               case 4:
                  len_array = 6;
                  array = (const char **)malloc( len_array * sizeof(const char*));
                  assign_method_four(array, len_array);
                  array = NULL;
                  break;
               case 5:
                  assign_method_five();
                  break;
               case 6:
                  assign_method_six();
                  break;
            }
         }
         else
            printf("Will not rerun method #%d.\n", ndx);

         // Prevent second run with same method
         flags[ndx] = 1;
      }
      ++ptr;
   }

   // Avoid non-test-generated memory leaks
   // free(flags);

   return 0;
}

