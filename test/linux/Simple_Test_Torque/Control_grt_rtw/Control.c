/*
 * Control.c
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

/* Block signals (auto storage) */
B_Control_T Control_B;

/* Block states (auto storage) */
DW_Control_T Control_DW;

/* External inputs (root inport signals with auto storage) */
ExtU_Control_T Control_U;

/* External outputs (root outports fed by signals with auto storage) */
ExtY_Control_T Control_Y;

/* Real-time model */
RT_MODEL_Control_T Control_M_;
RT_MODEL_Control_T *const Control_M = &Control_M_;

/* Model step function */
void Control_step(void)
{
  real_T rtb_SignPreIntegrator;
  real_T rtb_IntegralGain;
  real_T rtb_Saturate;
  real_T rtb_ZeroGain;
  boolean_T rtb_NotEqual;
  real_T rtb_ZeroGain_n;
  int8_T rtb_IntegralGain_0;

  /* Sin: '<S1>/Sine Wave' */
  rtb_ZeroGain_n = sin(Control_P.SineWave_Freq * Control_M->Timing.t[0] +
                       Control_P.SineWave_Phase);

  /* Sum: '<S1>/Sum' incorporates:
   *  Inport: '<Root>/Position'
   *  Inport: '<Root>/Position_Cmd'
   *  Sin: '<S1>/Sine Wave'
   *  Sum: '<S1>/Sum2'
   */
  rtb_SignPreIntegrator = ((rtb_ZeroGain_n * Control_P.SineWave_Amp[0] +
    Control_P.SineWave_Bias) + Control_U.Position_Cmd[0]) - Control_U.Position[0];

  /* DiscreteIntegrator: '<S2>/Integrator' */
  Control_B.Integrator = Control_DW.Integrator_DSTATE;

  /* DiscreteIntegrator: '<S2>/Filter' */
  Control_B.Filter = Control_DW.Filter_DSTATE;

  /* Gain: '<S2>/Filter Coefficient' incorporates:
   *  Gain: '<S2>/Derivative Gain'
   *  Sum: '<S2>/SumD'
   */
  Control_B.FilterCoefficient = (Control_P.PID_D * rtb_SignPreIntegrator -
    Control_B.Filter) * Control_P.PID_FN;

  /* Sum: '<S2>/Sum' incorporates:
   *  Gain: '<S2>/Proportional Gain'
   */
  rtb_IntegralGain = (Control_P.PID_P * rtb_SignPreIntegrator +
                      Control_B.Integrator) + Control_B.FilterCoefficient;

  /* Gain: '<S2>/Integral Gain' */
  rtb_SignPreIntegrator *= Control_P.PID_I;

  /* Saturate: '<S2>/Saturate' */
  if (rtb_IntegralGain > Control_P.MAX_TORQUE_OUTPUT) {
    rtb_Saturate = Control_P.MAX_TORQUE_OUTPUT;
  } else if (rtb_IntegralGain < -Control_P.MAX_TORQUE_OUTPUT) {
    rtb_Saturate = -Control_P.MAX_TORQUE_OUTPUT;
  } else {
    rtb_Saturate = rtb_IntegralGain;
  }

  /* End of Saturate: '<S2>/Saturate' */

  /* Gain: '<S4>/ZeroGain' */
  rtb_ZeroGain = Control_P.ZeroGain_Gain * rtb_IntegralGain;

  /* DeadZone: '<S4>/DeadZone' */
  if (rtb_IntegralGain > Control_P.MAX_TORQUE_OUTPUT) {
    rtb_IntegralGain -= Control_P.MAX_TORQUE_OUTPUT;
  } else if (rtb_IntegralGain >= -Control_P.MAX_TORQUE_OUTPUT) {
    rtb_IntegralGain = 0.0;
  } else {
    rtb_IntegralGain -= -Control_P.MAX_TORQUE_OUTPUT;
  }

  /* End of DeadZone: '<S4>/DeadZone' */

  /* RelationalOperator: '<S4>/NotEqual' */
  rtb_NotEqual = (rtb_ZeroGain != rtb_IntegralGain);

  /* Signum: '<S4>/SignDeltaU' */
  if (rtb_IntegralGain < 0.0) {
    rtb_IntegralGain = -1.0;
  } else if (rtb_IntegralGain > 0.0) {
    rtb_IntegralGain = 1.0;
  } else {
    if (rtb_IntegralGain == 0.0) {
      rtb_IntegralGain = 0.0;
    }
  }

  /* End of Signum: '<S4>/SignDeltaU' */

  /* Signum: '<S4>/SignPreIntegrator' */
  if (rtb_SignPreIntegrator < 0.0) {
    /* DataTypeConversion: '<S4>/DataTypeConv2' */
    rtb_ZeroGain = -1.0;
  } else if (rtb_SignPreIntegrator > 0.0) {
    /* DataTypeConversion: '<S4>/DataTypeConv2' */
    rtb_ZeroGain = 1.0;
  } else if (rtb_SignPreIntegrator == 0.0) {
    /* DataTypeConversion: '<S4>/DataTypeConv2' */
    rtb_ZeroGain = 0.0;
  } else {
    /* DataTypeConversion: '<S4>/DataTypeConv2' */
    rtb_ZeroGain = rtb_SignPreIntegrator;
  }

  /* End of Signum: '<S4>/SignPreIntegrator' */

  /* DataTypeConversion: '<S4>/DataTypeConv2' */
  if (rtIsNaN(rtb_ZeroGain) || rtIsInf(rtb_ZeroGain)) {
    rtb_ZeroGain = 0.0;
  } else {
    rtb_ZeroGain = fmod(rtb_ZeroGain, 256.0);
  }

  /* DataTypeConversion: '<S4>/DataTypeConv1' */
  if (rtb_IntegralGain < 128.0) {
    rtb_IntegralGain_0 = (int8_T)rtb_IntegralGain;
  } else {
    rtb_IntegralGain_0 = MAX_int8_T;
  }

  /* End of DataTypeConversion: '<S4>/DataTypeConv1' */

  /* Switch: '<S2>/Switch' incorporates:
   *  Constant: '<S2>/Constant'
   *  DataTypeConversion: '<S4>/DataTypeConv2'
   *  Logic: '<S4>/AND'
   *  RelationalOperator: '<S4>/Equal'
   */
  if (rtb_NotEqual && ((rtb_ZeroGain < 0.0 ? (int32_T)(int8_T)-(int8_T)(uint8_T)
                        -rtb_ZeroGain : (int32_T)(int8_T)(uint8_T)rtb_ZeroGain) ==
                       rtb_IntegralGain_0)) {
    Control_B.Switch = Control_P.Constant_Value;
  } else {
    Control_B.Switch = rtb_SignPreIntegrator;
  }

  /* End of Switch: '<S2>/Switch' */

  /* Sum: '<S1>/Sum1' incorporates:
   *  Inport: '<Root>/Position'
   *  Inport: '<Root>/Position_Cmd'
   *  Sin: '<S1>/Sine Wave'
   *  Sum: '<S1>/Sum2'
   */
  rtb_IntegralGain = ((rtb_ZeroGain_n * Control_P.SineWave_Amp[1] +
                       Control_P.SineWave_Bias) + Control_U.Position_Cmd[1]) -
    Control_U.Position[1];

  /* DiscreteIntegrator: '<S3>/Integrator' */
  Control_B.Integrator_h = Control_DW.Integrator_DSTATE_f;

  /* DiscreteIntegrator: '<S3>/Filter' */
  Control_B.Filter_g = Control_DW.Filter_DSTATE_a;

  /* Gain: '<S3>/Filter Coefficient' incorporates:
   *  Gain: '<S3>/Derivative Gain'
   *  Sum: '<S3>/SumD'
   */
  Control_B.FilterCoefficient_c = (Control_P.PID_D * rtb_IntegralGain -
    Control_B.Filter_g) * Control_P.PID_FN;

  /* Sum: '<S3>/Sum' incorporates:
   *  Gain: '<S3>/Proportional Gain'
   */
  rtb_SignPreIntegrator = (Control_P.PID_P * rtb_IntegralGain +
    Control_B.Integrator_h) + Control_B.FilterCoefficient_c;

  /* Gain: '<S3>/Integral Gain' */
  rtb_IntegralGain *= Control_P.PID_I;

  /* Saturate: '<S3>/Saturate' */
  if (rtb_SignPreIntegrator > Control_P.MAX_TORQUE_OUTPUT) {
    rtb_ZeroGain = Control_P.MAX_TORQUE_OUTPUT;
  } else if (rtb_SignPreIntegrator < -Control_P.MAX_TORQUE_OUTPUT) {
    rtb_ZeroGain = -Control_P.MAX_TORQUE_OUTPUT;
  } else {
    rtb_ZeroGain = rtb_SignPreIntegrator;
  }

  /* End of Saturate: '<S3>/Saturate' */

  /* Gain: '<S5>/ZeroGain' */
  rtb_ZeroGain_n = Control_P.ZeroGain_Gain_o * rtb_SignPreIntegrator;

  /* DeadZone: '<S5>/DeadZone' */
  if (rtb_SignPreIntegrator > Control_P.MAX_TORQUE_OUTPUT) {
    rtb_SignPreIntegrator -= Control_P.MAX_TORQUE_OUTPUT;
  } else if (rtb_SignPreIntegrator >= -Control_P.MAX_TORQUE_OUTPUT) {
    rtb_SignPreIntegrator = 0.0;
  } else {
    rtb_SignPreIntegrator -= -Control_P.MAX_TORQUE_OUTPUT;
  }

  /* End of DeadZone: '<S5>/DeadZone' */

  /* RelationalOperator: '<S5>/NotEqual' */
  rtb_NotEqual = (rtb_ZeroGain_n != rtb_SignPreIntegrator);

  /* Signum: '<S5>/SignDeltaU' */
  if (rtb_SignPreIntegrator < 0.0) {
    rtb_SignPreIntegrator = -1.0;
  } else if (rtb_SignPreIntegrator > 0.0) {
    rtb_SignPreIntegrator = 1.0;
  } else {
    if (rtb_SignPreIntegrator == 0.0) {
      rtb_SignPreIntegrator = 0.0;
    }
  }

  /* End of Signum: '<S5>/SignDeltaU' */

  /* Signum: '<S5>/SignPreIntegrator' */
  if (rtb_IntegralGain < 0.0) {
    /* DataTypeConversion: '<S5>/DataTypeConv2' */
    rtb_ZeroGain_n = -1.0;
  } else if (rtb_IntegralGain > 0.0) {
    /* DataTypeConversion: '<S5>/DataTypeConv2' */
    rtb_ZeroGain_n = 1.0;
  } else if (rtb_IntegralGain == 0.0) {
    /* DataTypeConversion: '<S5>/DataTypeConv2' */
    rtb_ZeroGain_n = 0.0;
  } else {
    /* DataTypeConversion: '<S5>/DataTypeConv2' */
    rtb_ZeroGain_n = rtb_IntegralGain;
  }

  /* End of Signum: '<S5>/SignPreIntegrator' */

  /* DataTypeConversion: '<S5>/DataTypeConv2' */
  if (rtIsNaN(rtb_ZeroGain_n) || rtIsInf(rtb_ZeroGain_n)) {
    rtb_ZeroGain_n = 0.0;
  } else {
    rtb_ZeroGain_n = fmod(rtb_ZeroGain_n, 256.0);
  }

  /* DataTypeConversion: '<S5>/DataTypeConv1' */
  if (rtb_SignPreIntegrator < 128.0) {
    rtb_IntegralGain_0 = (int8_T)rtb_SignPreIntegrator;
  } else {
    rtb_IntegralGain_0 = MAX_int8_T;
  }

  /* End of DataTypeConversion: '<S5>/DataTypeConv1' */

  /* Switch: '<S3>/Switch' incorporates:
   *  Constant: '<S3>/Constant'
   *  DataTypeConversion: '<S5>/DataTypeConv2'
   *  Logic: '<S5>/AND'
   *  RelationalOperator: '<S5>/Equal'
   */
  if (rtb_NotEqual && ((rtb_ZeroGain_n < 0.0 ? (int32_T)(int8_T)-(int8_T)
                        (uint8_T)-rtb_ZeroGain_n : (int32_T)(int8_T)(uint8_T)
                        rtb_ZeroGain_n) == rtb_IntegralGain_0)) {
    Control_B.Switch_k = Control_P.Constant_Value_h;
  } else {
    Control_B.Switch_k = rtb_IntegralGain;
  }

  /* End of Switch: '<S3>/Switch' */

  /* Outport: '<Root>/Torque_Cmd' */
  Control_Y.Torque_Cmd[0] = rtb_Saturate;
  Control_Y.Torque_Cmd[1] = rtb_ZeroGain;

  /* Matfile logging */
  rt_UpdateTXYLogVars(Control_M->rtwLogInfo, (Control_M->Timing.t));

  /* Update for DiscreteIntegrator: '<S2>/Integrator' */
  Control_DW.Integrator_DSTATE += Control_P.Integrator_gainval *
    Control_B.Switch;

  /* Update for DiscreteIntegrator: '<S2>/Filter' */
  Control_DW.Filter_DSTATE += Control_P.Filter_gainval *
    Control_B.FilterCoefficient;

  /* Update for DiscreteIntegrator: '<S3>/Integrator' */
  Control_DW.Integrator_DSTATE_f += Control_P.Integrator_gainval_a *
    Control_B.Switch_k;

  /* Update for DiscreteIntegrator: '<S3>/Filter' */
  Control_DW.Filter_DSTATE_a += Control_P.Filter_gainval_n *
    Control_B.FilterCoefficient_c;

  /* signal main to stop simulation */
  {                                    /* Sample time: [0.0s, 0.0s] */
    if ((rtmGetTFinal(Control_M)!=-1) &&
        !((rtmGetTFinal(Control_M)-Control_M->Timing.t[0]) > Control_M->
          Timing.t[0] * (DBL_EPSILON))) {
      rtmSetErrorStatus(Control_M, "Simulation finished");
    }
  }

  /* Update absolute time for base rate */
  /* The "clockTick0" counts the number of times the code of this task has
   * been executed. The absolute time is the multiplication of "clockTick0"
   * and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
   * overflow during the application lifespan selected.
   * Timer of this task consists of two 32 bit unsigned integers.
   * The two integers represent the low bits Timing.clockTick0 and the high bits
   * Timing.clockTickH0. When the low bit overflows to 0, the high bits increment.
   */
  if (!(++Control_M->Timing.clockTick0)) {
    ++Control_M->Timing.clockTickH0;
  }

  Control_M->Timing.t[0] = Control_M->Timing.clockTick0 *
    Control_M->Timing.stepSize0 + Control_M->Timing.clockTickH0 *
    Control_M->Timing.stepSize0 * 4294967296.0;

  {
    /* Update absolute timer for sample time: [0.001s, 0.0s] */
    /* The "clockTick1" counts the number of times the code of this task has
     * been executed. The resolution of this integer timer is 0.001, which is the step size
     * of the task. Size of "clockTick1" ensures timer will not overflow during the
     * application lifespan selected.
     * Timer of this task consists of two 32 bit unsigned integers.
     * The two integers represent the low bits Timing.clockTick1 and the high bits
     * Timing.clockTickH1. When the low bit overflows to 0, the high bits increment.
     */
    Control_M->Timing.clockTick1++;
    if (!Control_M->Timing.clockTick1) {
      Control_M->Timing.clockTickH1++;
    }
  }
}

