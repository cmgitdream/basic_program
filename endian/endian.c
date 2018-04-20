
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int is_little_endian()
{
  /*
  * human read:
  *  MSB --> LSB
  *  12345678943
  *  0x12345678
  *
  * big endian:
  * low address : MSB
  * high address: LSB
  *        32bits 
  * low || addrees 0: 0x12  MSB
  *     || address 1: 0x34 
  *     || address 2: 0x56
  * high\/ address 3: 0x78  LSB
  * 
  * little endian:
  * low address : LSB
  * high address: MSB
  *        32bits 
  * low || addrees 0: 0x78  LSB
  *     || address 1: 0x56 
  *     || address 2: 0x34
  * high\/ address 3: 0x12  MSB
  *
  */
  uint32_t n = 1;
  char low_address = 'a';
  low_address = (char)n; 

  printf("low_address: %d, n : %d\n", low_address, n);
  if (low_address == n) {
    printf("little endian\n");
    return 1;
  } else {
    printf("big endian\n");
    return 0;
  }
}

int main(int argc, char *argv[])
{
  is_little_endian();
  return 0;
}
