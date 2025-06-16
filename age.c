#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

struct fish {
	char		spec;
	char		sex;
	unsigned int	age;
	unsigned int	size;
#define MAXAGE	100
#define MAXSIZE	UINT_MAX
	struct fish*	prev;
	struct fish*	next;
};

struct list {
	struct fish *head;
	struct fish *tail;
	unsigned int num;
};

#define X 4
#define Y 4
struct list* grid[X][Y];
struct list* next[X][Y];
int alive = 0;


struct fish*
randfish()
{
	struct fish *f = NULL;
	f = calloc(1, sizeof(struct fish));
	f->spec = arc4random_uniform(2) ? 'A' : 'B';
	f->sex  = arc4random_uniform(2) ? 'M' : 'F';
	f->size = 1;
	return f;
}

/* Get a random treshold between 90% and 100% of MAXAGE.
 * If a fish is older than that, it dies of old age.*/
int
die(struct fish *f)
{
	return f && f->age >=
		((100.0 - arc4random_uniform(10)) / 100.0) * MAXAGE;
}

/* Let the fish grow a day older.
 * The age influences sexual maturity, strength, and dying probability. */
void
grow(struct fish *f)
{
	f->age++;
}

void
add(struct fish* f, struct list* list)
{
	if (f == NULL || list == NULL)
		return;
	if (list->num == 0) {
		list->head = f;
		list->tail = f;
	} else {
		f->prev = list->tail;
		list->tail->next = f;
		list->tail = f;
	}
	list->num++;
}

void
del(struct fish* f, struct list* list)
{
	if (f  == NULL || list == NULL || list->num == 0)
		return;
	if (f->prev)
		f->prev->next = f->next;
	else if ((list->head = f->next))
		list->head->prev = NULL;
	if (f->next)
		f->next->prev = f->prev;
	else if ((list->tail = f->prev))
		list->tail->next = NULL;
	f->prev = f->next = NULL;
	list->num--;
}

void
picture()
{
	unsigned int x, y;
	puts("generation:");
	for (x = 0; x < X; x++) {
		for (y = 0; y < Y; y++) {
			putchar(grid[x][y]->num ? grid[x][y]->head->spec : ' ');
		}
		putchar('\n');
	}
	putchar('\n');
}

/* Populate the grid randomly.
 * Return the number of fish. */
void
creation()
{
	unsigned int x, y;
	for (x = 0; x < X; x++) {
		for (y = 0; y < Y; y++) {
			grid[x][y] = calloc(1, sizeof(struct list));
			next[x][y] = calloc(1, sizeof(struct list));
			if (!arc4random_uniform(4)) {
				add(randfish(), grid[x][y]);
				alive = 1;
			}
		}
	}
}

/* Let the fish on a given list
 * either die of old age, or grow a day older */
void
reaper(struct list *list)
{
	struct fish *f = NULL;
	if (list == NULL)
		return;
	for (f = list->head; f; f = f->next) {
		if (die(f)) {
			del(f, list);
		} else {
			grow(f);
		}
	}
}

void
evolution()
{
	unsigned int x, y;
	alive = 0;
	for (x = 0; x < X; x++) {
		for (y = 0; y < Y; y++) {
			reaper(grid[x][y]);
			if (grid[x][y]->num)
				alive = 1;
		}
	}
}

int
main(int argc, char** argv)
{
	creation();
	do {
		picture();
		evolution();
	} while (alive);
	return 0;
}
