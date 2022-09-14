#include "tfs.h"
#include <math.h>

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
  if(curr_block != 0) {
    while(curr_block != 1) {
      curr_block = file_allocation_table[last_block];
      file_allocation_table[last_block] = 0;
      last_block = curr_block;
    }
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
  if(!file_is_readable(directory[file_descriptor].name)) {
    // printf("THIS FILE IS NOT READABLE!!!!!!!\n");
    return FALSE;
  }
  if(!tfs_check_fd_in_range(file_descriptor)) {
    return FALSE;
  }
  if(!tfs_check_file_is_open(file_descriptor)) {
    return FALSE;
  }
  if(directory[file_descriptor].status == 0) {
    return FALSE;
  }
  

  int newOffset = directory[file_descriptor].byte_offset;
  // directory[file_descriptor].byte_offset = newOffset + *buffer;
  unsigned int bytes_moved = 0;
  unsigned int amount = byte_count;
  unsigned int sizeBlock = ceil(byte_count/BLOCK_SIZE);
/*****************************/
  unsigned int bytes_left = amount;
  unsigned int size = 0;
  unsigned int curr_block = directory[file_descriptor].first_block;
  while((newOffset > 127) && (curr_block != 1)) {
    newOffset -= 128;
    curr_block = file_allocation_table[curr_block];
  }
  if (newOffset >= 128) {
    return FALSE;
  }
  if (newOffset != 0) {
    if (bytes_left > (128 - newOffset)){
      strncat(buffer, blocks[curr_block].bytes + newOffset, 128 - newOffset);
      size += (128-newOffset);
      bytes_left -= (128-newOffset);
      printf("TEST1: %d\n", size);
    }
    else {
      strncat(buffer, blocks[curr_block].bytes + newOffset, bytes_left);
      size += bytes_left;
      bytes_left = 0;
      printf("TEST2: %d\n", size);

    }
  }
  curr_block = file_allocation_table[curr_block];
  // bytes_left -= size;
  while(bytes_left > 0) {
    if(curr_block == 1) {
      break;
    }
    if(bytes_left < 128) {
      strncat(buffer, blocks[curr_block].bytes, bytes_left);
      size += bytes_left;
      bytes_left = 0;
      printf("TEST3: %d\n", size);

    }
    else {
      strncat(buffer, blocks[curr_block].bytes, 128);
      size += bytes_left;
      bytes_left -= 128;
      printf("TEST4: %d\n", size);

    }
    curr_block = file_allocation_table[curr_block];
  }

  directory[file_descriptor].byte_offset += size;

/*******************************/

  // for(int i = 0; i <= sizeBlock; i++) {

  //   if(!tmp) {
  //     return byte_count - BLOCK_SIZE;
  //   }

  //   directory[file_descriptor].first_block = tmp;
  //   file_allocation_table[tmp] = tmp + 1;

  //   if(i == sizeBlock) {
  //     file_allocation_table[tmp] = LAST_BLOCK;
  //   }

  // }
      printf("TEST: %d\n", size);

  return size;
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
  if(!tfs_check_fd_in_range(file_descriptor)) {
    return 0;
  }
  if(!tfs_check_file_is_open(file_descriptor)) {
    return 0;
  }
  if(!file_is_writable(directory[file_descriptor].name)) {
    // printf("THIS FILE IS NOT WRITABLE!!!!!!!\n");
    return 0;
  }

  int newOffset = directory[file_descriptor].byte_offset;
  // directory[file_descriptor].byte_offset = newOffset + *buffer;

  int fileSize = directory[file_descriptor].size;
  unsigned int loop_count = fileSize / 128;
  unsigned int leftover = fileSize % 128;
  unsigned int bytes_added = 0;
  unsigned int bytes_left = byte_count;
  // directory[file_descriptor].size = fileSize + byte_count;

  int count = byte_count;
  int sizeBlock = ceil(byte_count/BLOCK_SIZE);
  int last_move = 0;
  unsigned int curr_block;

  // iterates through how many blocks we need
  // for(int i = 0; i <= sizeBlock; i++) {
  if (directory[file_descriptor].first_block != 0) {
    curr_block = directory[file_descriptor].first_block;

    for (int i = 0; i < loop_count; i++) {
      curr_block = file_allocation_table[curr_block];
      last_move = curr_block;
    }
    strncat(blocks[curr_block].bytes+leftover, buffer, BLOCK_SIZE-leftover);
    bytes_added += (BLOCK_SIZE-leftover);
    bytes_left = byte_count - bytes_added;
  }

  loop_count = (ceil)(bytes_left/BLOCK_SIZE);
    // if (directory[file_descriptor].first_block != 0) {
    //   curr_block = directory[file_descriptor].first_block;
    //   while(file_allocation_table[curr_block] != LAST_BLOCK) {
        // curr_block = file_allocation_table[curr_block];
    //     last_move = curr_block;
    //   }
    // }

  for(int i = 0; i < loop_count; i++) {
    unsigned int tmp = tfs_new_block();
    if(!tmp) {
      break;    
    }

    // Set as first block if none already
    if (directory[file_descriptor].first_block == 0) {
      directory[file_descriptor].first_block = tmp;
    }

    if(i == (loop_count-1)) {
      strncat(blocks[tmp].bytes, buffer, bytes_left);
      bytes_added += bytes_left;
    }
    else {
      strncat(blocks[tmp].bytes, buffer, BLOCK_SIZE);
      bytes_added += BLOCK_SIZE;
    }

    file_allocation_table[tmp] = LAST_BLOCK;
    if(last_move != 0) {
      file_allocation_table[last_move] = tmp;
    }

    bytes_left = byte_count - bytes_added;
    last_move = tmp;
  }
  
  directory[file_descriptor].byte_offset += bytes_added;
  directory[file_descriptor].size += bytes_added;

  return bytes_added;
}

bool file_is_readable(char *name) {

  unsigned int fd = tfs_map_name_to_fd(name);

  if(directory[fd].read_perm == 0) {
    printf("%s is not readable\n", name);
    return false;
  }

  printf("%s is readable\n", name);
  return true;
}
  

bool file_is_writable(char *name){

  unsigned int fd = tfs_map_name_to_fd(name);

  if(directory[fd].write_perm == 0) {
    printf("%s is not writable\n", name);
    return false;
  }

  printf("%s is writable\n", name);
  return true;
  
}


void file_make_readable(char *name) {
  
  unsigned int fd = tfs_map_name_to_fd(name);

  directory[fd].read_perm = 1;
 
  printf("Permissions of file changed successfully\n");
  
}

void file_make_writable(char *name) {
  
  unsigned int fd = tfs_map_name_to_fd(name);

  directory[fd].write_perm = 1;
  

  printf("Permissions of file changed successfully\n");
}



