#include "control.h"
#include <stdio.h>
#include "Control_grt_rtw/Control.h"

/*
    The controller, called every cycle
    Input:
        - TorqueIn* t_in: feedback, position, torque, status, profile
    Output:
        - TorqueOut* t_out: torque, status
*/

void    controller_init(struct Controller* controller, int n_actuators, struct TorqueIn** t_in)
{
    int i = 0;
    controller->n_actuators = n_actuators;
    size_t n_actuators_sim = sizeof(Control_Y.Torque_Cmd)/sizeof(Control_Y.Torque_Cmd[0]);
    printf("actuators number: %d %d\n", controller->n_actuators, (int)n_actuators_sim);
    if (controller->n_actuators != n_actuators_sim)
    {
        printf("Error: %d actuators connected on EtherCAT, while %d actuators required in controller\n", n_actuators, (int)n_actuators_sim);
        controller->n_actuators = -1;
        // TODO: and halt controlling
    }
    printf("test point 1\n");
    Control_initialize();
    printf("test point 2\n");
    for (i = 1; i <= controller->n_actuators; i++)
    {
        Control_U.Position_Cmd[i-1] = t_in[i]->position;
    }
    printf("test point 3\n");
}

void    controller_loop(struct Controller* controller, struct TorqueIn** t_in, struct TorqueOut** t_out)
{
    int i = 0;
    double torque_cmd;
    if (controller->n_actuators > 0)
    {
        for (i = 1; i <= controller->n_actuators; i++)
        {
            Control_U.Position[i-1] = (real_T)t_in[i]->position;
        }
        Control_step();
        for (i = 1; i <= controller->n_actuators; i++)
        {
            torque_cmd  = Control_Y.Torque_Cmd[i-1];
            if (torque_cmd > 32767)
                torque_cmd = 32767;
            else if (torque_cmd < -32767)
                torque_cmd = -32767;
            t_out[i]->torque = (int16)(torque_cmd);
        }
        // printf("compare: %d %f\n", t_out[i]->torque, Control_Y.Torque_Cmd[i-1]);
        printf("1- p:%f, r:%f, t:%f, 2- p:%f, r:%f, t:%f\n", Control_U.Position[0], Control_U.Position_Cmd[0], Control_Y.Torque_Cmd[0], Control_U.Position[1], Control_U.Position_Cmd[1], Control_Y.Torque_Cmd[1]);
   }

}
