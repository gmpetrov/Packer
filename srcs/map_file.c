#include "packer.h"

void	map_elf64(t_data *data)
{
	int i;

	// Copy file's header
	data->header = malloc(sizeof(Elf64_Ehdr));
	memcpy(data->header, data->ptr, sizeof(Elf64_Ehdr));	
	
	// Copy file segment's header;
	data->program_header = malloc(sizeof(Elf64_Phdr) * data->header->e_phnum);
	memcpy(data->program_header, data->ptr + data->header->e_phoff, sizeof(Elf64_Phdr) * data->header->e_phnum);

	// Copy file sections's header
	data->section_header_table = malloc(sizeof(Elf64_Shdr) * data->header->e_shnum);
	memcpy(data->section_header_table, data->ptr + data->header->e_shoff, sizeof(Elf64_Shdr) * data->header->e_shnum);

	// Copy data section
	data->section_data = malloc(sizeof(unsigned char *) * data->header->e_shnum);

	for (i = 0; i < data->header->e_shnum; i++){
			data->section_data[i] = malloc(data->section_header_table[i].sh_size);
			memcpy(data->section_data[i], data->ptr + data->section_header_table[i].sh_offset, data->section_header_table[i].sh_size);
	}

}

void	map_file(t_data *data)
{
	Elf64_Ehdr *header = (Elf64_Ehdr *)data->ptr;

	if (header->e_ident[EI_CLASS] == ELFCLASS64){
			map_elf64(data);
	}
	else{
		fprintf(stderr, "Handling format elf64 only\n");
	}
}
