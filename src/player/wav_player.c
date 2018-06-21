/**
 * @file
 * Wav player implementation.
 *
 * @author Piotr Krzywicki <krzywicki.ptr@gmail.com>
 * @date 12.06.2018
 */

#include <stdlib.h>
#include <avr/interrupt.h>
#include "wav_player.h"
#include "wav_file.h"
#include "fifo_buffer.h"

/**
 * @brief Wav player state structure.
 */
struct WavPlayer {
    struct WavFile *wavFile; ///< Loaded wav file.
    struct FifoBuffer *buffer; ///< Internal buffer, used by loading and playing interrupts.
    struct View *view; ///< Display view.
    bool paused; ///< Flag indicating if is paused.
};

/**
 * @brief Pointer to currently playing wav player - shared state.
 */
static struct WavPlayer *currentlyPlaying;

/**
 * @brief Field @ref WavPlayer.buffer of @ref currentlyPlaying, exists because of performance reasons.
 */
static struct FifoBuffer *currentlyPlayingFifoBuffer;

/**
 * @brief Field @ref FifoBuffer.buffer of @ref currentlyPlayingFifoBuffer, exists because of performance reasons.
 */
static uint8_t *currentlyPlayingBuffer;

/**
 * @brief DAC output port.
 */
#define OUTPUT_PORT PORTD

/**
 * @brief DAC output interrupt.
 */
ISR(TIMER1_COMPA_vect) {
    OUTPUT_PORT = currentlyPlayingBuffer[currentlyPlayingFifoBuffer->currentReadPosition++];
}

/**
 * @brief File loading interrupt.
 */
ISR(TIMER0_COMP_vect, ISR_BLOCK) {
    if (bufferCurrentSize(currentlyPlaying->buffer) <= FIFO_BUFFER_SIZE / 2) {
        fileRefillBuffer(wavFileGetFile(currentlyPlaying->wavFile), currentlyPlayingFifoBuffer);
        if (f_eof(wavFileGetFile(currentlyPlaying->wavFile))) {
            viewStopped(currentlyPlaying->view);
            wavPlayerStopPlaying();
        }
    }
}

struct WavPlayer *wavPlayerInit(FIL *file, struct View *view) {
    struct WavPlayer *result = malloc(sizeof(struct WavPlayer));
    result->wavFile = wavFileLoad(file);
    result->buffer = bufferInit();
    result->view = view;
    result->paused = false;
    return result;
}

void wavPlayerDestroy(struct WavPlayer *player) {
    if (player != NULL) {
        wavFileDestroy(player->wavFile);
        bufferDestroy(player->buffer);
        free(player);
    }
}

void wavPlayerPausePlaying() {
    cli();
    if (currentlyPlaying) {
        currentlyPlaying->paused = true;
    }
}

void wavPlayerStopPlaying() {
    wavPlayerPausePlaying();

    TCCR1B = 0;
    TCCR0 = 0;
    TIMSK = 0;

    wavPlayerDestroy(currentlyPlaying);
    currentlyPlaying = NULL;
    currentlyPlayingBuffer = NULL;
    currentlyPlayingFifoBuffer = NULL;
}

void wavPlayerStartPlaying(struct WavPlayer *player) {
    wavPlayerPausePlaying();

    currentlyPlaying = player;
    currentlyPlayingFifoBuffer = player->buffer;
    currentlyPlayingBuffer = player->buffer->buffer;

    // Configure playing timer
    TCCR1B = 1 << CS10 | 1 << WGM12; // NOLINT
    OCR1A = (uint16_t) (F_CPU / wavFileSampleRate(player->wavFile) - 1);
    TIMSK |= 1 << OCIE1A; // NOLINT

    // Configure player file reading timer
    TCCR0 = 1 << CS00 | 1 << CS01 | 1 << WGM01; // NOLINT
    OCR0 = (uint8_t) ((F_CPU / wavFileSampleRate(player->wavFile) - 1) / 5);
    TIMSK |= 1 << OCIE0; // NOLINT

    sei();
    player->paused = false;
}

bool wavPlayerIsPlaying() {
    return currentlyPlaying != NULL && !currentlyPlaying->paused;
}

struct WavPlayer *wavPlayerGetCurrentlyPlaying() {
    return currentlyPlaying;
}

struct WavFile *wavPlayerGetWavFile(const struct WavPlayer *player) {
    return player->wavFile;
}