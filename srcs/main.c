#include "packer.h"

void	clean(t_data *data)
{	

		int i;

		for (i = 0; i < data->header->e_shnum; i++){
				free(data->section_data[i]);
		}
		free(data->header);
		free(data->program_header);
		free(data->section_header_table);
		free(data->section_data);
		free(data);
}

int		main(int ac, char **av)
{
		int fd;
		t_data *data;

		if (ac != 2){
				fprintf(stderr, "[USAGE] - ./packer <binary_file>\n");
				exit(EXIT_FAILURE);				
		}

		if ((fd = open(av[1], O_RDONLY)) < 1) {
				fprintf(stderr, "Error opening file: %s\n", av[1]);
				exit(EXIT_FAILURE);
		}	

		data = (t_data *)malloc(sizeof(t_data));

		if (fstat(fd, &data->buf) < 0) {
				fprintf(stderr, "Error stat filename\n");
				exit(EXIT_FAILURE);
		}


		if ((data->ptr = mmap(0, data->buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED) {
				fprintf(stderr, "[ERROR] - Map Failed\n");
				exit(EXIT_FAILURE);
		}

		map_file(data);

		inject_payload(data);

		generate_elf(data);

		close(fd);
		clean(data);

		return (EXIT_SUCCESS);
}
