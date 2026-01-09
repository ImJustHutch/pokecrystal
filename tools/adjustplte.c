#define PROGRAM_NAME "adjustplte"
#define USAGE_OPTS "[-h|--help]"

#include "common.h"
#include <dirent.h>
#include <sys/stat.h>

bool check_bw(uint8_t *rgb) {
	return ((rgb[0] == 0 || rgb[0] == 255) && rgb[0] == rgb[1] && rgb[1] == rgb[2]); 
}

uint8_t to5bit(uint8_t num) {
	return ((num * 31 + 127) / 255);
}

uint8_t to8bit(uint8_t num) {
	return ((num * 255 - 127) / 31);
}

 /* Table of CRCs of all 8-bit messages. */
uint32_t crc_table[256];
   
   /* Flag: has the table been computed? Initially false. */
//int crc_table_computed = 0;
   
   /* Make the table for a fast CRC. */
void make_crc_table(void) {
	uint32_t c;
	size_t n, k;
	
	for (n = 0; n < 256; n++) {
		c = (uint32_t) n;
		for (k = 0; k < 8; k++) {
			if (c & 1)
			c = 0xedb88320L ^ (c >> 1);
			else
			c = c >> 1;
		}
		crc_table[n] = c;
	}
	//crc_table_computed = 1;
}
   
   /* Update a running CRC with the bytes buf[0..len-1]--the CRC
      should be initialized to all 1's, and the transmitted value
      is the 1's complement of the final running CRC (see the
      crc() routine below)). */
   
uint32_t update_crc(uint32_t crc, uint8_t *buf, uint32_t len) {
	uint32_t c = crc;
	size_t n;

	//if (!crc_table_computed)
		make_crc_table();
	for (n = 0; n < len; n++) {
		c = crc_table[(c ^ buf[n]) & 0xff] ^ (c >> 8);
	}
	return c;
}
   
   /* Return the CRC of the bytes buf[0..len-1]. */
uint32_t crc(uint8_t *buf, uint32_t len) {
 return update_crc(0xffffffffL, buf, len) ^ 0xffffffffL;
}

uint32_t reverse_bytes_bitwise(uint32_t value) {
    return ((value & 0x000000FFU) << 24) | ((value & 0x0000FF00U) << 8) | ((value & 0x00FF0000U) >> 8)  | ((value & 0xFF000000U) >> 24);
}

const int8_t deltaG0[32] = {0, -1, -2, -3, -3, -4, -4, -4, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -6, -6, -6, -6, -6, -7, -7, -7, -8, -8, -9, -10, -10};
const int8_t deltaG1[32] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2};
const int8_t deltaG2[32] = {0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 3, 3, 4, 4, 5, 5, 5, 6, 6, 7, 7, 7, 8, 8, 8, 9, 9, 9, 9, 10, 10};
const int8_t deltaRB[32] = {0, -1, -2, -2, -3, -3, -3, -3, -3, -3, -3, -2, -2, -2, -1, -1, 0, 0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0};
const int8_t gamma_delta[32] = {0, 0, 3, 4, 5, 6, 7, 8, 8, 8, 8, 8, 8, 8, 7, 7, 7, 6, 6, 5, 5, 4, 3, 3, 2, 2, 2, 1, 1, 0, 0, 0};
const int8_t desat_delta[32] = {-0, -0, -0, -0, -0, -0, -0, -0, -1, -1, -1, -1, -1, -1, -1, -2, -2, -2, -2, -2, -2, -2, -2, -3, -3, -3, -3, -3, -3, -3, -3, -3};

void update_rgb(uint8_t *rgb, uint8_t *rgb_adj) {
	for (size_t i = 0; i< 3; i++)
		rgb[i] = rgb_adj[i];
}

void limit_range(uint8_t *rgb_adj) {
	for (size_t i = 0; i< 3; i++)
		if (rgb_adj[i] == 31)
			rgb_adj[i] = 31;
}

