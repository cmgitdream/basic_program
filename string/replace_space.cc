
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

// ' ' -> "%20"
int replace_space(char *input, int size, int *count)
{
	int i = 0;
	int len = strlen(input);
	int posi = len;
	*count = 0;
	for (i = 0; i < size && input[i] != '\0'; i++) {
	printf("size:%d, len: %d, posi:%d, input = \"%s\"\n", size, len, posi, input);
		if (input[i] == ' ') {
			printf("found pos at %d\n", i);
			if (posi + 2 >= size) {
				printf("array has no enough length at pos %d\n", i);
				return -1;
			}
			*count = *count + 1;
			if (strlen(&input[i + 1]) > 0)
				memmove(&input[i + 3], &input[i + 1], strlen(&input[i + 1]));
			input[i] = '%';
			input[i+1] = '2';
			input[i+2] = '0';
			posi += 2;
		}
	}
	return 0;
}

// reverse copy after extend 2*count chars postion
int replace_space2(char *input, int size, int *count)
{
  char *ptr = input;
  int len = strlen(input);
  int i = 0, pos = 0;
  int spaces = 0;
  *count = 0;
  for (i = 0; i < size && input[i] != '\0'; i++) {
    if (input[i] == ' ')
      spaces++;
  }
  if (spaces == 0) {
    printf("array has no space, do not insert %%20\n");
    return 0;
  }
  *count = spaces;
  if ((len +  2 * spaces) >= size) {
    printf("array has no enough length\n");
    return -1;
  }
  // last index after expanding
  pos = len - 1 + 2 * spaces;
  for (i = len - 1; i >= 0; i--) {
    printf("size:%d, len: %d, input = \"%s\"\n", size, len, input);
    if (input[i] == ' ') {
      printf("found pos at %d\n", i);
      input[pos--] = '0';
      input[pos--] = '2';
      input[pos--] = '%';
    } else {
      input[pos--] = input[i];
    }
  }
  return 0;
}

int main(int argc, char *argv[])
{
	//char *str = " this   is     a test string"; 
	//char *str = "thisisateststring";
	char *str = "this is a test string";
	//char *str = "          ";
	int len = strlen(str);
	int total = len + 22 + 1;
	int count;
	char *ptr = (char *)malloc(total * sizeof(char));
	if (ptr == NULL) {
		printf("malloc error\n");
		return -1;
	}

	memset(ptr, '\0', total);
	memcpy(ptr, str, len);
	
	printf("before: ptr = \"%s\"\n", ptr);
	replace_space2(ptr, total, &count);	
	printf("after: ptr = \"%s\", space count = %d\n", ptr, count);
	
	if (ptr)
		free(ptr);
	return 0;
}
