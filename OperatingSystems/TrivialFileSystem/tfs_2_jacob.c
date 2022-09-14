#include "tfs.h"


/* implementation of assigned functions */


/* you are welcome to use helper functions of your own */



/* tfs_delete()
 *
 * deletes a closed directory entry having the given file descriptor
 *   (changes the status of the entry to unused) and releases all
 *   allocated file blocks
 *
 * preconditions:
 *   (1) the file descriptor is in range
 *   (2) the directory entry is closed
 *
 * postconditions:
 *   (1) the status of the directory entry is set to unused
 *   (2) all file blocks have been set to free
 *
 * input parameter is a file descriptor
 *
 * return value is TRUE when successful or FALSE when failure
 */

unsigned int tfs_delete( unsigned int file_descriptor ){

  /* your code here */
  if(!tfs_check_fd_in_range(file_descriptor)) return(FALSE);
  if(directory[file_descriptor].status != CLOSED) return(FALSE);

  unsigned char curr_block = directory[file_descriptor].first_block;
  unsigned char last_block = curr_block;
  while(curr_block != 1) {
    curr_block = file_allocation_table[last_block];
    file_allocation_table[last_block] = 0;
    last_block = curr_block;
  }

  directory[file_descriptor].status = UNUSED;
  directory[file_descriptor].first_block = 0;

  return TRUE;
}


/* tfs_read()
 *
 * reads a specified number of bytes from a file starting
 *   at the byte offset in the directory into the specified
 *   buffer; the byte offset in the directory entry is
 *   incremented by the number of bytes transferred
 *
 * depending on the starting byte offset and the specified
 *   number of bytes to transfer, the transfer may cross two
 *   or more file blocks
 *
 * the function will read fewer bytes than specified if the
 *   end of the file is encountered before the specified number
 *   of bytes have been transferred
 *
 * preconditions:
 *   (1) the file descriptor is in range
 *   (2) the directory entry is open
 *   (3) the file has allocated file blocks
 *
 * postconditions:
 *   (1) the buffer contains bytes transferred from file blocks
 *   (2) the specified number of bytes has been transferred
 *         except in the case that end of file was encountered
 *         before the transfer was complete
 *
 * input parameters are a file descriptor, the address of a
 *   buffer of bytes to transfer, and the count of bytes to
 *   transfer
 *
 * return value is the number of bytes transferred
 */

unsigned int tfs_read( unsigned int file_descriptor,
                       char *buffer,
                       unsigned int byte_count ){

  /* your code here */

}

/* tfs_write()
 *
 * writes a specified number of bytes from a specified buffer
 *   into a file starting at the byte offset in the directory;
 *   the byte offset in the directory entry is incremented by
 *   the number of bytes transferred
 *
 * depending on the starting byte offset and the specified
 *   number of bytes to transfer, the transfer may cross two
 *   or more file blocks
 *
 * furthermore, depending on the starting byte offset and the
 *   specified number of bytes to transfer, additional file
 *   blocks, if available, will be added to the file as needed;
 *   in this case, the size of the file will be adjusted
 *   based on the number of bytes transferred beyond the
 *   original size of the file
 *
 * the function will read fewer bytes than specified if file
 *   blocks are not available
 *
 * preconditions:
 *   (1) the file descriptor is in range
 *   (2) the directory entry is open
 *
 * postconditions:
 *   (1) the file contains bytes transferred from the buffer
 *   (2) the specified number of bytes has been transferred
 *         except in the case that file blocks needed to
 *         complete the transfer were not available
 *   (3) the size of the file is increased as appropriate
 *         when transferred bytes extend beyond the previous
 *         end of the file
 *
 * input parameters are a file descriptor, the address of a
 *   buffer of bytes to transfer, and the count of bytes to
 *   transfer
 *
 * return value is the number of bytes transferred
 */

unsigned int tfs_write( unsigned int file_descriptor,
                        char *buffer,
                        unsigned int byte_count ){

  /* your code here */
  if(!tfs_check_fd_in_range(file_descriptor)) return(FALSE);
  if(!tfs_check_file_is_open(file_descriptor)) return(FALSE);
  unsigned int bytes_left = byte_count;
  unsigned int blocks_filled = 0;
  unsigned int curr_block = FIRST_VALID_BLOCK;
  unsigned int last_move = 0;
  if (directory[file_descriptor].first_block == 0) {
    while(bytes_left > 0) {
      if(file_allocation_table[curr_block] == FREE) {
        if(directory[file_descriptor].first_block == 0) {
          directory[file_descriptor].first_block = curr_block;
        }
        file_allocation_table[curr_block] = LAST_BLOCK;
        if (last_move != 0) {
          file_allocation_table[last_move] = curr_block;
        }
        last_move = curr_block;
        bytes_left -= 128;
      }
      curr_block++;
    }
  }
  else{
    curr_block = directory[file_descriptor].first_block;
    while(file_allocation_table[curr_block] != LAST_BLOCK) {
      curr_block = file_allocation_table[curr_block];
    }
    last_move = curr_block;
    curr_block++;
    while(bytes_left > 0) {
      if(file_allocation_table[curr_block] == FREE) {
        file_allocation_table[curr_block] = LAST_BLOCK;
        file_allocation_table[last_move] = curr_block;
        last_move = curr_block;
        bytes_left -= 128;
      }
      curr_block++;
    }
  }

  bytes_left = byte_count;
  curr_block = directory[file_descriptor].first_block;
  while(bytes_left > 0) {
    strncat(&blocks[curr_block], buffer, 128);
    curr_block = file_allocation_table[curr_block];
    blocks_filled++;
  }

  if(blocks_filled*128 >= byte_count) {
    directory[file_descriptor].size += byte_count;
    return (byte_count);
  }
  else {
    directory[file_descriptor].size += (blocks_filled*128);
  }
  return (blocks_filled*128);
}