/*
	the adjusted RGB asm macro more or less converted to C.
	takes 3 pointers to 5bit color channels for a single colors
	and does some adjustments based on the delta values above.
	even though pngs use 8 bits to store channel data, it's okay
	to convert down since no information is lost that wouldn't
	already be lost at compile time.
	does occasional checks when values could exceed 31, but it
	shouldn't be possible to underflow given the range of 5 bit
	channel values and predetermined deltas.
	assumes that it will not be fed pure white or pure black.
*/
void adjust_rgb(uint8_t *r, uint8_t *g, uint8_t *b) {
	
	//r & g are swapped for easier iterating
    uint8_t rgb[3] = {*g, *r, *b};
    uint8_t rgb_adj[3] = {*g, *r, *b};

	// --- GBC color filter ---
	rgb_adj[0] += deltaG0[rgb[0]];
	rgb_adj[0] += deltaG1[rgb[1]];
	rgb_adj[0] += deltaG2[rgb[2]];
	
	for (size_t i = 1; i < 3; i++)
		rgb_adj[i] += deltaRB[rgb[i]];
	
	limit_range(rgb_adj);
		
	// --- Gamma filter ---
	update_rgb(rgb, rgb_adj);
	
	for (size_t i = 0; i < 3; i++)
		rgb_adj[i] += gamma_delta[rgb[i]];
	
	limit_range(rgb_adj);
		
	// --- Desaturation filter ---
	update_rgb(rgb, rgb_adj);
	
	for (size_t i = 0; i < 3; i++)
		rgb_adj[i] += desat_delta[rgb[i]];
		
	// Additional adjustments
	if (rgb[1] > 11)
		rgb_adj[0] += 1;
	if (rgb[2] > 11)
		rgb_adj[0] += 1;
	if (rgb[0] > 11) {
		rgb_adj[1] += 1;
		rgb_adj[2] += 1;
	}
	if (rgb[2] > 26)
		rgb_adj[1] += 2;
	else if (rgb[2] > 10)
		rgb_adj[1] += 1;
	if (rgb[1] > 26)
		rgb_adj[2] += 2;
	else if (rgb[1] > 10)
		rgb_adj[2] += 1;
	
	limit_range(rgb_adj);
	
	//printf("input r: 0x%" PRIx8 "\n", rgb[0]);	//for testing
	//printf("input g: 0x%" PRIx8 "\n", rgb[1]);	//for testing
	//printf("input b: 0x%" PRIx8 "\n", rgb[2]);	//for testing
	
	*r = rgb_adj[1];
	*g = rgb_adj[0];
	*b = rgb_adj[2];
	
	//printf("adjusted r: 0x%" PRIx8 "\n", rgb_adj[0]);	//for testing
	//printf("adjusted g: 0x%" PRIx8 "\n", rgb_adj[1]);	//for testing
	//printf("adjusted b: 0x%" PRIx8 "\n", rgb_adj[2]);	//for testing
}

/*
	this is where the majority of the work is done.
	opens the png file and verifies that it's valid.
	finds the PLTE chunk (if there is one) and reads
	the palette info from it. calls adjust_rgb to
	modify any non-white, non-black colors.
	and writes the new colors to the file.
	recalculates the Cyclic Redundancy Checksum (CRC)
	and writes that to file as well.
	assumes that the PLTE chunk comes after the IHDR
	chunk since that's how all of the colored pngs
	are structured. if using a modified png, make
	sure there are no other chunks.
*/

