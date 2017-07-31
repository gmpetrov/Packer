#include "packer.h"

void	write_to_file(FILE *file, int offset,  void *data, int size)
{
		fseek(file, offset, SEEK_SET);
		fwrite(data, size, 1, file);
		rewind(file);
}

void	fill_file_zero(FILE *file, int size)
{
	int i = 0;
	for (i = 0; i < size; i++){
		fwrite("\x00", 1, 1, file);
	}
}

void	generate_elf(t_data *data)
{
	int i;
	char file_name[] = "GMP_GENERATED_FILE";
	FILE *file;

	if ((file = fopen(file_name, "w+b")) == NULL) {
		fprintf(stderr, "generate_elf2 - cant't create file\n");
		exit(EXIT_FAILURE);
	}

	// init file - fille with zeros
	fill_file_zero(file, (int)data->buf.st_size);

	// write elf header
	write_to_file(file, 0, data->header, sizeof(Elf64_Ehdr));
	
	// write following program header
	write_to_file(file, (int)data->header->e_phoff, data->program_header, sizeof(Elf64_Phdr) * data->header->e_phnum);	

	// write sections
	for (i = 0; i < data->header->e_shnum; i++){
			write_to_file(file, (int)data->section_header_table[i].sh_offset, data->section_data[i], (int)data->section_header_table[i].sh_size);
	}

	// write sections header table
	write_to_file(file, (int)data->header->e_shoff, (void *)data->section_header_table, sizeof(Elf64_Shdr) * data->header->e_shnum);

	fclose(file);
	chmod(file_name, 0755);
}
