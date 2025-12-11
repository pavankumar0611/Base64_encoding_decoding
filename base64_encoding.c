#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char base64_table[]  = 
{ 'A' , 'B' , 'C' , 'D' , 'E' , 'F' , 'G', 'H' , 'I',
	'J' , 'K' , 'L', 'M', 'N', 'O', 'P', 'Q', 'R' , 'S', 
	'T' , 'U', 'V', 'W', 'X', 'Y', 'Z',

	'a', 'b', 'c' , 'd', 'e', 'f', 'g', 'h', 'i', 'j', 
	'k', 'l' , 'm', 'n', 'o' , 'p' , 'q' , 'r', 's',
	't', 'u', 'v' , 'w', 'x' , 'y', 'z',

	'0', '1' , '2', '3' , '4', '5', '6' , '7', '8' , '9' , 

	'+' , '-' };

void function_for_exact_3byte_encoding( char  *arr)
{
	unsigned char current_bit = 0 , next_bit = 0, save = 2;
	for(int i = 0 ;i < 3;  i++)
	{
		if(save == 2) {
			current_bit = arr[i] >> 2;
			printf("%c", base64_table[current_bit]);
			next_bit = (arr[i] & 0x3);
			save = save + 2;
		}
		else if (save == 4) {
			current_bit = arr[i] & 0xf0;
			current_bit = (current_bit >> 4);
			next_bit = next_bit << 4;
			current_bit = (next_bit  | current_bit);
			printf("%c", base64_table[current_bit]);
			next_bit = arr[i] & 0xf;
			save = save + 2;
		}
		else if ( save == 6) {
			next_bit  = next_bit << 2;
			current_bit = ((arr[i] & 0xc0) >> 6);
			current_bit = next_bit | current_bit;
			printf("%c%c",base64_table[current_bit], base64_table[arr[i] & 0x3f]);
			save = 2;
		}
	}
}

void function_for_byte_encoding(char *arr , int size)
{
	if (size % 2 == 0)
	{
		unsigned char current_bit = 0, next_bit = 0, save = 2;
		for(int i = 0; i < size ; i++) {
			if(save == 2) {
				current_bit = arr[i] >> 2;
				printf("%c",base64_table[current_bit]);
				next_bit = arr[i] & 0x3;
				save = save + 2;
			}else if (save == 4) {
				current_bit  = arr[i] & 0xf0;
				current_bit = current_bit >> 4;
				next_bit = next_bit << 4;
				current_bit = (next_bit | current_bit);
				printf("%c",base64_table[current_bit]);
				next_bit = arr[i] & 0xf;
				printf("%c=",base64_table[next_bit << 2]);
			}
		}
	}
	else {
		unsigned char current_bit = 0, next_bit = 0;
		current_bit = arr[0] >> 2;
		printf("%c",base64_table[current_bit]);
		next_bit =  arr[0] & 0x3;
		printf("%c==",base64_table[next_bit << 4]);
	}
}

int main(int argc , char* argv[])
{
	if ( argc < 2 ){
		printf(" ./a.out < string> \n");
		exit(1);
	}

	int length = strlen(argv[1]);
	char *arr = argv[1];

	for(int i = 0 ; i >= 0 ;i = i + 3)
	{
		if (length >= 3 ) {
			function_for_exact_3byte_encoding(&arr[i]);
			length = length - 3;
		}
		else {
			function_for_byte_encoding(&arr[i], length);
			length = 0;
			break;
		}
	}
	printf("\n");
}
