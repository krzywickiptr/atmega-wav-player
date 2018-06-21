/**
 * @file
 * Wav file representation implementation.
 *
 * @author Piotr Krzywicki <krzywicki.ptr@gmail.com>
 * @date 12.06.2018
 */

#include <avr/io.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "wav_file.h"
#include "../view/screen_utils.h"

/**
 * @brief Structure representing wav file properties.
 */
struct WavFileInfo {
    uint16_t numberOfChannels; ///< @p 1 - mono, or @p 2 - stereo, expecting mono.
    uint32_t sampleRate; ///< We expect @p 8khz
    uint16_t bitsPerSample; ///< We expect 8 bits per sample.
};

/**
 * @brief Representation of wav file properties and data.
 */
struct WavFile {
    struct WavFileInfo info; ///< properties
    FIL *file; ///< file, which we are representing.
};

/**
 * @brief File offset from zero in bytes of number of channels property.
 */
#define NUMBER_OF_CHANNELS_OFFSET 22

/**
 * @brief File offset from zero in bytes of sample rate property.
 */
#define SAMPLE_RATE_OFFSET 24

/**
 * @brief File offset from zero in bytes of bits per sample property.
 */
#define BITS_PER_SAMPLE_OFFSET 34

/**
 * @brief File offset from zero in bytes of raw data.
 */
#define WAV_FILE_DATA_OFFSET 44

struct WavFile *wavFileLoad(FIL *file) {
    struct WavFile *result = malloc(sizeof(struct WavFile));
    assert(result);
    size_t read;
    f_lseek(file, NUMBER_OF_CHANNELS_OFFSET);
    f_read(file, (uint8_t *) &result->info.numberOfChannels, sizeof(result->info.numberOfChannels), &read);
    f_lseek(file, SAMPLE_RATE_OFFSET);
    f_read(file, (uint8_t *) &result->info.sampleRate, sizeof(result->info.sampleRate), &read);
    f_lseek(file, BITS_PER_SAMPLE_OFFSET);
    f_read(file, (uint8_t *) &result->info.bitsPerSample, sizeof(result->info.bitsPerSample), &read);
    result->file = file;
    f_lseek(file, WAV_FILE_DATA_OFFSET);
    return result;
}

/**
 * @brief Print number to a display.
 * @param[in] number : number, which will be printed.
 */
static void printNumber(int32_t number) {
    char buffer[10] = {0};
    ltoa(number, buffer, 10);
    print(buffer);
    write('\n');
}

void wavFilePrint(struct WavFile *wavFile) {
    setTextColor(WHITE, RED);
    print("Channels: ");
    restoreColours();
    printNumber(wavFile->info.numberOfChannels);

    setTextColor(WHITE, RED);
    print("Sample rate: ");
    restoreColours();
    printNumber(wavFile->info.sampleRate);

    setTextColor(WHITE, RED);
    print("Bits per sample: ");
    restoreColours();
    printNumber(wavFile->info.bitsPerSample);
}

inline void wavFileDestroy(struct WavFile *wavFile) {
    if (wavFile != NULL) {
        f_close(wavFile->file);
        free(wavFile->file);
        free(wavFile);
    }
}

inline uint16_t wavFileNumberOfChannels(struct WavFile *wavFile) {
    return wavFile->info.numberOfChannels;
}

inline uint32_t wavFileSampleRate(struct WavFile *wavFile) {
    return wavFile->info.sampleRate;
}

inline uint16_t wavFileBitsPerSample(struct WavFile *wavFile) {
    return wavFile->info.bitsPerSample;
}

inline FIL *wavFileGetFile(struct WavFile *wavFile) {
    return wavFile->file;
}