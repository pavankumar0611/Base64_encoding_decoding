#include <stdio.h>
#include <string.h>

char base64_table[]  = 
{ 'A' , 'B' , 'C' , 'D' , 'E' , 'F' , 'G', 'H' , 'I',
	'J' , 'K' , 'L', 'M', 'N', 'O', 'P', 'Q', 'R' , 'S', 
	'T' , 'U', 'V', 'W', 'X', 'Y', 'Z',

	'a', 'b', 'c' , 'd', 'e', 'f', 'g', 'h', 'i', 'j', 
	'k', 'l' , 'm', 'n', 'o' , 'p' , 'q' , 'r', 's',
	't', 'u', 'v' , 'w', 'x' , 'y', 'z',

	'0', '1' , '2', '3' , '4', '5', '6' , '7', '8' , '9' , 

	'+' , '-' };


int main()
{
	char *arr;

	printf("enter the string in multiple of 3's \n");
	scanf("%s",arr);

	unsigned char current_bit = 0 , next_bit = 0, save = 2;
	for(int i = 0 ; i < 6;  i++)
	{
		if(save == 2) {
			current_bit = arr[i] >> 2;
			printf("%c ", base64_table[current_bit]);
			next_bit = (arr[i] & 0x3);
			save = save + 2;
		}
		else if (save == 4) {
			current_bit = arr[i] & 0xf0;
			current_bit = (current_bit >> 4);
			next_bit = next_bit << 4;
			current_bit = (next_bit  | current_bit);
			printf("%c ", base64_table[current_bit]);
			next_bit = arr[i] & 0xf;
			save = save + 2;
		}
		else if ( save == 6) {
			next_bit  = next_bit << 2;
			current_bit = ((arr[i] & 0xc0) >> 6);
			current_bit = next_bit | current_bit;
			printf("%c %c ",base64_table[current_bit], base64_table[arr[i] & 0x3f]);
			save = 2;
		}
	}
}
