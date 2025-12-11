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

// This function encodes exactly 3 input bytes into Base64.
// It processes the 3 bytes in 6-bit chunks, as required by Base64 encoding.
//
// The variables `save` and `next_bit` keep track of leftover bits between
// iterations so that the correct 6-bit groups can be assembled.
//
// Encoding flow for each byte:
//   • When `save == 2`: Extract the first 6 bits (byte >> 2) and save the
//     remaining 2 bits for the next step.
//   • When `save == 4`: Combine the previously saved 2 bits with the next
//     4 bits of the current byte to form a 6-bit group. Save the remaining
//     4 bits for the next step.
//   • When `save == 6`: Combine the stored 4 bits with the top 2 bits of the
//     current byte to form one 6-bit group, then output another 6-bit group
//     using the bottom 6 bits of the current byte. After this, the state
//     resets (`save = 2`).
//
// This logic ensures that each step correctly extracts 6 bits from the
// incoming bytes and feeds them into the Base64 encoding table.

void function_for_exact_3byte_encoding( char  *arr)
{
	unsigned char current_6bits = 0 , next_bit = 0, save = 2;
	for(int i = 0 ;i < 3;  i++)
	{
		if(save == 2) {
			current_6bits = arr[i] >> 2;
			printf("%c", base64_table[current_6bits]);
			next_bit = (arr[i] & 0x3);
			save = save + 2;
		}
		else if (save == 4) {
			current_6bits = arr[i] & 0xf0;
			current_6bits = (current_6bits >> 4);
			next_bit = next_bit << 4;
			current_6bits = (next_bit  | current_6bits);
			printf("%c", base64_table[current_6bits]);
			next_bit = arr[i] & 0xf;
			save = save + 2;
		}
		else if ( save == 6) {
			next_bit  = next_bit << 2;
			current_6bits = ((arr[i] & 0xc0) >> 6);
			current_6bits = next_bit | current_6bits;
			printf("%c%c",base64_table[current_6bits], base64_table[arr[i] & 0x3f]);
			save = 2;
		}
	}
}

// This function handles Base64 encoding when the remaining input
// contains fewer than 3 bytes (i.e., either 1 byte or 2 bytes).
//
// Behavior:
//   • If `size` is even (i.e., 2 bytes remain):
//       The function extracts 6-bit groups from the two bytes using
//       the same bit-shifting technique as the 3-byte encoder.
//       It outputs three Base64 characters and one '=' padding.
//       (Total output: 3 chars + '=').
//
//   • If `size` is odd (i.e., 1 byte remains):
//       Only two 6-bit groups can be formed from a single byte.
//       The function outputs two Base64 characters followed by
//       two '=' padding characters.
//       (Total output: 2 chars + '==').
//
// Variables:
//   • `current_6bits` – Holds the current 6-bit value to map to Base64.
//   • `next_bit`      – Stores leftover bits carried into the next step.
//   • `save`          – Tracks how many bits have been consumed/leftover.
//
// This ensures that partial input at the end of the stream is encoded
// correctly using proper Base64 padding rules.

void function_for_byte_encoding(char *arr , int size)
{
	if (size % 2 == 0)
	{
		unsigned char current_6bits = 0, next_bit = 0, save = 2;
		for(int i = 0; i < size ; i++) {
			if(save == 2) {
				current_6bits = arr[i] >> 2;
				printf("%c",base64_table[current_6bits]);
				next_bit = arr[i] & 0x3;
				save = save + 2;
			}else if (save == 4) {
				current_6bits  = arr[i] & 0xf0;
				current_6bits = current_6bits >> 4;
				next_bit = next_bit << 4;
				current_6bits = (next_bit | current_6bits);
				printf("%c",base64_table[current_6bits]);
				next_bit = arr[i] & 0xf;
				printf("%c=",base64_table[next_bit << 2]);
			}
		}
	}
	else {
		unsigned char current_6bits = 0, next_bit = 0;
		current_6bits = arr[0] >> 2;
		printf("%c",base64_table[current_6bits]);
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
