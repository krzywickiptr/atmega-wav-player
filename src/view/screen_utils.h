/**
 * @file
 * Screen utils and colour defines.
 *
 * @author Piotr Krzywicki <krzywicki.ptr@gmail.com>
 * @date 12.06.2018
 */

#ifndef __COLOURS_H__
#define __COLOURS_H__

#include "../lib/uTFT-ST7735/uTFT_ST7735.h"

//! @cond Doxygen_Suppress
#define BLACK (Color565(0U, 0U, 0U))
#define WHITE (Color565(255U, 255U, 255U))
#define BLUE (Color565(0U, 0U, 255U))
#define RED (Color565(255U, 0U, 0U))
#define VIOLET (Color565(102U, 0U, 102U))
#define GREEN (Color565(0U, 255U, 0U))
//! @endcond

/**
 * @brief Restore colour to defaults.
 */
inline void restoreColours() {
    setTextColor(WHITE, BLUE); // NOLINT
}

/**
 * @brief Clear screen to black, set cursor to (0, 0).
 */
inline void clearScreen() {
    fillScreen(BLACK); // NOLINT
    setCursor(0, 0);
    setTextWrap(1);
    restoreColours();
}

#endif /* __COLOURS_H__ */
