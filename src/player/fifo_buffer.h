/**
 * @file
 * Fifo buffer interface.
 *
 * @author Piotr Krzywicki <krzywicki.ptr@gmail.com>
 * @date 12.06.2018
 */

#ifndef __FIFO_BUFFER_H__
#define __FIFO_BUFFER_H__

#include <avr/io.h>
#include "stdbool.h"
#include "../lib/fat-fs/ff.h"

/**
 * @brief Default fifo buffer size, set to @p 256 because of performance reasons.
 */
#define FIFO_BUFFER_SIZE ((uint16_t) 256)

/**
 * @brief Structure holding internals of FifoBuffer
 * Exposed only because of performance reasons.
 */
struct FifoBuffer {
    uint8_t currentReadPosition; ///< Reading position, cyclic.
    uint8_t currentWritePosition; ///< Writing position, cyclic.
    uint8_t *buffer; ///< Actual storage.
};

/**
 * @brief Initialize @ref FifoBuffer.
 * @return Pointer to newly created fifo buffer.
 */
struct FifoBuffer *bufferInit();

/**
 * @brief Destroy @ref FifoBuffer.
 * @param[out] buffer : Pointer to @ref FifoBuffer, which will be destroyed.
 */
void bufferDestroy(struct FifoBuffer *buffer);

/**
 * @brief Refill free slots, by bytes from @p file.
 * @param[out] file : Source file.
 * @param[out] buffer : Pointer to buffer, we want to refill.
 */
void fileRefillBuffer(FIL *file, struct FifoBuffer *buffer);

/**
 * @brief Add value to buffer.
 * @param[in] buffer : Pointer to buffer.
 * @param[in] data : data, we want to add.
 */
void bufferAdd(struct FifoBuffer *buffer, uint8_t data);

/**
 * @brief Check if buffer is empty.
 * @param[in] buffer : Examined buffer.
 * @return @p true if is empty, @p false otherwise.
 */
bool bufferIsEmpty(struct FifoBuffer *buffer);

/**
 * @brief Get top element.
 * @param[in] buffer : Pointer to buffer.
 * @return Top element from @p buffer.
 */
uint8_t bufferPeek(struct FifoBuffer *buffer);

/**
 * @brief Get and remove top element.
 * @param[in] buffer : Pointer to buffer.
 * @return Top element from @p buffer.
 */
uint8_t bufferPool(struct FifoBuffer *buffer);


/**
 * @brief Get buffer size.
 * @param[in] buffer : Pointer to buffer.
 * @return Number of elements stored in @p buffer.
 */
uint8_t bufferCurrentSize(struct FifoBuffer *buffer);

/**
 * @brief Get maximum size of a buffer.
 * @param[in] buffer : Pointer to buffer.
 * @return Maximum size of @p buffer.
 */
uint8_t bufferMaxSize(struct FifoBuffer *buffer);

#endif /* __FIFO_BUFFER_H__ */
