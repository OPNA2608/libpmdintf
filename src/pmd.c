#include "pmd.h"

#include <stdio.h>
#include <string.h>

#include <i86.h>

#define PMD_VECTOR 0x60

#define PMD_MIN(x, y) (y < x) ? y : x

#if PMD_SAVE_RESIDENCE
int pmdIsResident = -1;
#endif

bool pmdIsResident (void) {
	char far * far * residenceIndicatorStart;
	char far* residenceIndicator;
#if !PMD_SAVE_RESIDENCE
	bool pmdIsResident;
#endif

#if PMD_SAVE_RESIDENCE
	if (pmdIsResident != -1) {
#	if PMD_VERBOSE
		fprintf (stderr, "PMD residence state already determined, reusing: %s\n", pmdIsResident ? "yes" : "no");
#	endif
		return pmdIsResident;
	}
#endif

#if PMD_VERBOSE
	fprintf (stderr, "Determining PMD residence state...\n");
#endif

	/* i hate far pointers... */
	residenceIndicatorStart = (char far * far*) MK_FP (0x0000, PMD_VECTOR * 4);
#if PMD_VERBOSE
	fprintf (stderr, "%04u:%04u", FP_SEG (residenceIndicatorStart), FP_OFF (residenceIndicatorStart));
#endif

	residenceIndicator = *(residenceIndicatorStart) + 2;
#if PMD_VERBOSE
	fprintf (stderr, ", %04u:%04u", FP_SEG (residenceIndicator), FP_OFF (residenceIndicator));
#endif

	pmdIsResident = residenceIndicator[0] == 'P' && residenceIndicator[1] == 'M' && residenceIndicator[2] == 'D';
#if PMD_VERBOSE
	fprintf (stderr, ": %c%c%c\n", residenceIndicator[0], residenceIndicator[1], residenceIndicator[2]);
#endif

#if PMD_VERBOSE
	fprintf (stderr, "PMD residence state: %s\n", pmdIsResident ? "yes" : "no");
#endif

	return pmdIsResident;
}

bool pmdCallFunc (
	enum pmdFuncId funcId,
	bool haveAlIn,
	uint8_t* alIn,
	bool haveAhOut,
	uint8_t* ahOut,
	bool haveAlOut,
	uint8_t* alOut,
	bool haveAddressOut,
	void far** addressOut
) {
	union REGS regsIn, regsOut;
	struct SREGS segregsOut;

	if (!pmdIsResident()) {
		return false;
	}

	regsIn.h.ah = funcId;

	if (haveAlIn) {
		regsIn.h.al = *alIn;
	}

#if PMD_VERBOSE
	fprintf (
		stderr,
		"INT %02Xh, AH=%02Xh", //
		PMD_VECTOR,
		regsIn.h.ah
	);

	if (haveAlIn) {
		fprintf (stderr, ", AL=%02Xh", regsIn.h.al);
	}

	fprintf (stderr, "\n");
#endif

	int86x (PMD_VECTOR, &regsIn, &regsOut, &segregsOut);

	if (haveAhOut) {
#if PMD_VERBOSE
		fprintf (stderr, "-> AH=%02Xh\n", regsOut.h.ah);
#endif

		*ahOut = regsOut.h.ah;
	}

	if (haveAlOut) {
#if PMD_VERBOSE
		fprintf (stderr, "-> AL=%02Xh\n", regsOut.h.al);
#endif

		*alOut = regsOut.h.al;
	}

	if (haveAddressOut) {
#if PMD_VERBOSE
		fprintf (stderr, "-> %04X:%04X\n", segregsOut.ds, regsOut.x.dx);
#endif

		*addressOut = MK_FP (segregsOut.ds, regsOut.x.dx);
	}

	return true;
}

bool pmd_get_memo_address (uint8_t memoId, char far** addressOut) {
	char far* memoEntryAddr;

	if (!pmdCallFunc (
				PMD_FUNC_GET_MEMO_ADDR,
				true,
				&memoId,
				false,
				NULL,
				false,
				NULL,
				true,
				(void far**) &memoEntryAddr
			))
		return false;

	if (FP_SEG (memoEntryAddr) == 0x0000 && FP_OFF (memoEntryAddr) == 0x0000) {
		return false;
	}

	*addressOut = memoEntryAddr;
	return true;
}

bool pmdLoadFile (char* filePath) {
	int i;
	size_t fileCopyCount;
	size_t fileReadCount;
	size_t fileLengthMax;
	size_t filePathLength;
	char fileBuffer[256];
	char* fileName;
	char far* filenameAddr;
	char far* effectdataAddr;
	char far* musicdataAddr;
	FILE* fileHandle;

	if (!pmdIsResident()) {
		return false;
	}

	fileHandle = fopen (filePath, "rb");
	if (fileHandle == NULL) {
		return false;
	}

	/* 8.3 filename for playback metadata */
	filePathLength = strlen (filePath);
	for (i = filePathLength; filePathLength - i < (8 + 3 + 1); --i) {
		if (filePath[i - 1] == '\\') {
			--i;
			break;
		}
	}
	fileName = &filePath[++i];

	pmd_get_filename_address ((void far**) &filenameAddr);
	do {
		*filenameAddr = *fileName;
		++filenameAddr;
		++fileName;
	} while (*(fileName - 1) != '\0');

	/* area for music data allocated by driver and borders non-music data, must not over-write */
	pmd_get_effectdata_address ((void far**) &effectdataAddr);
	pmd_get_musicdata_address ((void far**) &musicdataAddr);
	fileLengthMax = effectdataAddr - musicdataAddr;

	while (true) {
		fileReadCount = fread (fileBuffer, sizeof (char), PMD_MIN (fileLengthMax, 256), fileHandle);

		if (fileReadCount == 0)
			break;

		for (fileCopyCount = 0; fileCopyCount < fileReadCount; ++fileCopyCount) {
			*musicdataAddr = fileBuffer[fileCopyCount];
			++musicdataAddr;
		}
		fileLengthMax -= fileReadCount;
	}

	fclose (fileHandle);
	return true;
}

#undef PMD_MIN
