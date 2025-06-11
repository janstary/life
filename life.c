#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <err.h>

int bits = 1;
int loop = 1;
int less = 2;
int more = 3;

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

void
prgrid(struct grid * grid)
{
	uint32_t i, j, w;
	if (grid == NULL)
		return;
	w = (grid->bits / 4) + 1; /* FIXME */
	for (i = 0; i < grid->rows; i++) {
		for (j = 0; j < grid->cols; j++)
			printf("%0*x ", w, grid->cell[i][j]);
		putchar('\n');
	}
	printf("gen %u: %u live cells\n", grid->time, grid->live);
}

struct grid*
init(uint32_t rows, uint32_t cols, int bits)
{
	uint32_t i;
	struct grid * grid = NULL;
	if ((grid = (struct grid *) calloc(1, sizeof(struct grid))) == NULL)
		err(1, NULL);
	if ((grid->cell = calloc(rows, sizeof(uint32_t*))) == NULL)
		err(1, NULL);
	if ((grid->next = calloc(rows, sizeof(uint32_t*))) == NULL)
		err(1, NULL);
	for (i = 0; i < rows; i++) {
		if ((grid->cell[i] = calloc(cols, sizeof(uint32_t))) == NULL)
			err(1, NULL);
		if ((grid->next[i] = calloc(cols, sizeof(uint32_t))) == NULL)
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
	mask = (1 << bits) - 1;
	/* Init whole rows, regardless of bit width - there is nothing
	 * to be gained by doing the random init by individual cells. */
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

void
bstep(struct grid * grid, uint32_t r, uint32_t c, unsigned b)
{
	uint32_t mask = 0;
	uint32_t nbhd = 0;
	uint32_t pr, pc, nr, nc;

	pr = r > 0 ? r - 1 : grid->rows - 1;
	pc = c > 0 ? c - 1 : grid->cols - 1;
	nr = (r + 1) % grid->rows;
	nc = (c + 1) % grid->cols;
	mask = 1 << b;
	nbhd =  (
		(grid->cell[pr][pc] & mask) +
		(grid->cell[pr][ c] & mask) +
		(grid->cell[pr][nc] & mask) +
		(grid->cell[ r][pc] & mask) +
		(grid->cell[ r][nc] & mask) +
		(grid->cell[nr][pc] & mask) +
		(grid->cell[nr][ c] & mask) +
		(grid->cell[nr][nc] & mask) )
		>> b;
	/*printf("(%u,%u)[%u] has %u\n", r, c, b, nbhd);*/
	if (nbhd == less) {
		grid->next[r][c] |= mask;
	} else if ((nbhd < less) || (nbhd > more)) {
		grid->next[r][c] &= ~mask;
	}
}

void
cstep(struct grid * grid, uint32_t r, uint32_t c)
{
	unsigned b;
	grid->next[r][c] = grid->cell[r][c];
	/* FIXME: copy whole rows with memset() */
	for (b = 0; b < grid->bits; b++)
		bstep(grid, r, c, b);
}

void
step(struct grid * grid)
{
	uint32_t r, c;
	if (grid == NULL)
		return;
	if (grid->live == 0)
		return;
	grid->time++;
	grid->live = 0;
	for (r = 0; r < grid->rows; r++)
		for (c = 0; c < grid->cols; c++)
			cstep(grid, r, c);
	for (r = 0; r < grid->rows; r++) {
		for (c = 0; c < grid->cols; c++) {
			/* FIXME: copy whole rows with memcpy()
			 * That means do live++ inside cstep(). */
			if ((grid->cell[r][c] = grid->next[r][c]))
				grid->live++;
		}
	}
}

void
usage(void)
{
	errx(1, "life [-b bits] [-c num] [-g num] input [output]");
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

	while ((c = getopt(argc, argv, "b:c:g:l")) != -1) switch (c) {
		case 'b':
			if (((bits = strtonum(optarg, 1, 32, &e)) == 0) && e)
				errx(1, "%s is %s", optarg, e);
			break;
		case 'c':
			if (((back = strtonum(optarg, 1, 16, &e)) == 0) && e)
				errx(1, "%s is %s", optarg, e);
			break;
		case 'g':
			if (((stop = strtonum(optarg,1,UINT32_MAX,&e))==0) && e)
				errx(1, "%s is %s", optarg, e);
			break;
		case 'l':
			loop = 0;
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
			if ((grid = init_rand(h, w, bits)) == NULL)
				errx(1, "Cannot init random %ux%u grid", w, h);
		}
	}

	if (argc--) {
		if (strncmp("-", argv[0], 2)) {
			ofile = STDOUT_FILENO;
		} else if ((ofile = open(argv[0], O_WRONLY)) > 0) {
		}
	}

	prgrid(grid);
	while (grid->live) {
		if (stop && (grid->time == stop))
			break;
		step(grid);
		prgrid(grid);
	}

	close(ifile);
	close(ofile);

	return 0;
}
