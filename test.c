#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>

typedef struct line_reader {
   int fd;
   int buff_len;
   char *buffer;
   const char *end_buffer;
   const char *ptr_data;
   const char *end_data;
} LR;


int line_reader_valid_data_length(const LR *reader);
int line_reader_fill_empty_space(LR *reader);

/**
 * @brief Prepare line reader with buffer memory and opened file.
 *
 * @param "reader"     Pointer to uninitialized line_reader object
 * @param "buff_len"   Byte-length request for read buffer
 * @param "filename"   Name of file from which lines are to be read.
 *                     If @p filename is NULL, use STDIN_FILENO.
 *
 * @return 1 for success, 0 for failure.  errno is set for failure.
 */
int line_reader_init(LR *reader, int buff_len, const char *filename)
{
   errno = 0;
   memset(reader, 0, sizeof(LR));

   int fd = STDIN_FILENO;

   if (filename)
   {
      fd = open(filename, O_RDONLY);
      if (fd == -1)
         goto early_exit;
   }

   char *buff = (char*)malloc(buff_len);
   if (!buff)
   {
      close(fd);
      goto early_exit;
   }

   reader->fd = fd;

   reader->buffer = buff;
   reader->buff_len = buff_len;
   reader->end_buffer = (const char *)buff + buff_len;
   reader->end_data = reader->ptr_data = (const char *)buff;
   line_reader_fill_empty_space(reader);

  early_exit:
   return errno == 0;
}

/**
 * @brief Increase buffer size and associated member pointers
 *
 * @param "reader"        Pointer to line_reader struct
 * @param "new_buff_len"  Requested new buffer length
 *
 * @return 1 for success, 0 for failure.  Consult errno for failure.
 */
int line_reader_grow(LR *reader, int new_buff_len)
{
   if (new_buff_len > reader->buff_len)
   {
      char *newbuffer = (char*)malloc(new_buff_len);
      if (newbuffer)
      {
         int bytes_to_copy = line_reader_valid_data_length(reader);
         memcpy(newbuffer, reader->ptr_data, bytes_to_copy);

         free(reader->buffer);

         reader->buff_len = new_buff_len;
         reader->ptr_data = reader->buffer = newbuffer;
         reader->end_buffer = newbuffer + new_buff_len;
         reader->end_data = newbuffer + bytes_to_copy;

         return 1;
      }
   }
   else
      errno = EINVAL;

   return 0;
}

/**
 * @brief Clean up line_reader object after use.
 *
 * @param "reader"   pointer to the line_reader struct.
 */
void line_reader_close(LR *reader)
{
   if (reader->fd > STDIN_FILENO)
      close(reader->fd);

   if (reader->buffer)
      free(reader->buffer);

   memset(reader, 0, sizeof(LR));
}

/**
 * @brief Returns length of remaining valid data in buffer
 */
int line_reader_buffer_data_left(const LR *reader)
{
   return reader->end_data - reader->ptr_data;
}

/**
 * @brief Returns length of free buffer space that can be filled
 */
int line_reader_buffer_left(const LR *reader)
{
   return reader->end_buffer - reader->end_data;
}

int line_reader_valid_data_length(const LR *reader)
{
   return reader->end_data - reader->ptr_data;
}

/**
 * @brief Reads from source to fill empty buffer space
 *
 * @param "reader"   pointer to line_reader struct
 *
 * @return 1 for success, 0 for failure.  errno set for failure
 */
int line_reader_fill_empty_space(LR *reader)
{
   errno = 0;
   // Fill empty space
   int bytes_read;
   int bytes_count = line_reader_buffer_left(reader);
   if (bytes_count > 0)
   {
      char *target = (char*)reader->end_data;
      bytes_read = read(reader->fd, target, bytes_count);
      if (bytes_read == 0)
         return 0;
      else if (bytes_read > 0)
         reader->end_data += bytes_read;
   }
   else
      errno = ENOBUFS;

   return errno == 0;
}

/**
 * @brief Shift buffer data and fill invalid area with new data from source.
 *
 * @param "reader"   pointer to line_reader struct
 *
 * @return Number of bytes shifted
 */
