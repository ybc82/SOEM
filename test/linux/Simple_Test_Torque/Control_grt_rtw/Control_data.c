/*
 * Control_data.c
 *
 * Code generation for model "Control".
 *
 * Model version              : 1.47
 * Simulink Coder version : 8.11 (R2016b) 25-Aug-2016
 * C source code generated on : Sat Apr  6 17:21:46 2019
 *
 * Target selection: grt.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: Intel->x86-64 (Linux 64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "Control.h"
#include "Control_private.h"

/* Block parameters (auto storage) */
P_Control_T Control_P = {
  2500.0,                               /* Variable: MAX_TORQUE_OUTPUT
                                        * Referenced by:
                                        *   '<S2>/Saturate'
                                        *   '<S3>/Saturate'
                                        *   '<S4>/DeadZone'
                                        *   '<S5>/DeadZone'
                                        */
  0.01,                                /* Variable: PID_D
                                        * Referenced by:
                                        *   '<S2>/Derivative Gain'
                                        *   '<S3>/Derivative Gain'
                                        */
  100.0,                               /* Variable: PID_FN
                                        * Referenced by:
                                        *   '<S2>/Filter Coefficient'
                                        *   '<S3>/Filter Coefficient'
                                        */
  0.0,                                 /* Variable: PID_I
                                        * Referenced by:
                                        *   '<S2>/Integral Gain'
                                        *   '<S3>/Integral Gain'
                                        */
  0.2,                                 /* Variable: PID_P
                                        * Referenced by:
                                        *   '<S2>/Proportional Gain'
                                        *   '<S3>/Proportional Gain'
                                        */

  /*  Expression: [10000 10000]
   * Referenced by: '<S1>/Sine Wave'
   */
  { 10000.0, 10000.0 },
  0.0,                                 /* Expression: 0
                                        * Referenced by: '<S1>/Sine Wave'
                                        */
  1.0,                                 /* Expression: 1
                                        * Referenced by: '<S1>/Sine Wave'
                                        */
  0.0,                                 /* Expression: 0
                                        * Referenced by: '<S1>/Sine Wave'
                                        */
  0.001,                               /* Computed Parameter: Integrator_gainval
                                        * Referenced by: '<S2>/Integrator'
                                        */
  0.0,                                 /* Expression: InitialConditionForIntegrator
                                        * Referenced by: '<S2>/Integrator'
                                        */
  0.001,                               /* Computed Parameter: Filter_gainval
                                        * Referenced by: '<S2>/Filter'
                                        */
  0.0,                                 /* Expression: InitialConditionForFilter
                                        * Referenced by: '<S2>/Filter'
                                        */
  0.0,                                 /* Expression: 0
                                        * Referenced by: '<S2>/Constant'
                                        */
  0.0,                                 /* Expression: 0
                                        * Referenced by: '<S4>/ZeroGain'
                                        */
  0.001,                               /* Computed Parameter: Integrator_gainval_a
                                        * Referenced by: '<S3>/Integrator'
                                        */
  0.0,                                 /* Expression: InitialConditionForIntegrator
                                        * Referenced by: '<S3>/Integrator'
                                        */
  0.001,                               /* Computed Parameter: Filter_gainval_n
                                        * Referenced by: '<S3>/Filter'
                                        */
  0.0,                                 /* Expression: InitialConditionForFilter
                                        * Referenced by: '<S3>/Filter'
                                        */
  0.0,                                 /* Expression: 0
                                        * Referenced by: '<S3>/Constant'
                                        */
  0.0                                  /* Expression: 0
                                        * Referenced by: '<S5>/ZeroGain'
                                        */
};