/* Model initialize function */
void Control_initialize(void)
{
  /* Registration code */

  /* initialize non-finites */
  rt_InitInfAndNaN(sizeof(real_T));

  /* initialize real-time model */
  (void) memset((void *)Control_M, 0,
                sizeof(RT_MODEL_Control_T));

  {
    /* Setup solver object */
    rtsiSetSimTimeStepPtr(&Control_M->solverInfo, &Control_M->Timing.simTimeStep);
    rtsiSetTPtr(&Control_M->solverInfo, &rtmGetTPtr(Control_M));
    rtsiSetStepSizePtr(&Control_M->solverInfo, &Control_M->Timing.stepSize0);
    rtsiSetErrorStatusPtr(&Control_M->solverInfo, (&rtmGetErrorStatus(Control_M)));
    rtsiSetRTModelPtr(&Control_M->solverInfo, Control_M);
  }

  rtsiSetSimTimeStep(&Control_M->solverInfo, MAJOR_TIME_STEP);
  rtsiSetSolverName(&Control_M->solverInfo,"FixedStepDiscrete");
  rtmSetTPtr(Control_M, &Control_M->Timing.tArray[0]);
  rtmSetTFinal(Control_M, 10.0);
  Control_M->Timing.stepSize0 = 0.001;

  /* Setup for data logging */
  {
    static RTWLogInfo rt_DataLoggingInfo;
    rt_DataLoggingInfo.loggingInterval = NULL;
    Control_M->rtwLogInfo = &rt_DataLoggingInfo;
  }

  /* Setup for data logging */
  {
    rtliSetLogXSignalInfo(Control_M->rtwLogInfo, (NULL));
    rtliSetLogXSignalPtrs(Control_M->rtwLogInfo, (NULL));
    rtliSetLogT(Control_M->rtwLogInfo, "tout");
    rtliSetLogX(Control_M->rtwLogInfo, "");
    rtliSetLogXFinal(Control_M->rtwLogInfo, "");
    rtliSetLogVarNameModifier(Control_M->rtwLogInfo, "rt_");
    rtliSetLogFormat(Control_M->rtwLogInfo, 4);
    rtliSetLogMaxRows(Control_M->rtwLogInfo, 0);
    rtliSetLogDecimation(Control_M->rtwLogInfo, 1);
    rtliSetLogY(Control_M->rtwLogInfo, "");
    rtliSetLogYSignalInfo(Control_M->rtwLogInfo, (NULL));
    rtliSetLogYSignalPtrs(Control_M->rtwLogInfo, (NULL));
  }

  /* block I/O */
  (void) memset(((void *) &Control_B), 0,
                sizeof(B_Control_T));

  /* states (dwork) */
  (void) memset((void *)&Control_DW, 0,
                sizeof(DW_Control_T));

  /* external inputs */
  (void)memset((void *)&Control_U, 0, sizeof(ExtU_Control_T));

  /* external outputs */
  (void) memset(&Control_Y.Torque_Cmd[0], 0,
                2U*sizeof(real_T));

  /* Matfile logging */
  rt_StartDataLoggingWithStartTime(Control_M->rtwLogInfo, 0.0, rtmGetTFinal
    (Control_M), Control_M->Timing.stepSize0, (&rtmGetErrorStatus(Control_M)));

  /* InitializeConditions for DiscreteIntegrator: '<S2>/Integrator' */
  Control_DW.Integrator_DSTATE = Control_P.Integrator_IC;

  /* InitializeConditions for DiscreteIntegrator: '<S2>/Filter' */
  Control_DW.Filter_DSTATE = Control_P.Filter_IC;

  /* InitializeConditions for DiscreteIntegrator: '<S3>/Integrator' */
  Control_DW.Integrator_DSTATE_f = Control_P.Integrator_IC_n;

  /* InitializeConditions for DiscreteIntegrator: '<S3>/Filter' */
  Control_DW.Filter_DSTATE_a = Control_P.Filter_IC_e;
}

/* Model terminate function */
void Control_terminate(void)
{
  /* (no terminate code required) */
}
