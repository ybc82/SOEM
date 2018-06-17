#ifndef CONTROL_H__
#define CONTROL_H__

#include <math.h>
#include "ethercattype.h"

struct TorqueOut {
    uint16 torque;
    uint16 status;
};
struct TorqueIn {
    int32 position;
    int16 torque;
    uint16 status;
    int8 profile;
};

struct Controller{
	int16 id;
	// empty for now
};

// Initialize, if necessary
void controller_init();
// {
// 	// TODO
// }

// Control loop
void controller_loop(const struct Controller* controller, const struct TorqueIn* t_in, struct TorqueOut* t_out);

#endif // CONTROL_H__