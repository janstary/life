#include <stdlib.h>
#include <unistd.h>

int bflag = 1;
int cflag = 0;
int gflag = 0;
int kflag = 0;

uint64_t backgen = 0;
uint64_t stopgen = 0;

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

	return 0;
}
