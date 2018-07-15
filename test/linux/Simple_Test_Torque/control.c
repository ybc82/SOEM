#include "control.h"

/*
    The controller, called every cycle
    Input:
        - TorqueIn* t_in: feedback, position, torque, status, profile
    Output:
        - TorqueOut* t_out: torque, status
*/
void    controller_loop(const struct Controller* controller, struct TorqueIn** t_in, struct TorqueOut** t_out)
{
    static int i = 0;
    int16 torque = t_in[1]->torque;
    i ++;
    t_out[1]->torque = (int16) (sin(i/100.)*(200));
}