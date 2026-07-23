/*
 * Based on the source code for PMD and its tools.
 * Original license is not formal, so please assume Unfree
 */

#ifndef LIBPMDINTF_PMD_H
#define LIBPMDINTF_PMD_H

/*
 * Print details during execution.
 */
#ifndef PMD_VERBOSE
#	define PMD_VERBOSE 0
#endif

/*
 * Save residence state of PMD between calls. Otherwise, residence gets re-checked at the start for every relevant
 * function (PMD might get loaded/unloaded during execution).
 */
#ifndef PMD_SAVE_RESIDENCE
#	define PMD_SAVE_RESIDENCE 0
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* TODO: Add the rest of the functions (enum + interrupt wrapper)
 * Section labeled "INT 60H 仕様" in PMD.ASM documents all the functions, their arguments, and their outputs.
 */
enum pmdFuncId {
	PMD_FUNC_MUSIC_START = 0x00,
	PMD_FUNC_MUSIC_STOP = 0x01,
	PMD_FUNC_FADE_INOUT = 0x02,
	PMD_FUNC_EFFECT_ON = 0x03,
	PMD_FUNC_EFFECT_OFF = 0x04,
	PMD_FUNC_GET_SYOUSETU = 0x05,
	PMD_FUNC_GET_MUSICDATA_ADDR = 0x06,
	PMD_FUNC_GET_TONEDATA_ADDR = 0x07,
	PMD_FUNC_GET_FOUT_VOL = 0x08,
	PMD_FUNC_GET_STATUS = 0x0A,
	PMD_FUNC_GET_EFFECTDATA_ADDR = 0x0B,
	PMD_FUNC_GET_PCM_ADDR = 0x0E,
	PMD_FUNC_GET_WORK_ADDR = 0x0E,
	PMD_FUNC_GET_MEMO_ADDR = 0x1D,
	PMD_FUNC_GET_FILENAME_ADDR = 0x21,
	PMD_FUNC_GET_SIZE = 0x22
};

bool pmdIsResident (void);
bool pmdLoadFile (char* filePath);

bool pmdCallFunc (
	enum pmdFuncId funcId,
	/* side-by-side for readability */
	/* clang-format off */
	bool haveAlIn, uint8_t* alIn,
	bool haveAhOut, uint8_t* ahOut,
	bool haveAlOut, uint8_t* alOut,
	bool haveAddressOut, void far** addressOut
	/* clang-format on */
);

/* side-by-side for readability */
/* clang-format off */
#	define pmd_music_start() \
	pmdCallFunc ( \
		PMD_FUNC_MUSIC_START, \
		false, NULL, \
		false, NULL, \
		false, NULL, \
		false, NULL \
	)

#	define pmd_music_stop() \
	pmdCallFunc ( \
		PMD_FUNC_MUSIC_STOP, \
		false, NULL, \
		false, NULL, \
		false, NULL, \
		false, NULL \
	)

#	define pmd_fade_inout(speed) \
	pmdCallFunc ( \
		PMD_FUNC_FADE_INOUT, \
		true, speed, \
		false, NULL, \
		false, NULL, \
		false, NULL \
	)

#	define pmd_effect_on(effectNum) \
	pmdCallFunc ( \
		PMD_FUNC_EFFECT_ON, \
		true, effectNum, \
		false, NULL, \
		false, NULL, \
		false, NULL \
	)

#	define pmd_effect_off() \
	pmdCallFunc ( \
		PMD_FUNC_EFFECT_OFF, \
		false, NULL, \
		false, NULL, \
		false, NULL, \
		false, NULL \
	)

#	define pmd_get_syousetu(ahOut, alOut) \
	pmdCallFunc ( \
		PMD_FUNC_EFFECT_OFF, \
		false, NULL, \
		true, ahOut, \
		true, alOut, \
		false, NULL \
	)

#	define pmd_get_musicdata_address(addressOut) \
	pmdCallFunc ( \
		PMD_FUNC_GET_MUSICDATA_ADDR, \
		false, NULL, \
		false, NULL, \
		false, NULL, \
		true, addressOut \
	)

#	define pmd_get_tonedata_address(addressOut) \
	pmdCallFunc ( \
		PMD_FUNC_GET_TONEDATA_ADDR, \
		false, NULL, \
		false, NULL, \
		false, NULL, \
		true, addressOut \
	)

#	define pmd_get_fout_volume(foutVolumeOut) \
	pmdCallFunc ( \
		PMD_FUNC_GET_FOUT_VOL, \
		false, NULL, \
		false, NULL, \
		true, foutVolumeOut, \
		false, NULL \
	)

#	define pmd_get_status(status1Out, status2Out) \
	pmdCallFunc ( \
		PMD_FUNC_GET_STATUS, \
		false, NULL, \
		true, status1Out, \
		true, status2Out, \
		false, NULL \
	)

#	define pmd_get_effectdata_address(addressOut) \
	pmdCallFunc ( \
		PMD_FUNC_GET_EFFECTDATA_ADDR, \
		false, NULL, \
		false, NULL, \
		false, NULL, \
		true, addressOut \
	)

#	define pmd_get_pcm_address(addressOut) \
	pmdCallFunc ( \
		PMD_FUNC_GET_PCM_ADDR, \
		false, NULL, \
		false, NULL, \
		false, NULL, \
		true, addressOut \
	)

#	define pmd_get_work_address(addressOut) \
	pmdCallFunc ( \
		PMD_FUNC_GET_WORK_ADDR, \
		false, NULL, \
		false, NULL, \
		false, NULL, \
		true, addressOut \
	)

#	define pmd_get_filename_address(addressOut) \
	pmdCallFunc ( \
		PMD_FUNC_GET_FILENAME_ADDR, \
		false, NULL, \
		false, NULL, \
		false, NULL, \
		true, addressOut \
	)
/* clang-format on */

bool pmd_get_memo_address (uint8_t memoId, char far** addressOut);

#endif /* LIBPMDINTF_PMD_H */
