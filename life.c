#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <err.h>

int bflag = 1;
int cflag = 0;
int gflag = 0;
int kflag = 0;

uint64_t backgen = 0;
uint64_t stopgen = 0;

void
usage()
{
	errx(1, "life [-C num] [-G num] [-bcgk] input [output]");
}

int
main(int argc, char** argv)
{
	int c;
	const char* e = NULL;

	while ((c = getopt(argc, argv, "bC:cG:gk")) != -1) switch (c) {
		case 'b':
			bflag = 1;
			break;
		case 'C':
			backgen = strtonum(optarg, 1, 16, &e);
			break;
		case 'c':
			cflag = 0;
			break;
		case 'G':
			stopgen = strtonum(optarg, 1, UINT64_MAX, &e);
			break;
		case 'g':
			gflag = 0;
			break;
		case 'k':
			kflag = 0;
			break;
	}
	argc -= optind;
	argv += optind;

	if (argc == 0)
		usage();

	return 0;
}
