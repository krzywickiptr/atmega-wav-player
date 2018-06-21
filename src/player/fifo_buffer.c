/**
 * @file
 * Fifo buffer implementation.
 *
 * @author Piotr Krzywicki <krzywicki.ptr@gmail.com>
 * @date 12.06.2018
 */

#include <stddef.h>
#include <avr/io.h>
#include <stdlib.h>
#include <stdbool.h>
#include "fifo_buffer.h"

struct FifoBuffer *bufferInit() {
    struct FifoBuffer *result = malloc(sizeof(struct FifoBuffer));
    uint8_t *buffer = malloc(FIFO_BUFFER_SIZE);
    result->currentReadPosition = result->currentWritePosition = 0;
    result->buffer = buffer;
    return result;
}

inline void bufferDestroy(struct FifoBuffer *buffer) {
    if (buffer != NULL) {
        free(buffer->buffer);
        free(buffer);
    }
}

/**
 * @brief Get min of two numbers.
 * @param[in] a : one number of the examined pair.
 * @param[in] b : one number of the examined pair.
 * @return min of @p and @p b.
 */
inline static uint16_t min(uint16_t a, uint16_t b) {
    return a > b ? b : a;
}

void fileRefillBuffer(FIL *file, struct FifoBuffer *buffer) {
    uint16_t freeSlots = FIFO_BUFFER_SIZE - bufferCurrentSize(buffer);
    size_t read = 0;
    uint16_t added = min(freeSlots, FIFO_BUFFER_SIZE - buffer->currentWritePosition);
    f_read(file, buffer->buffer + buffer->currentWritePosition, added, &read);
    buffer->currentWritePosition = (uint8_t) ((read + buffer->currentWritePosition));
    if (added < freeSlots) {
        f_read(file, buffer->buffer, freeSlots - added, &read);
        buffer->currentWritePosition = (uint8_t) read;
    }
}

inline void bufferAdd(struct FifoBuffer *buffer, uint8_t data) {
    buffer->buffer[buffer->currentWritePosition] = data;
    if (buffer->currentWritePosition == FIFO_BUFFER_SIZE - 1) {
        buffer->currentWritePosition = 0;
    }
    else {
        buffer->currentWritePosition++;
    }
}

inline uint8_t bufferPeek(struct FifoBuffer *buffer) {
    return buffer->buffer[buffer->currentReadPosition];
}

inline uint8_t bufferPool(struct FifoBuffer *buffer) {
    uint8_t value = bufferPeek(buffer);
    buffer->currentReadPosition++;
    return value;
}

inline bool bufferIsEmpty(struct FifoBuffer *buffer) {
    return buffer->currentWritePosition == buffer->currentReadPosition;
}

inline uint8_t bufferCurrentSize(struct FifoBuffer *buffer) {
    return (uint8_t) (buffer->currentWritePosition - buffer->currentReadPosition + FIFO_BUFFER_SIZE);
}