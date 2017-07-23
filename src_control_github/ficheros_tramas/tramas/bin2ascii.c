#include <unistd.h>
#include <stdio.h>

int main () {
	unsigned char value;
	while (read (0, &value, 1) > 0) {
		for (int j=0; j<8; ++j) {
			char ch = (value & 0x80)? '1' : '0';
			write (1, &ch, 1);
			value <<= 1;
		}
	}
	puts ("");
}
