/**
 * @file
 * Wav player interface.
 *
 * @author Piotr Krzywicki <krzywicki.ptr@gmail.com>
 * @date 12.06.2018
 */

#ifndef __WAV_PLAYER_H__
#define __WAV_PLAYER_H__

#include <stdbool.h>
#include "../view/view.h"
#include "../lib/fat-fs/ff.h"

/**
 * @brief Wav player state structure.
 */
struct WavPlayer;

/**
 * @brief Initialize @ref WavPlayer.
 * @param[in] file : Pointer to file to be played.
 * @param[in] view : Pointer to a screen managing view.
 * @return Pointer to newly created @ref WavPlayer.
 */
struct WavPlayer *wavPlayerInit(FIL *file, struct View *view);

/**
 * @brief Destroy @ref WavPlayer.
 * @param[out] player : Pointer to a wav player to destroy.
 */
void wavPlayerDestroy(struct WavPlayer *player);

/**
 * @brief Start playing loaded song.
 * @param[out] player : Pointer to a wav player structure.
 */
void wavPlayerStartPlaying(struct WavPlayer *player);

/**
 * @brief Stop currently playing wav player and remove it.
 */
void wavPlayerStopPlaying();

/**
 * @brief Pause currently running wav player.
 */
void wavPlayerPausePlaying();

/**
 * @brief Check if any wav player is running.
 * @return @p true if it is, @p false otherwise.
 */
bool wavPlayerIsPlaying();

/**
 * @brief Get wav player loaded wav file.
 * @param[in] player : Pointer to wav player structure.
 * @return Pointer to loaded wav file.
 */
struct WavFile *wavPlayerGetWavFile(const struct WavPlayer *player);

/**
 * @brief Get currently playing wav player, even if paused.
 * @return Pointer to currently playing wav player.
 */
struct WavPlayer *wavPlayerGetCurrentlyPlaying();

#endif /* __WAV_PLAYER_H__ */
