#ifndef CONTROL_H__
#define CONTROL_H__

#include <math.h>
#include "ethercattype.h"

struct TorqueOut {
    int16 torque;
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
	int16 n_actuators; 	// number of actuators
	// empty for now
};

// Initialize, if necessary
void controller_init(struct Controller* controller, int n_actuators, struct TorqueIn** t_in);

// Control loop
void controller_loop(struct Controller* controller, struct TorqueIn** t_in, struct TorqueOut** t_out);

#endif // CONTROL_H__