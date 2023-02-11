/*
 * gatedrive.h
 *
 *  Created on: Mar 3, 2022
 *      Author: Alex Hattori
 */

#ifndef INC_GATEDRIVE_H_
#define INC_GATEDRIVE_H_

#include <stdint.h>
#include "tim.h"
#include "hw_config.h"
#include "structs.h"

void enable_gd(ControllerStruct *controller);
void disable_gd(ControllerStruct *controller);

#endif /* INC_GATEDRIVE_H_ */
