#include <stdint.h>
#include <stdio.h>

#include <conio.h>
#include <pmd.h>

#define TOOL_NAME "st1-chck"

void printUsage (char* name) {
	printf (
		"Usage: %s <SONG.M>\n"
		"\n"
		"Plays <SONG.M> (for 1 loop), prints information about Status1 writes.\n"
		"(no fixed meaning to values, but indicates something might be timed to the song)\n",
		name
	);
}

int main (int argc, char** argv) {
	uint8_t status1, prevStatus1;
	uint8_t status2, prevStatus2;

	if (argc <= 1) {
		printUsage ((argc == 1) ? argv[0] : TOOL_NAME);
		return 1;
	}

	if (!pmdIsResident()) {
		fprintf (stderr, "PMD *NOT* resident!\n");
		return 1;
	}

	if (argc > 1) {
		pmd_music_stop();
		pmdLoadFile (argv[1]);
		pmd_music_start();
	}

	status1 = 0x00;
	status2 = 0x00;

	fprintf (stderr, "(press any key to quit early)\n");

	while (true) {
		prevStatus1 = status1;
		prevStatus2 = status2;

		if (kbhit()) {
			fprintf (stderr, "key pressed, exiting.\n");
			break;
		}
		if (!pmd_get_status (&status1, &status2)) {
			fprintf (stderr, "Error while getting PMD status, exiting.\n");
			break;
		}
		if (status2 > prevStatus2) {
			fprintf (stderr, "Song looped or finished, exiting.\n");
			break;
		}
		if (status1 == prevStatus1) {
			continue;
		}

		printf ("Status1 write! 0x%02X\n", status1);
		fflush (stdout);
	}
	printf ("\n");

	pmd_music_stop();

	return 0;
}
