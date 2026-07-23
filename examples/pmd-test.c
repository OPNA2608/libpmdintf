#include <stdint.h>
#include <stdio.h>

#include <pmd.h>

int main (int argc, char** argv) {
	char far* memoLine;
	unsigned int i;

	if (!pmdIsResident()) {
		fprintf (stderr, "PMD *NOT* resident!\n");
		return 1;
	}

	fprintf (stderr, "PMD resident.\n");

	if (argc > 1) {
		pmd_music_stop();
		pmdLoadFile (argv[1]);
		pmd_music_start();
	}

	pmd_get_filename_address ((void far**) &memoLine);
	// nothing loaded
	if (*memoLine == '\0')
		return 0;

	printf ("File:\t\t%Fs\n", memoLine);
	for (i = 1; i < 128; ++i) {
		if (!pmd_get_memo_address (i, &memoLine))
			break;

		switch (i) {
			case 1:
				printf ("Title:\t\t");
				break;

			case 2:
				printf ("Composer:\t");
				break;

			case 3:
				printf ("Arranger:\t");
				break;

			case 4:
				printf ("Memo:");
				/* fallthrough */

			default:
				printf ("\t\t");
		}
		printf ("%Fs\n", memoLine);
	}
	printf ("\n");

	return 0;
}
