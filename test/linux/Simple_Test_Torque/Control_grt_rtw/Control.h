/*
 * Control.h
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

#ifndef RTW_HEADER_Control_h_
#define RTW_HEADER_Control_h_
#include <math.h>
#include <string.h>
#include <float.h>
#include <stddef.h>
#ifndef Control_COMMON_INCLUDES_
# define Control_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#include "rt_logging.h"
#endif                                 /* Control_COMMON_INCLUDES_ */

#include "Control_types.h"

/* Shared type includes */
#include "multiword_types.h"
#include "rt_nonfinite.h"
#include "rtGetInf.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetFinalTime
# define rtmGetFinalTime(rtm)          ((rtm)->Timing.tFinal)
#endif

#ifndef rtmGetRTWLogInfo
# define rtmGetRTWLogInfo(rtm)         ((rtm)->rtwLogInfo)
#endif

#ifndef rtmGetErrorStatus
# define rtmGetErrorStatus(rtm)        ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
# define rtmSetErrorStatus(rtm, val)   ((rtm)->errorStatus = (val))
#endif

#ifndef rtmGetStopRequested
# define rtmGetStopRequested(rtm)      ((rtm)->Timing.stopRequestedFlag)
#endif

#ifndef rtmSetStopRequested
# define rtmSetStopRequested(rtm, val) ((rtm)->Timing.stopRequestedFlag = (val))
#endif

#ifndef rtmGetStopRequestedPtr
# define rtmGetStopRequestedPtr(rtm)   (&((rtm)->Timing.stopRequestedFlag))
#endif

#ifndef rtmGetT
# define rtmGetT(rtm)                  (rtmGetTPtr((rtm))[0])
#endif

#ifndef rtmGetTFinal
# define rtmGetTFinal(rtm)             ((rtm)->Timing.tFinal)
#endif

/* Block signals (auto storage) */
typedef struct {
  real_T Integrator;                   /* '<S2>/Integrator' */
  real_T Filter;                       /* '<S2>/Filter' */
  real_T FilterCoefficient;            /* '<S2>/Filter Coefficient' */
  real_T Switch;                       /* '<S2>/Switch' */
  real_T Integrator_h;                 /* '<S3>/Integrator' */
  real_T Filter_g;                     /* '<S3>/Filter' */
  real_T FilterCoefficient_c;          /* '<S3>/Filter Coefficient' */
  real_T Switch_k;                     /* '<S3>/Switch' */
} B_Control_T;

/* Block states (auto storage) for system '<Root>' */
typedef struct {
  real_T Integrator_DSTATE;            /* '<S2>/Integrator' */
  real_T Filter_DSTATE;                /* '<S2>/Filter' */
  real_T Integrator_DSTATE_f;          /* '<S3>/Integrator' */
  real_T Filter_DSTATE_a;              /* '<S3>/Filter' */
} DW_Control_T;

/* External inputs (root inport signals with auto storage) */
typedef struct {
  real_T Position_Cmd[2];              /* '<Root>/Position_Cmd' */
  real_T Position[2];                  /* '<Root>/Position' */
} ExtU_Control_T;

/* External outputs (root outports fed by signals with auto storage) */
typedef struct {
  real_T Torque_Cmd[2];                /* '<Root>/Torque_Cmd' */
} ExtY_Control_T;

