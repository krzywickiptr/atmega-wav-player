/**
 * @file
 * Controller module implementation.
 *
 * @author Piotr Krzywicki <krzywicki.ptr@gmail.com>
 * @date 12.06.2018
 */

#include <avr/io.h>
#include <stdlib.h>
#include <stdbool.h>
#include <util/delay.h>
#include <string.h>
#include "controller.h"
#include "../view/view.h"
#include "../player/wav_player.h"

/**
 * @brief Used to navigate, moving up.
 */
#define LEFT_BUTTON PA6

/**
 * @brief Used to trigger change of directory, or to start playing.
 */
#define MIDDLE_BUTTON PA5

/**
 * @brief Used to navigate, moving down.
 */
#define RIGHT_BUTTON PA4

/**
 * @brief Delay after button switch, to debounce it.
 */
#define DEFAULT_BUTTON_DELAY_MILLISECONDS 50

/**
 * @brief Available key types in device interface.
 */
enum KeyType {
    LEFT,
    MIDDLE,
    RIGHT,
    NONE,

    KEY_TYPE_LENGTH
};

/**
 * @brief Structure representing current controller state.
 */
struct Controller {
    struct View *view; ///< View used by controller.
    bool running; ///< Flag indicating if controller is running.
    void (*eventHandlers[KEY_TYPE_LENGTH])
            (struct Controller *const controller); ///< Handlers dispatch table
};

/**
 * @brief Handle left key pressed action - stop player and move position up.
 * @param[in] controller : Pointer to controller structure.
 */
static void leftKeyPressedHandler(struct Controller *const controller) {
    if (wavPlayerGetCurrentlyPlaying() != NULL) {
        wavPlayerStopPlaying();
    }
    viewPositionUp(controller->view);
    _delay_ms(DEFAULT_BUTTON_DELAY_MILLISECONDS);
}

/**
 * @brief Handle right key pressed action - stop player and move position down.
 * @param[in] controller : Pointer to controller structure.
 */
static void rightKeyPressedHandler(struct Controller *const controller) {
    if (wavPlayerIsPlaying()) {
        wavPlayerStopPlaying();
    }
    viewPositionDown(controller->view);
    _delay_ms(DEFAULT_BUTTON_DELAY_MILLISECONDS);
}

/**
 * @brief Start playing song from current position.
 * @param[in] controller : Pointer to controller structure.
 */
static void startPlayingNew(const struct Controller *controller) {
    FIL *file = malloc(sizeof(FIL));
    char *currentFilePath = viewGetCurrentPath(controller->view);
    if (f_open(file, currentFilePath, FA_READ) == FR_OK) {
        struct WavPlayer *wavPlayer = wavPlayerInit(file, controller->view);
        wavPlayerStartPlaying(wavPlayer);
    }
    viewPlaying(controller->view);
    free(currentFilePath);
}

/**
 * @brief Pause/Resume current wav player.
 * @param[in] controller : Pointer to controller structure.
 */
static void switchPlayingState(const struct Controller *controller) {
    if (wavPlayerIsPlaying()) { // Pause
        viewPaused(controller->view);
        wavPlayerPausePlaying();
    }
    else if (!wavPlayerIsPlaying() && wavPlayerGetCurrentlyPlaying() != NULL) { // Resume
        viewPlaying(controller->view);
        wavPlayerStartPlaying(wavPlayerGetCurrentlyPlaying());
    }
    else { // Start
        startPlayingNew(controller);
    }
}

/**
 * @brief Handle middle key pressed action - enter directory or switch playing state.
 * @param[in] controller : Pointer to controller structure.
 */
static void middleKeyPressedHandler(struct Controller *const controller) {
    if (viewIsCurrentDir(controller->view)) {
        viewEnterDirectory(controller->view);
    }
    else {
        switchPlayingState(controller);
    }
    _delay_ms(DEFAULT_BUTTON_DELAY_MILLISECONDS);
}

/**
 * @brief Handle unknown action - do nothing
 * Is not empty, because of unused parameter warning.
 * @param[in] controller : Pointer to controller structure.
 */
static void defaultEventHandler(struct Controller *const controller) {
    (void) controller;
}

struct Controller *controllerInit(struct View *view) {
    DDRA &= ~(1 << LEFT_BUTTON | 1 << MIDDLE_BUTTON | 1 << RIGHT_BUTTON); // NOLINT
    // Configuring pullups
    PORTA |= 1 << LEFT_BUTTON | 1 << MIDDLE_BUTTON | 1 << RIGHT_BUTTON; // NOLINT

    struct Controller *result = calloc(1, sizeof(struct Controller));
    result->view = view;
    result->eventHandlers[LEFT] = leftKeyPressedHandler;
    result->eventHandlers[RIGHT] = rightKeyPressedHandler;
    result->eventHandlers[MIDDLE] = middleKeyPressedHandler;
    result->eventHandlers[NONE] = defaultEventHandler;
    return result;
}

void controllerDestroy(struct Controller *controller) {
    free(controller);
}

/**
 * @brief Get type of currently pressed key.
 * @return Type of currently pressed key.
 */
static enum KeyType getKeyType() {
    if (!(PINA & 1 << LEFT_BUTTON)) { // NOLINT
        return LEFT;
    }
    if (!(PINA & 1 << MIDDLE_BUTTON)) { // NOLINT
        return MIDDLE;
    }
    if (!(PINA & 1 << RIGHT_BUTTON)) { // NOLINT
        return RIGHT;
    }
    return NONE;
}

void run(struct Controller *controller) {
    controller->running = true;
    while (controller->running) {
        controller->eventHandlers[getKeyType()](controller);
    }
}