int line_reader_shift_data(LR *reader)
{
   int bytes_count = line_reader_buffer_data_left(reader);
   if (bytes_count >= reader->buff_len)
      return 0;
   else if (bytes_count == 0)
      reader->ptr_data = reader->end_data = reader->buffer;
   else
   {
      memmove(reader->buffer, reader->ptr_data, bytes_count);
      reader->ptr_data = reader->buffer;
      reader->end_data = reader->buffer + bytes_count;
   }

   return bytes_count;
}

/**
 * @brief Get next available line from source
 *
 * @param[out] "line"   Pointer-to-char-pointer from which the line can be read
 * @param[out] "count"  Number of characters in the retrieved line
 * @param[out] "erret"  Possible error number if function failed
 * @param[in]  "reader" Pointer to initialized line_reader struct
 * 
 * @return 1 if more lines to read, 0 if reached end-of-file
 */
int line_reader_read_line(const char **line, int *count, int *erret, LR *reader)
{
   *erret = 0;
   *count = 0;
   *line = NULL;

   int buffer_shifted = -1;

   const char *ptr = reader->ptr_data;

   while (1)
   {
      // Scan for newline and return if founc
      while (ptr < reader->end_data)
      {
         if (*ptr == '\n')
         {
            *count = ptr - reader->ptr_data;
            *line = reader->ptr_data;

            // Move data-pointer to next line
            reader->ptr_data = ptr + 1;

            return 1;
         }

         ++ptr;
      }

      // If no newline in entire buffer, indicate that the buffer is too small:
      if (ptr == reader->end_buffer && reader->ptr_data == reader->buffer)
      {
         // Attempt to grow the buffer
         if (!line_reader_grow(reader, reader->buff_len * 2))
         {
            *erret = errno;
            return 0;
         }
         // *erret = ENOBUFS;
         // return 0;
      }

      // Didn't find a newline, get more data
      buffer_shifted = line_reader_shift_data(reader);
      if (line_reader_fill_empty_space(reader))
      {
         // Continue scanning characters where we left off:
         ptr = reader->ptr_data + buffer_shifted;
      }
      else
      {
         if (reader->end_data > reader->ptr_data)
         {
            *line = reader->ptr_data;
            reader->ptr_data += buffer_shifted;
            return 1;
         }
         else
            // No source data and no buffer data: we're done
            return 0;
      }
   }
}


void run_test(int buffer_length, const char *filename)
{
   LR lr;
   if (line_reader_init(&lr, buffer_length, filename))
   {
      const char *line;
      int ccount;
      int erret;

      while (line_reader_read_line(&line, &ccount, &erret, &lr))
         printf("%2d: '%.*s'\n", ccount, ccount, line);

      if (erret)
         printf("Line reader of buffer length %d, terminated due to '%s' (%d).\n",
                lr.buff_len,strerror(erret), erret);
 
      line_reader_close(&lr);
   }
}

void copy_lines_to_array(char **strings, int strings_count, int buffer_length, const char *filename)
{
   LR lr;
   if (line_reader_init(&lr, buffer_length, filename))
   {
      const char *line;
      int ccount;
      int erret;

      char **ptr = strings;
      char **end = strings + strings_count;

      while (ptr < end && line_reader_read_line(&line, &ccount, &erret, &lr))
      {
         char *temp = (char*)malloc(ccount+1);
         memcpy(temp, line, ccount);
         temp[ccount] = '\0';
         *ptr = temp;

         ++ptr;
      }

      printf("Line reader of buffer length %d, terminated due to '%s'.\n",
             lr.buff_len,strerror(erret));

      line_reader_close(&lr);
   }
}

void run_test_array(int buffer_length, int array_length, const char *filename)
{
   int memlen = array_length * sizeof(char*);
   char **sarray = (char**)malloc(memlen);
   memset(sarray, 0, memlen);
   copy_lines_to_array(sarray, array_length, buffer_length, filename);

   // Show what we've saved:
   char **end = sarray + array_length;
   char **ptr = sarray;
   int linenumber=0;
   while (ptr < end)
   {
      if (*ptr)
         printf("%2d: %s\n", ++linenumber, *ptr);
      else
         break;

      ++ptr;
   }
}

int main(int argc, const char **argv)
{
   const char *filename = NULL;
   if (argc > 1)
      filename = argv[1];

   // run_test(41, filename);
   run_test_array(100, 500, filename);
   return 0;
}
