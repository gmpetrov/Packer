#include <stdio.h>

int find_bytes_index(unsigned char *bytes, unsigned char *sequence, int bytes_len, int sequence_len)
{
		int      i;
		int  j;

		i = 0;
		while (i < bytes_len) {
				if (bytes[i] == *sequence) {
						j = 0;
						while (j < sequence_len) {
								if (i + j < bytes_len && bytes[i + j] != sequence[j]) {
										break ;
								}
								j++;
						}

						if (j == sequence_len) {
								return i;
						}
				}
				i++;
		}
		return (-1);
}

void    overwrite_bytes(int index, unsigned char *bytes, unsigned char *sequence, int sequence_len)
{
		int     i;

		for (i = 0; i < sequence_len; i++) {
				bytes[index + i] = sequence[i];
		}
}

void    dump_bytes(unsigned char *bytes, int bytes_len)
{
		int i;

		for (i = 0; i < bytes_len; i++) {
				if (i % 16 == 0)
						printf("%.2x\n", bytes[i]);
				else
						printf("%.2x ", bytes[i]);
		}
		printf("\n");
}