void adjust_plte(const char *filename) {
	FILE *f = fopen(filename, "r+");
	//validate png header
	uint8_t header[16] = {0};
	xfread(header, sizeof(header), filename, f);
	static uint8_t expected_header[16] = {
		0x89, 'P', 'N', 'G', '\r', '\n', 0x1A, '\n', // signature
		0, 0, 0, 13,                                 // IHDR chunk length
		'I', 'H', 'D', 'R',                          // IHDR chunk type
	};
	if (memcmp(header, expected_header, sizeof(header))) {
		fclose(f);
		error_exit("Not a valid PNG file: \"%s\"\n", filename);
	}
	//skip the rest of the IHDR chunk
	fseek(f, 17, SEEK_CUR);
	//grab the chunk length
	uint8_t len_bytes[4] = {0};
	xfread(len_bytes, sizeof(len_bytes), filename, f);
	//grab the chunk name
	static uint8_t expected_plte[4] = {'P','L','T','E'};
	uint8_t name_bytes[4] = {0};
	xfread(name_bytes, sizeof(name_bytes), filename, f);
	//make sure the next chunk is a PLTE chunk. if not, close the file.
	if (memcmp(name_bytes, expected_plte, sizeof(name_bytes))) {
		fclose(f);
	}
	else { //otherwise, continue
		//build length
		uint32_t length = (len_bytes[0] << 24) | (len_bytes[1] << 16) | (len_bytes[2] << 8) | len_bytes[3];
		//3 channels per palette (R G & B), so divide by 3 to get # of palettes
		uint32_t num_pal = length / 3;
		uint8_t pltes[num_pal][3] = {};
		//get the RGB values
		for (size_t i = 0; i < num_pal; i++)
			xfread(pltes[i], 3, filename, f);
		//convert 8bit channel to 5 bit
		//uint32_t num_colors = num_pal;
		for (size_t i = 0; i < num_pal; i++)
			//ignore pure black and pure white
			if (!(check_bw(pltes[i])))
				for (size_t j = 0; j < 3; j++)
					pltes[i][j] = to5bit(pltes[i][j]);
		//adjust the colors
		for (size_t i = 0; i < num_pal; i++) {
			if (!(check_bw(pltes[i])))
				adjust_rgb(&pltes[i][0], &pltes[i][1], &pltes[i][2]);
		}
		//convert the 5bit channels back to 8bit
		for (size_t i = 0; i < num_pal; i++) {
			if (!(check_bw(pltes[i])))
				for (size_t j = 0; j < 3; j++)
					pltes[i][j] = to8bit(pltes[i][j]);
		}
		//go back to the first palette
		fseek(f, -((int32_t)length), SEEK_CUR);
		//write the adjusted palettes to file
		for (size_t i = 0; i < num_pal; i++) {
			for (size_t j = 0; j < 3; j++)
				xfwrite(&pltes[i][j], 1, filename, f);
		}
		//set up char array to pass to CRC calc
		uint8_t plte[4 + length] = {};
		plte[0] = 'P';
		plte[1] = 'L';
		plte[2] = 'T';
		plte[3] = 'E';
		for (size_t i = 0; i < num_pal; i++)
			for (size_t j = 0; j < 3; j++)
				plte[4+(3*i)+j] = pltes[i][j];
		//recalculate CRC
		uint32_t pltecrc = reverse_bytes_bitwise(crc(plte, length + 4));
		uint8_t *crc_bytes = &pltecrc; //too lazy to do a proper conversion. this will always work since CRC is always 4 bytes
		for (size_t i = 0; i < 4; i++)
			xfwrite(&crc_bytes[i], 1, filename, f);
		fclose(f);
	}
}

bool is_png(const char *filename) {
    const char *ext = strrchr(filename, '.');
    return ext && strcmp(ext, ".png") == 0;
}

void search_directory(const char *path) {
    DIR *dir = opendir(path);
    if (!dir) {
        perror(path);
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {

        /* Skip . and .. */
        if (strcmp(entry->d_name, ".") == 0 ||
            strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        char fullpath[1024];
        snprintf(fullpath, sizeof(fullpath), "%s/%s", path, entry->d_name);

        struct stat st;
        if (stat(fullpath, &st) == -1) {
            perror(fullpath);
            continue;
        }

        if (S_ISDIR(st.st_mode)) {
            /* Recurse into subdirectory */
            search_directory(fullpath);
        }
		else if (S_ISREG(st.st_mode)) {
            /* Regular file */
            if (is_png(entry->d_name)) {
                printf("%s\n", fullpath);
				//printf("%s\n", entry->d_name);
				adjust_plte(fullpath);
            }
        }
    }
    closedir(dir);
}

int main(int argc, char *argv[]) {
	if (argc != 2) {
        fprintf(stderr, "Usage: %s <directory>\n", argv[0]);
        return 1;
    }
    search_directory(argv[1]);
    return 0;
}