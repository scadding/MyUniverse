/* rand(n) - a random integer generator with 20 bit resolution */

static int random_first = 1;

rand(n)		/* generate a number from 1 to n, n < 1.0EE6 */
int n;
{
	long rnd;
#ifdef DOUBLE_RND
	double tmp;
#endif
	int val;

	if (random_first) {
		srandom(time((long)0));
		random_first=0;
	}
	rnd = random() & 0xFFFFF;
#ifdef DOUBLE_RND
	tmp = (double)rnd/0x100000;
	val = (int)(tmp*n);
#else
	val = rnd % n;
#endif
	return(++val);
}

