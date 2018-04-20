
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

unsigned ceph_str_hash_linux(const char *str, unsigned length)
{
	unsigned hash = 0;

	while (length--) {
		unsigned char c = *str++;
		hash = (hash + (c << 4) + (c >> 4)) * 11;
	}
	return hash;
}

uint32_t ceph_frag_make(uint32_t b, uint32_t v)
{
  printf("b : %u\n", b);
  printf("v : %u\n", v);
  printf("24 - b : %u\n", 24 - b);
  printf("0xffffffu << (24 - b) : %u\n", 0xffffffu << (24 - b));
  printf("(v & (0xffffffu << (24 - b)): %u\n",
	(v & (0xffffffu << (24 - b))));
  printf("(v & (0xffffffu << (24 - b)) & 0xffffffu: %u\n",
	(v & (0xffffffu << (24 - b)) & 0xffffffu));
  return (b << 24) | 
	(v & (0xffffffu << (24 - b)) & 0xffffffu);
}

uint32_t ceph_frag_bits(uint32_t f)
{
  return f >> 24;
}

uint32_t ceph_frag_value(uint32_t f)
{
  return f & 0xffffffu;
}

void usage()
{
  printf("./bit <bits> <value> </path/to/dir>\n");
}

int main(int argc, char *argv[])
{
  if (argc < 4) {
    usage();
    exit(1);
  }
  uint32_t bits = (uint32_t)strtoul(argv[1], NULL, 10);
  uint32_t value = (uint32_t)strtoul(argv[2], NULL, 10);
  unsigned hash_val = ceph_str_hash_linux(argv[3], strlen(argv[3])); 
  value = hash_val;
  uint32_t frag = ceph_frag_make(bits, hash_val);
  printf("%u/%u --> %x : %u\n", value, bits, frag, frag);
  printf("%u/%u <-- %x : %u\n", ceph_frag_value(frag), ceph_frag_bits(frag), frag, frag);
  return 0;
}

