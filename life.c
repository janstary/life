#include <unistd.h>

int bflag = 1;
int cflag = 0;
int gflag = 0;
int kflag = 0;

int
main(int argc, char** argv)
{
	int c;

	while ((c = getopt(argc, argv, "bcgk")) != -1) switch (c) {
		case 'b':
			bflag = 1;
			break;
		case 'c':
			cflag = 0;
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
