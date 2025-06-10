#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <err.h>

int nbits = 1;
int kflag = 0;

uint32_t back = 0;
uint32_t stop = 0;

struct grid {
	uint32_t bits;
	uint32_t time;
	uint32_t live;
	uint32_t rows;
	uint32_t cols;
	uint32_t **cell;
	uint32_t **next;
};

struct grid*
init(uint32_t rows, uint32_t cols, int bits)
{
	uint32_t i;
	struct grid * grid = NULL;
	if ((grid = (struct grid *) calloc(1, sizeof(struct grid))) == NULL)
		err(1, NULL);
	if ((grid->cell = calloc(rows, sizeof(uint32_t*))) == NULL)
		err(1, NULL);
	for (i = 0; i < rows; i++) {
		if ((grid->cell[i] = calloc(cols, sizeof(uint32_t))) == NULL)
			err(1, NULL);
	}
	grid->rows = rows;
	grid->cols = cols;
	grid->bits = bits;
	return grid;
}

struct grid*
init_rand(uint32_t rows, uint32_t cols, int bits)
{
	uint32_t i, j, mask;
	struct grid * grid = NULL;
	if ((grid = init(rows, cols, bits)) == NULL)
		return NULL;
	mask = (bits == 1) ? 1 : (bits == 8 ? 0x000000ff : 0x00ffffff);
	/* Init whole rows, regardless of bit width - there is nothing
	 * to be gained by doing the random init by individual cells.
	 * Then trim the irrelevant bits. */
	for (i = 0; i < rows; i++) {
		arc4random_buf(grid->cell[i], cols * sizeof(uint32_t));
		for (j = 0; j < cols; j++)
			grid->live += (grid->cell[i][j] &= mask) > 0;
	}
	return grid;
}

struct grid*
init_txt(int ifile)
{
	return NULL;
}

struct grid*
init_png(int ifile)
{
	return NULL;
}

/* We need to make a copy of the entire grid,
 * as rewriting the cells in place would
 * alter the subsequent cells done later. */
void
step(struct grid * grid)
{
	if (grid == NULL)
		return;
	if (grid->live == 0)
		return;
	grid->time++;
}

void
prgrid(struct grid * grid)
{
	uint32_t i, j, w;
	if (grid == NULL)
		return;
	w = grid->bits == 24 ? 6 : (grid->bits == 8) ? 2 : 1;
	for (i = 0; i < grid->rows; i++) {
		for (j = 0; j < grid->cols; j++)
			printf("%0*x ", w, grid->cell[i][j] & 0x00ffffff);
		putchar('\n');
	}
	printf("gen %u: %u live cells\n", grid->time, grid->live);
}

void
usage(void)
{
	errx(1, "life [-C num] [-G num] [-bcgk] input [output]");
}

int
main(int argc, char** argv)
{
	int c;
	char *p = NULL;
	int ifile = -1;
	int ofile = -1;
	const char * e = NULL;
	struct grid * grid = NULL;

	uint32_t w = 0;
	uint32_t h = 0;

	while ((c = getopt(argc, argv, "bC:cG:gk")) != -1) switch (c) {
		case 'b':
			nbits = 1 * 1;
			break;
		case 'C':
			back = strtonum(optarg, 1, 16, &e);
			if (e)
				errx(1, "%s is %s", optarg, e);
			break;
		case 'c':
			nbits = 3 * 8;
			break;
		case 'G':
			stop = strtonum(optarg, 1, UINT32_MAX, &e);
			if (e)
				errx(1, "%s is %s", optarg, e);
			break;
		case 'g':
			nbits = 1 * 8;
			break;
		case 'k':
			kflag = 0;
			break;
	}
	argc -= optind;
	argv += optind;

	if (argc == 0 || argc > 2)
		usage();

	if (argc--) {
		if (strncmp("-", argv[0], 2) == 0) {
			ifile = STDIN_FILENO;
		} else if ((ifile = open(argv[0], O_RDONLY)) > 0) {
			if ( (p = strrchr(argv[0], '.'))
			&& (strncmp("png", ++p, 4) == 0) ) {
				/* init_png(); */
			} else {
				/* init_txt(); */
			}
		} else {
			if ((p = strchr(argv[0], 'x')) == NULL)
				errx(1, "%s is not a WxH size", argv[0]);
			*p++ = '\0';
			if ((0 == (w=strtonum(argv[0],1,UINT32_MAX, &e))) && e)
				errx(1, "%s is not a width", argv[0]);
			if ((0 == (h = strtonum(p, 1, UINT32_MAX, &e))) && e)
				errx(1, "%s is not a height", p);
			if ((grid = init_rand(h, w, nbits)) == NULL)
				errx(1, "Cannot init random %ux%u grid", w, h);
			prgrid(grid);
		}
	}

	if (argc--) {
		if (strncmp("-", argv[0], 2)) {
			ofile = STDOUT_FILENO;
		} else if ((ofile = open(argv[0], O_WRONLY)) > 0) {
		}
	}

	while (grid->live) {
		if (stop && (grid->time == stop))
			break;
		step(grid);
	}

	prgrid(grid);
	close(ifile);
	close(ofile);

	return 0;
}
