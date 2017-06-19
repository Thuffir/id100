/***********************************************************************************************************************
 *
 * ID100 Utility
 * File Operations
 *
 * (C) 2017 Gergely Budai
 *
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * For more information, please refer to <http://unlicense.org/>
 *
 **********************************************************************************************************************/
#include "file.h"
#include "utils.h"

/***********************************************************************************************************************
 * Open file
 **********************************************************************************************************************/
FILE *FileOpen(char *filename, bool openWrite)
{
  // File
  FILE *file;

  // Open file
  if(filename != NULL) {
    if((file = fopen(filename, openWrite ? "wb" : "rb")) == NULL) {
      ExitWithError("Unable to open file: %s", filename);
    }
  }
  else {
    file = openWrite ? stdout : stdin;
  }

  return file;
}

/***********************************************************************************************************************
 * Close file
 **********************************************************************************************************************/
void FileClose(FILE *file)
{
  // Close File
  if((file != stdout) && (file != stdin)) {
    if(fclose(file) != 0) {
      ExitWithError("Unable to close file");
    }
  }
}

/***********************************************************************************************************************
 * Write file
 **********************************************************************************************************************/
void FileWrite(FILE *file, void *buffer, size_t length)
{
  if(fwrite(buffer, length, 1, file) != 1) {
    ExitWithError("Unable to write %u bytes", length);
  }
}

/***********************************************************************************************************************
 * Read file
 **********************************************************************************************************************/
void FileRead(FILE *file, void *buffer, size_t length)
{
  if(fread(buffer, length, 1, file) != 1) {
    ExitWithError("Unable to read %u bytes", length);
  }
}
