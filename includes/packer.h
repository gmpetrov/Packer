#ifndef PACKER_H
# define PACKER_H

#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

typedef struct 	s_data
{
		void	*ptr;
		struct stat buf;

		Elf64_Ehdr *header;
		Elf64_Phdr *program_header;
		Elf64_Shdr *section_header_table;	
		unsigned char **section_data;
}				t_data;


int 	find_bytes_index(unsigned char *bytes, unsigned char *sequence, int bytes_len, int sequence_len);
void    overwrite_bytes(int index, unsigned char *bytes, unsigned char *sequence, int sequence_len);
void    dump_bytes(unsigned char *bytes, int bytes_len);
void	inject_payload(t_data *data);
void	generate_elf(t_data *data);
void	fill_file_zero(FILE *file, int size);
void	write_to_file(FILE *file, int offset, void *data, int size);


#endif
