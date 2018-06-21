/**
 * @file
 * Wav file representation interface.
 *
 * @author Piotr Krzywicki <krzywicki.ptr@gmail.com>
 * @date 12.06.2018
 */

#ifndef __WAV_FILE_H__
#define __WAV_FILE_H__

#include <avr/io.h>
#include <stdlib.h>
#include <assert.h>
#include "../lib/fat-fs/ff.h"

/**
 * @brief Representation of wav file properties and data.
 */
struct WavFile;

/**
 * @brief Initialize @ref WavFile.
 * @param[in] file : Pointer to file, which will be loaded.
 * @return Pointer to a newly created wav file.
 */
struct WavFile *wavFileLoad(FIL *file);

/**
 * @brief Destroy @ref WavFile.
 * @param[out] wavFile : Pointer to structure, which will be destroyed.
 */
void wavFileDestroy(struct WavFile *wavFile);

/**
 * @brief Get number of channels.
 * @param[in] wavFile : Pointer to wav file.
 * @return Number of channels from wav file properties.
 */
uint16_t wavFileNumberOfChannels(struct WavFile *wavFile);

/**
 * @brief Get sample rate.
 * @param[in] wavFile : Pointer to wav file.
 * @return Sample rate from wav file properties.
 */
uint32_t wavFileSampleRate(struct WavFile *wavFile);

/**
 * @brief Get bits per sample.
 * @param[in] wavFile : Pointer to wav file.
 * @return Bits per sample from wav file properties.
 */
uint16_t wavFileBitsPerSample(struct WavFile *wavFile);

/**
 * @brief Get raw data size.
 * @param[in] wavFile : Pointer to wav file.
 * @return Raw data size from wav file properties.
 */
uint32_t wavFileDataSize(struct WavFile *wavFile);

/**
 * @brief Get file represented by @p wavFile.
 * @param[in] wavFile : Pointer to wav file.
 * @return File represented by @p wavFile.
 */
FIL* wavFileGetFile(struct WavFile *wavFile);

/**
 * @brief Print wav file properties.
 * @param[in] wavFile : Pointer to wav file.
 */
void wavFilePrint(struct WavFile *wavFile);

#endif /* __WAV_FILE_H__ */
