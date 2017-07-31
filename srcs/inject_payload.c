#include "packer.h"

extern void payload(void);
extern int payload_size;

unsigned char *craft_payload(uint32_t target_addr)
{
	int index;
	unsigned char bytes_entry[4];
	unsigned char *bytes;

	bytes = malloc(sizeof(unsigned char) * payload_size);
	memcpy(bytes, payload, payload_size);
	
	if ((index = find_bytes_index(bytes, "\x44\x44\x44\x44", payload_size, 4)) < 0){
			printf("Can't find bytes sequence\n");
			exit(EXIT_FAILURE);
	}

	// littlen endian
	bytes_entry[0] = target_addr & 0xff;
	bytes_entry[1] = (target_addr >> 8) & 0xff;
	bytes_entry[2] = (target_addr >> 16) & 0xff;
	bytes_entry[3] = (target_addr >> 24) & 0xff;

	overwrite_bytes(index, bytes, bytes_entry, 4);
	return bytes;
}

int		find_load_segment(t_data *data)
{
	int i;

	for (i = 0; i < data->header->e_phnum; i++) {
		uint32_t type = data->program_header[i].p_type;	
		uint32_t flags = data->program_header[i].p_flags;	
		if (type == PT_LOAD && flags & PF_X) {
				return i;
		}
	}
	return -1;
}

void	inject_payload(t_data *data)
{
	int i;
	int index;
	int new_offset;
	int new_addr;
	int last_ptload_index;
	int nb_sections;
	unsigned char *bytes;
	unsigned char **tmp;
	Elf64_Phdr last_ptload;
	Elf64_Shdr *tmp_section_header;

	bytes = craft_payload(data->header->e_entry);

	// update header
	data->header->e_shnum += 1;
	nb_sections = data->header->e_shnum;

	// update section header
	tmp_section_header = realloc(data->section_header_table, sizeof(Elf64_Shdr) * data->header->e_shnum);
	if (!tmp_section_header){
			fprintf(stderr, "error realloc section_header_table\n");
			exit(EXIT_FAILURE);
	}
	data->section_header_table = tmp_section_header;

	last_ptload_index = find_load_segment(data);
	last_ptload = data->program_header[last_ptload_index];

	new_offset = last_ptload.p_offset + last_ptload.p_memsz;
	new_addr = last_ptload.p_vaddr + last_ptload.p_memsz;
	data->program_header[last_ptload_index].p_memsz += payload_size;
	data->program_header[last_ptload_index].p_filesz += payload_size;

	data->section_header_table[nb_sections - 1].sh_name = 0;
	data->section_header_table[nb_sections - 1].sh_type = SHT_PROGBITS;
	data->section_header_table[nb_sections - 1].sh_flags = SHF_EXECINSTR | SHF_ALLOC;
	data->section_header_table[nb_sections - 1].sh_addr = new_addr;
	data->section_header_table[nb_sections - 1].sh_offset = new_offset;
	data->section_header_table[nb_sections - 1].sh_size = payload_size;
	data->section_header_table[nb_sections - 1].sh_link = 0;
	data->section_header_table[nb_sections - 1].sh_info = 0;
	data->section_header_table[nb_sections - 1].sh_entsize = 0;

	data->header->e_entry = new_addr;

	tmp = realloc(data->section_data, sizeof(unsigned char *) * data->header->e_shnum);

	if (!tmp) {
			fprintf(stderr, "error realloc data->section_data\n");
			exit(EXIT_FAILURE);
	}

	data->section_data = tmp;
	data->section_data[nb_sections - 1] = malloc(sizeof(payload_size));
	data->section_data[nb_sections - 1] = bytes;

	data->buf.st_size += payload_size;

/*
	for (i = 0; i < nb_sections; i++){
			printf("section addr: %p, section size : %d, section offset : %d\n", (void *)data->section_header_table[i].sh_addr, (int)data->section_header_table[i].sh_size, (int)data->section_header_table[i].sh_offset);
	}

*/

	/*
	for (i = 0; i < data->header->e_phnum; i++) {
			printf("segment addr : %p, segment offset %d, segment memsz: %d\n", (void *)data->program_header[i].p_vaddr, (int)data->program_header[i].p_offset, (int)data->program_header[i].p_memsz);
	}

	*/
}
