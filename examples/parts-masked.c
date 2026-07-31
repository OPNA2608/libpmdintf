#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <conio.h>
#include <pmd.h>

#define TOOL_NAME "partmask"

void printUsage (char* name) {
	printf (
		"Usage: %s <SONG.M> [ch1 [ch2 [... [chN]]]]\n"
		"\n"
		"Plays <SONG.M> (main loop thrice, or until end), with all channels muted.\n"
		"If any channel names are specified after <SONG.M>, then those will be unmuted.\n",
		name
	);
}

char* partLabels[PMD_PART_END] = { "A",
	"B",
	"C",
	"D",
	"E",
	"F",
	"G",
	"H",
	"I",
	"J",
	"K",
	"C2",
	"C3",
	"C4",
	"Rhythm",
	"Effect",
	"PPZ1",
	"PPZ2",
	"PPZ3",
	"PPZ4",
	"PPZ5",
	"PPZ6",
	"PPZ7",
	"PPZ8" };

int main (int argc, char** argv) {
	enum pmdPartId partToAffect = PMD_PART_END;
	int argNumber;
	bool partKnown;
	uint8_t partValueForCall;
	uint8_t status1;
	uint8_t status2;

	if (argc <= 1) {
		printUsage ((argc == 1) ? argv[0] : TOOL_NAME);
		return 1;
	}

	if (!pmdIsResident()) {
		fprintf (stderr, "PMD *NOT* resident!\n");
		return 1;
	}

	pmd_music_stop();
	pmdLoadFile (argv[1]);

	printf ("[default] Muting part: ");
	for (partToAffect = PMD_PART_A; partToAffect < PMD_PART_END; ++partToAffect) {
		printf ("%s%s", (partToAffect != PMD_PART_A) ? ", " : "", partLabels[partToAffect]);
		partValueForCall = partToAffect;
		pmd_set_part_mask (&partValueForCall);
	}
	printf ("\n");

	if (argc > 2) {
		for (argNumber = 2; argNumber < argc; ++argNumber) {
			partKnown = false;

			for (partToAffect = PMD_PART_A; partToAffect < PMD_PART_END; ++partToAffect) {
				if (strcmp (argv[argNumber], partLabels[partToAffect]) == 0) {
					partKnown = true;
					printf ("enabling part: %s\n", partLabels[partToAffect]);
					partValueForCall = 0x80 | partToAffect;
					pmd_set_part_mask (&partValueForCall);
					break;
				}
			}

			if (!partKnown) {
				printf ("Unknown part name, ignoring: %s\n", argv[argNumber]);
			}
		}
	}

	pmd_music_start();

	fprintf (stderr, "(press any key to quit early)\n");

	while (true) {
		if (kbhit()) {
			fprintf (stderr, "Key pressed, exiting.\n");
			break;
		}
		if (!pmd_get_status (&status1, &status2)) {
			fprintf (stderr, "Error while getting PMD status, exiting.\n");
			break;
		}
		if (status2 > 0x02) {
			fprintf (stderr, "Song looped thrice or finished, exiting.\n");
			break;
		}
	}
	printf ("\n");

	pmd_music_stop();

	for (partToAffect = PMD_PART_A; partToAffect < PMD_PART_END; ++partToAffect) {
		partValueForCall = 0x80 | partToAffect;
		pmd_set_part_mask (&partValueForCall);
	}

	return 0;
}