/* Parameters (auto storage) */
struct P_Control_T_ {
  real_T MAX_TORQUE_OUTPUT;            /* Variable: MAX_TORQUE_OUTPUT
                                        * Referenced by:
                                        *   '<S2>/Saturate'
                                        *   '<S3>/Saturate'
                                        *   '<S4>/DeadZone'
                                        *   '<S5>/DeadZone'
                                        */
  real_T PID_D;                        /* Variable: PID_D
                                        * Referenced by:
                                        *   '<S2>/Derivative Gain'
                                        *   '<S3>/Derivative Gain'
                                        */
  real_T PID_FN;                       /* Variable: PID_FN
                                        * Referenced by:
                                        *   '<S2>/Filter Coefficient'
                                        *   '<S3>/Filter Coefficient'
                                        */
  real_T PID_I;                        /* Variable: PID_I
                                        * Referenced by:
                                        *   '<S2>/Integral Gain'
                                        *   '<S3>/Integral Gain'
                                        */
  real_T PID_P;                        /* Variable: PID_P
                                        * Referenced by:
                                        *   '<S2>/Proportional Gain'
                                        *   '<S3>/Proportional Gain'
                                        */
  real_T SineWave_Amp[2];              /* Expression: [10000 10000]
                                        * Referenced by: '<S1>/Sine Wave'
                                        */
  real_T SineWave_Bias;                /* Expression: 0
                                        * Referenced by: '<S1>/Sine Wave'
                                        */
  real_T SineWave_Freq;                /* Expression: 1
                                        * Referenced by: '<S1>/Sine Wave'
                                        */
  real_T SineWave_Phase;               /* Expression: 0
                                        * Referenced by: '<S1>/Sine Wave'
                                        */
  real_T Integrator_gainval;           /* Computed Parameter: Integrator_gainval
                                        * Referenced by: '<S2>/Integrator'
                                        */
  real_T Integrator_IC;                /* Expression: InitialConditionForIntegrator
                                        * Referenced by: '<S2>/Integrator'
                                        */
  real_T Filter_gainval;               /* Computed Parameter: Filter_gainval
                                        * Referenced by: '<S2>/Filter'
                                        */
  real_T Filter_IC;                    /* Expression: InitialConditionForFilter
                                        * Referenced by: '<S2>/Filter'
                                        */
  real_T Constant_Value;               /* Expression: 0
                                        * Referenced by: '<S2>/Constant'
                                        */
  real_T ZeroGain_Gain;                /* Expression: 0
                                        * Referenced by: '<S4>/ZeroGain'
                                        */
  real_T Integrator_gainval_a;         /* Computed Parameter: Integrator_gainval_a
                                        * Referenced by: '<S3>/Integrator'
                                        */
  real_T Integrator_IC_n;              /* Expression: InitialConditionForIntegrator
                                        * Referenced by: '<S3>/Integrator'
                                        */
  real_T Filter_gainval_n;             /* Computed Parameter: Filter_gainval_n
                                        * Referenced by: '<S3>/Filter'
                                        */
  real_T Filter_IC_e;                  /* Expression: InitialConditionForFilter
                                        * Referenced by: '<S3>/Filter'
                                        */
  real_T Constant_Value_h;             /* Expression: 0
                                        * Referenced by: '<S3>/Constant'
                                        */
  real_T ZeroGain_Gain_o;              /* Expression: 0
                                        * Referenced by: '<S5>/ZeroGain'
                                        */
};

/* Real-time Model Data Structure */
struct tag_RTM_Control_T {
  const char_T *errorStatus;
  RTWLogInfo *rtwLogInfo;
  RTWSolverInfo solverInfo;

  /*
   * Timing:
   * The following substructure contains information regarding
   * the timing information for the model.
   */
  struct {
    uint32_T clockTick0;
    uint32_T clockTickH0;
    time_T stepSize0;
    uint32_T clockTick1;
    uint32_T clockTickH1;
    time_T tFinal;
    SimTimeStep simTimeStep;
    boolean_T stopRequestedFlag;
    time_T *t;
    time_T tArray[2];
  } Timing;
};

/* Block parameters (auto storage) */
extern P_Control_T Control_P;

/* Block signals (auto storage) */
extern B_Control_T Control_B;

/* Block states (auto storage) */
extern DW_Control_T Control_DW;

/* External inputs (root inport signals with auto storage) */
extern ExtU_Control_T Control_U;

/* External outputs (root outports fed by signals with auto storage) */
extern ExtY_Control_T Control_Y;

/* Model entry point functions */
extern void Control_initialize(void);
extern void Control_step(void);
extern void Control_terminate(void);

/* Real-time Model object */
extern RT_MODEL_Control_T *const Control_M;

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Note that this particular code originates from a subsystem build,
 * and has its own system numbers different from the parent model.
 * Refer to the system hierarchy for this subsystem below, and use the
 * MATLAB hilite_system command to trace the generated code back
 * to the parent model.  For example,
 *
 * hilite_system('controller/Control')    - opens subsystem controller/Control
 * hilite_system('controller/Control/Kp') - opens and selects block Kp
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'controller'
 * '<S1>'   : 'controller/Control'
 * '<S2>'   : 'controller/Control/PID Controller'
 * '<S3>'   : 'controller/Control/PID Controller1'
 * '<S4>'   : 'controller/Control/PID Controller/Clamping circuit'
 * '<S5>'   : 'controller/Control/PID Controller1/Clamping circuit'
 */
#endif                                 /* RTW_HEADER_Control_h_ */
