/*
*Name: Jacob Alcumbrack and Drew Garrison
*Assignment: HW3
*Date: 7/29/2022
*/

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

  //Error checking
  if(!tfs_check_fd_in_range(file_descriptor)) return(FALSE);
  if(directory[file_descriptor].status != CLOSED) return(FALSE);

  //setting current block equal to the first block
  unsigned char curr_block = directory[file_descriptor].first_block;
  unsigned char last_block = curr_block;

  //setting blocks equal to zero
  if(curr_block != 0) {
    while(curr_block != 1) {
      curr_block = file_allocation_table[last_block];
      file_allocation_table[last_block] = 0;
      last_block = curr_block;
    }
  }
  
  //setting status to UNUSED after deleting
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

  //Error checking
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
  
  //setting byte offset
  int newOffset = directory[file_descriptor].byte_offset;
  // directory[file_descriptor].byte_offset = newOffset + *buffer;

  unsigned int bytes_moved = 0;
  unsigned int amount = byte_count;
  unsigned int sizeBlock = ceil(byte_count/BLOCK_SIZE);
  unsigned int bytes_left = amount;
  unsigned int size = 0;
  unsigned int curr_block = directory[file_descriptor].first_block;

  while((newOffset > 127) && (curr_block != 1)) {
    newOffset -= 128;
    curr_block = file_allocation_table[curr_block];
  }

  //checking offsets
  if (newOffset >= 128) {
    return FALSE;
  }
  //reading in buffer if offset is not equal to 0
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

  //Error Checking
  if(!tfs_check_fd_in_range(file_descriptor)) {
    return 0;
  }
  if(!tfs_check_file_is_open(file_descriptor)) {
    return 0;
  }

  int newOffset = directory[file_descriptor].byte_offset;
  directory[file_descriptor].byte_offset = newOffset + *buffer;

  int fileSize = directory[file_descriptor].size;
  directory[file_descriptor].size = fileSize + byte_count;

  int count = byte_count;
  int sizeBlock = ceil(byte_count/BLOCK_SIZE);
  int last_move = 0;
  unsigned int curr_block;
  // iterates through how many blocks we need
  for(int i = 0; i <= sizeBlock; i++) {
    if (directory[file_descriptor].first_block != 0) {
      curr_block = directory[file_descriptor].first_block;
      while(file_allocation_table[curr_block] != LAST_BLOCK) {

        if(LAST_BLOCK >= N_BLOCKS) {
          return 0;
        }

        curr_block = file_allocation_table[curr_block];
        last_move = curr_block;
        strncat(blocks[curr_block].bytes, buffer, BLOCK_SIZE);
        
      }
    }
    unsigned int tmp = tfs_new_block();
    if(!tmp) {
      return byte_count - BLOCK_SIZE;
    }
    // Set as first block if none already
    if (directory[file_descriptor].first_block == 0) {
      directory[file_descriptor].first_block = tmp;
    }

    file_allocation_table[tmp] = LAST_BLOCK;
    if(last_move != 0) {
      file_allocation_table[last_move] = tmp;
    }
    
    last_move = tmp;
  }

  return byte_count;
}
unsigned int tfs_write( unsigned int file_descriptor,
                        char *buffer,
                        unsigned int byte_count ){

  /* your code here */

  //Error Checking
  if(!tfs_check_fd_in_range(file_descriptor)) {
    return 0;
  }
  if(!tfs_check_file_is_open(file_descriptor)) {
    return 0;
  }
  // if(directory[file_descriptor].first_block == 0) {
  //   directory[file_descriptor].write_perm = 1;
  // }
  if(!file_is_writable(directory[file_descriptor].name)) {
    // printf("THIS FILE IS NOT WRITABLE!!!!!!!\n");
    return 0;
  }


  //finding new byte offset based off the file desriptor
  int newOffset = directory[file_descriptor].byte_offset;
  directory[file_descriptor].byte_offset = newOffset + *buffer;

  //finding the size of the file based off the file descriptor
  int fileSize = directory[file_descriptor].size;
  directory[file_descriptor].size = fileSize + byte_count;

  int count = byte_count;
  int sizeBlock = ceil(byte_count/BLOCK_SIZE);
  int last_move = 0;
  unsigned int curr_block;

  //finding how many blocks are needed
  for(int i = 0; i <= sizeBlock; i++) {
  
    if (directory[file_descriptor].first_block != 0) {
      curr_block = directory[file_descriptor].first_block;
      while(file_allocation_table[curr_block] != LAST_BLOCK) {
        curr_block = file_allocation_table[curr_block];
        last_move = curr_block;
      }
    }
    //creating new block
    unsigned int tmp = tfs_new_block();
    if(!tmp) {
      return byte_count - BLOCK_SIZE;
    }
    // Set as first block if none already
    if (directory[file_descriptor].first_block == 0) {
      directory[file_descriptor].first_block = tmp;
    }
    
    //setting new block to the last block in the FAT
    file_allocation_table[tmp] = LAST_BLOCK;
    if(last_move != 0) {
      file_allocation_table[last_move] = tmp;
    }
    
    last_move = tmp;
  }

  return byte_count;
}

bool file_is_readable(char *name) {

  //finding file descriptor associated with file name
  unsigned int fd = tfs_map_name_to_fd(name);

  //checking if file has read permissions
  if(directory[fd].read_perm == 0) {
    printf("%s is not readable\n", name);
    return false;
  }

  printf("%s is readable\n", name);
  return true;
}
  

bool file_is_writable(char *name){

  //finding file descriptor associated with file name 
  unsigned int fd = tfs_map_name_to_fd(name);

  //checking if the file has write permissions
  if(directory[fd].write_perm == 0) {
    printf("%s is not writable\n", name);
    return false;
  }

  printf("%s is writable\n", name);
  return true;
  
}


void file_make_readable(char *name) {
  
  //finding file descriptor associated with the file name
  unsigned int fd = tfs_map_name_to_fd(name);

  //setting read permissions
  directory[fd].read_perm = 1;
 
  printf("Permissions of file changed successfully\n");
  
}

void file_make_writable(char *name) {
  
  //finding file descriptor associated with the file name
  unsigned int fd = tfs_map_name_to_fd(name);

  //setting write permissions
  directory[fd].write_perm = 1;
  
  printf("Permissions of file changed successfully\n");
}



