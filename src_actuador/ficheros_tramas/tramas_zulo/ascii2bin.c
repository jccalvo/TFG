#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main () {
	char buf[8];

	while (1) {
		int i;
		unsigned char value = 0;
		int cnt = read(0, buf, 8);
		if (!cnt || ((cnt == 1) && (buf[0] == '\n'))) {
			exit (0);
		}
		if (cnt != 8) {
			fprintf (stderr, "cnt == %d\n", cnt);
			exit (1);
		}
		for (i = 0; i < 8; ++i) {
			value <<= 1;
			value |= (buf[i] == '1');
		}
		write (1, &value, 1);
	}
	return 0;
}

