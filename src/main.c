/**
 * @file
 * Entrypoint of wav file player.
 *
 * @author Piotr Krzywicki <krzywicki.ptr@gmail.com>
 * @date 12.06.2018
 */

#include <stdio.h>
#include <avr/pgmspace.h>
#include "view/view.h"
#include "controller/controller.h"
#include "lib/fat-fs/ff.h"

/**
 * @brief Initialize device, start active waiting by controller.
 */
int main() {
    DDRB = 0xff; // All B pins to output mode.
    DDRD = 0xff; // All D pins (DAC) to output mode.

    FATFS FatFs;
    f_mount(&FatFs, "", 0);

    struct View *view = viewInit(ROOT_PATH);
    viewAvailableSongs(view);

    struct Controller *controller = controllerInit(view);
    run(controller);

    viewDestroy(view);
    controllerDestroy(controller);
    f_unmount("");
}
