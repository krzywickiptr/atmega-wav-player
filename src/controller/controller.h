/**
 * @file
 * Controller module interface.
 *
 * @author Piotr Krzywicki <krzywicki.ptr@gmail.com>
 * @date 12.06.2018
 */


#ifndef _CONTROLLER_H__
#define _CONTROLLER_H__

#include "../view/view.h"

/**
 * @brief Structure representing current controller state.
 */
struct Controller;

/**
 * @brief Initialize @ref Controller.
 * @param[in] view : Pointer to view, which will be used.
 * @return Pointer to newly created controller.
 */
struct Controller *controllerInit(struct View *view);

/**
 * @brief Destroy @ref Controller.
 * @param[out] controller : Pointer to structure, which will be  destroyed.
 */
void controllerDestroy(struct Controller *controller);

/**
 * @brief Active loop for events.
 * @param[in] controller : Pointer to controller structure.
 */
void run(struct Controller *controller);

#endif /* _CONTROLLER_H__ */
