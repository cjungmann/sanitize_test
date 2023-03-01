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

/**
 * @brief Set elements of an array of char* to NULL.
 *
 * @param "array"   array whose elements should be NULLed
 * @param "len"     number of elements to be NULLed
 */
void null_init_strings_array(const char **array, int len)
{
   int asize = len * sizeof(char*);
   memset(array, 0, asize);
}

/**
 * @brief Frees and sets to NULL each member of an array.
 *
 * Be careful not to use the function on a uninitialized array.
 *
 * @param "array"    array of pointers to char* whose elements must be freed
 * @param "len"      number of elements to free
 */
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
 * @brief Given an integer value, returns a character from the set A-Za-z0-9.
 *
 * @param "rnum"   integer value to convert to a char
 *
 * @return Associated char value from A-Za-z0-9
 */
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

/**
 * @brief Generates a random string of random length (5-25 characters)
 *
 * This function is an unlimited source of random text strings that
 * can be used in place of the array of strings defines at the top of
 * this source file.
 *
 * @param[out] "line"    Pointer to char* in which the generated text
 *                       will be indicated.
 * @param[out] "len"     Number of characters in the generated text.
 */
void get_random_text_string(const char **line, int *len)
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

/**
 * @brief Fill an array with randomly-generated strings.
 */
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
      get_random_text_string(&line, &line_len);
      char *buff = (char*)malloc(line_len+1);
      memcpy(buff, line, line_len);
      buff[line_len] = '\0';
      *ptr = buff;
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

