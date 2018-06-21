/**
 * @file
 * Screen view implementation.
 *
 * @author Piotr Krzywicki <krzywicki.ptr@gmail.com>
 * @date 12.06.2018
 */

#include <stddef.h>
#include <avr/io.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "view.h"
#include "screen_utils.h"
#include "../lib/fat-fs/ff.h"
#include "../player/wav_player.h"
#include "../player/wav_file.h"
#include <inttypes.h>

/**
 * @brief Maximum length of file/directory path.
 */
#define MAXIMUM_PATH_LENGTH 32

/**
 * @brief Directory separator symbol, used when displaying and with Fatfs library.
 */
#define DIRECTORY_SEPARATOR "/"

/**
 * @brief Parent directory symbol, used when displaying and with Fatfs library.
 */
#define PARENT_DIRECTORY ".."

/**
 * @brief Screen view state holding structure.
 */
struct View {
    char currentPath[MAXIMUM_PATH_LENGTH]; ///< String containing current path, without current selection.
    FILINFO current; ///< Currently selected file.
    size_t position; ///< Index of current selection.
};

struct View *viewInit(const char *const initialPath) {
    init();
    clearScreen();

    struct View *view = calloc(1, sizeof(struct View));
    view->position = 1;
    strcpy(view->currentPath, initialPath);
    return view;
}

void viewDestroy(struct View *view) {
    free(view);
}

void viewPositionUp(struct View *const view) {
    view->position++;
    viewAvailableSongs(view);
}

void viewPositionDown(struct View *const view) {
    view->position--;
    viewAvailableSongs(view);
}

/**
 * @brief Show entry on display in proper way.
 * @param[in] view : Pointer to view structure.
 * @param[in] fileInfo : Pointer to entry, which has to be displayed.
 * @param[in] read : Number of already read entries.
 */
static void viewProcessEntry(struct View *const view, FILINFO *fileInfo, size_t read) {
    if (read == view->position) {
        setTextColor(WHITE, RED); // NOLINT
        view->current = *fileInfo;
        if (view->current.fattrib & AM_DIR) { // NOLINT
            setTextColor(WHITE, VIOLET); // NOLINT
        }
    }

    print(view->currentPath);
    if (strcmp(view->currentPath, ROOT_PATH) != 0) {
        print(ROOT_PATH);
    }

    print(fileInfo->fname);
    write('\n');
    restoreColours();
}

/**
 * @brief Show parent directory on display if necessary.
 * @param[in] view : Pointer to view structure.
 */
static void prependParentDirectory(const struct View *view) {
    if (strcmp(view->currentPath, ROOT_PATH) != 0) {
        if (view->position == 0) {
            setTextColor(WHITE, RED); // NOLINT
        }
        print(PARENT_DIRECTORY);
        write('\n');
        restoreColours();
    }
}

void viewAvailableSongs(struct View *view) {
    DIR directory;
    FILINFO fileInfo;

    clearScreen();

    prependParentDirectory(view);

    size_t read = 1;
    FRESULT openResult = f_opendir(&directory, view->currentPath);
    if (openResult == FR_OK) {
        while (f_readdir(&directory, &fileInfo) == FR_OK && (&fileInfo)->fname[0]) {
            viewProcessEntry(view, &fileInfo, read);
            read++;
        }
        f_closedir(&directory);
    }
}

/**
 * @brief Change directory based on a selected one.
 * @param[in] view : Pointer to view structure.
 */
static void viewUpdateCurrentPath(struct View *view) {
    if (view->position != 0) {
        if (strlen(view->currentPath) > strlen(ROOT_PATH)) {
            strcat(view->currentPath, DIRECTORY_SEPARATOR);
        }
        strcat(view->currentPath, view->current.fname);
    }
    else {
        *strrchr(view->currentPath, DIRECTORY_SEPARATOR[0]) = 0;
        if (!strlen(view->currentPath)) {
            strcpy(view->currentPath, ROOT_PATH);
        }
    }
}

void viewEnterDirectory(struct View *const view) {
    viewUpdateCurrentPath(view);
    view->position = 1;
    viewAvailableSongs(view);
}

bool viewIsCurrentDir(struct View *const view) {
    return !view->position || view->current.fattrib & AM_DIR; // NOLINT
}

char *viewGetCurrentPath(struct View *const view) {
    char *result = malloc(strlen(view->current.fname) + strlen(view->currentPath) + 1);
    strcpy(result, view->currentPath);
    if (strlen(result) > strlen(ROOT_PATH)) {
        strcat(result, DIRECTORY_SEPARATOR);
    }
    strcat(result, view->current.fname);
    return result;
}

/**
 * @brief Display current wav file with label.
 * @param[in] view : Pointer to a view structure.
 * @param[in] label : Label printed before showing current wav file.
 */
static void displayCurrent(struct View *const view, const char *const label) {
    clearScreen();
    setTextColor(WHITE, RED);
    print(label);
    restoreColours();

    char *current = viewGetCurrentPath(view);
    print(current);
    write('\n');
    free(current);

    struct WavFile *currentlyPlayingFile = wavPlayerGetWavFile(wavPlayerGetCurrentlyPlaying());
    if (currentlyPlayingFile != NULL) {
        wavFilePrint(currentlyPlayingFile);
    }
}

void viewPlaying(struct View *view) {
    displayCurrent(view, "Playing:\n");
}

void viewPaused(struct View *view) {
    displayCurrent(view, "Paused:\n");
}

void viewStopped(struct View *view) {
    displayCurrent(view, "Stopped:\n");
}
