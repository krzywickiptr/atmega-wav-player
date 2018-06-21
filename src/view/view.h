/**
 * @file
 * Screen view interface.
 *
 * @author Piotr Krzywicki <krzywicki.ptr@gmail.com>
 * @date 12.06.2018
 */

#ifndef __VIEW_H__
#define __VIEW_H__

#include <stdbool.h>

/**
 * @brief Filesystem root path symbol.
 */
#define ROOT_PATH "/"

/**
 * @brief Screen view state holding structure.
 */
struct View;

/**
 * @brief Initialize view structure.
 * @param[in] initialPath : Songs listing directory path.
 * @return Pointer to new @ref View.
 */
struct View *viewInit(const char *initialPath);

/**
 * @brief View structure destructor.
 * @param[out] view : Pointer to view, which will be destroyed.
 */
void viewDestroy(struct View *view);

/**
 * @brief Switch view to show element up.
 * @param[out] view : Pointer to a view structure.
 */
void viewPositionUp(struct View *view);

/**
 * @brief Switch view to show element up.
 * @param[out] view : Pointer to a view structure.
 */
void viewPositionDown(struct View *view);

/**
 * @brief Show all songs in current directory.
 * @param[out] view : Pointer to a view structure.
 */
void viewAvailableSongs(struct View *view);

/**
 * @brief Switch to play view.
 * @param[out] view : Pointer to a view structure.
 */
void viewPlaying(struct View *view);

/**
 * @brief Switch to pause view.
 * @param[out] view : Pointer to a view structure.
 */
void viewPaused(struct View *view);

/**
 * @brief Switch to stopped view.
 * @param[out] view : Pointer to a view structure.
 */
void viewStopped(struct View *view);

/**
 * @brief Get selected file path.
 * @param[in] view : Pointer to a view structure.
 * @return Created path string, must be freed.
 */
char *viewGetCurrentPath(struct View *view);

/**
 * @brief Check if selected file is a directory.
 * @param[in] view : Pointer to a view structure.
 * @return @p true if selected file a directory, @p false otherwise.
 */
bool viewIsCurrentDir(struct View *view);

/**
 * @brief Switch to selected directory.
 * @param[out] view : Pointer to a view structure.
 */
void viewEnterDirectory(struct View *view);

#endif /* __VIEW_H__ */
