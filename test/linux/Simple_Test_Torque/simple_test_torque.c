/** 
 *  (c) 2014, Manuel Vonthron - OPAL-RT Technologies, inc.
 * From https://gist.github.com/manuelvonthron-opalrt/9467152
 */

#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <pthread.h>

#include <math.h>

#include "ethercattype.h"
#include "nicdrv.h"
#include "ethercatbase.h"
#include "ethercatmain.h"
#include "ethercatdc.h"
#include "ethercatcoe.h"
#include "ethercatfoe.h"
#include "ethercatconfig.h"
#include "ethercatprint.h"

#include "control.h"

#define EC_TIMEOUTMON 500

#define INITIAL_POS 0

char IOmap[4096];
pthread_t thread1;
int expectedWKC;
boolean needlf;
volatile int wkc;
boolean inOP;
uint8 currentgroup = 0;

/**
 * helper macros
 */
#define READ(idx, sub, buf, comment)    \
    {   \
        buf=0;  \
        int __s = sizeof(buf);    \
        int __ret = ec_SDOread(1, idx, sub, FALSE, &__s, &buf, EC_TIMEOUTRXM);   \
        printf("Read at 0x%04x:%d => wkc: %d; data: 0x%.*x (%d)\t[%s]\n", idx, sub, __ret, __s, (unsigned int)buf, (unsigned int)buf, comment);    \
     }

#define READ_i(slave, idx, sub, buf, comment)    \
    {   \
        buf=0;  \
        int __s = sizeof(buf);    \
        int __ret = ec_SDOread(slave, idx, sub, FALSE, &__s, &buf, EC_TIMEOUTRXM);   \
        printf("Read from slave %d at 0x%04x:%d => wkc: %d; data: 0x%.*x (%d)\t[%s]\n", slave, idx, sub, __ret, __s, (unsigned int)buf, (unsigned int)buf, comment);    \
     }

#define WRITE(idx, sub, buf, value, comment) \
    {   \
        int __s = sizeof(buf);  \
        buf = value;    \
        int __ret = ec_SDOwrite(1, idx, sub, FALSE, __s, &buf, EC_TIMEOUTRXM);  \
        printf("Write at 0x%04x:%d => wkc: %d; data: 0x%.*x\t{%s}\n", idx, sub, __ret, __s, (unsigned int)buf, comment);    \
    }

#define WRITE_i(slave, idx, sub, buf, value, comment) \
    {   \
        int __s = sizeof(buf);  \
        buf = value;    \
        int __ret = ec_SDOwrite(slave, idx, sub, FALSE, __s, &buf, EC_TIMEOUTRXM);  \
        printf("Write to slave %d at 0x%04x:%d => wkc: %d; data: 0x%.*x\t{%s}\n", slave, idx, sub, __ret, __s, (unsigned int)buf, comment);    \
    }

#define CHECKERROR()   \
{   \
    ec_readstate();\
    printf("EC> \"%s\" %x - %x [%s] \n", (char*)ec_elist2string(), ec_slave[1].state, ec_slave[1].ALstatuscode, (char*)ec_ALstatuscode2string(ec_slave[1].ALstatuscode));    \
}

#define CHECKERROR_i(i)   \
{   \
    ec_readstate();\
    printf("EC> \"%s\" %x - %x [%s] \n", (char*)ec_elist2string(), ec_slave[i].state, ec_slave[i].ALstatuscode, (char*)ec_ALstatuscode2string(ec_slave[i].ALstatuscode));    \
}

#define MAX_EC_SLAVECOUNT 3

void simpletest(char *ifname)
{
    int i, oloop, iloop, chk, slave;
    needlf = FALSE;
    inOP = FALSE;

    uint32 buf32;
    uint16 buf16;
    uint8 buf8;

    struct TorqueIn *val[MAX_EC_SLAVECOUNT+1];
    struct TorqueOut *target[MAX_EC_SLAVECOUNT+1];

    struct Controller controller;
   printf("Starting simple test\n");

   /* initialise SOEM, bind socket to ifname */
   if (ec_init(ifname))
   {
      printf("ec_init on %s succeeded.\n",ifname);
      /* find and auto-config slaves */

      /** network discovery */
      if ( ec_config_init(FALSE) > 0 )
      {
         printf("%d slaves found and configured.\n",ec_slavecount);

         printf("Has CA? \n");
         for (i = 1; i <= ec_slavecount; i++)
           printf("%s", ec_slave[i].CoEdetails & ECT_COEDET_SDOCA ? "true ":"false " );
         printf("\n");

         /** CompleteAccess disabled for Elmo driver */
         for (i = 1; i <= ec_slavecount; i++)
          ec_slave[1].CoEdetails ^= ECT_COEDET_SDOCA;

         ec_statecheck(0, EC_STATE_PRE_OP,  EC_TIMEOUTSTATE);

         /** set PDO mapping */
         /** opMode: 8  => Position profile */
         for (i = 1; i <= ec_slavecount; i++)
         {
           WRITE_i(i, 0x6060, 0, buf8, 10, "OpMode");
           READ_i(i, 0x6061, 0, buf8, "OpMode display");


           READ_i(i, 0x1c12, 0, buf32, "rxPDO:0");
           READ_i(i, 0x1c13, 0, buf32, "txPDO:0");

           READ_i(i, 0x1c12, 1, buf32, "rxPDO:1");
           READ_i(i, 0x1c13, 1, buf32, "txPDO:1");

//         WRITE(0x1c12, 1, buf16, 0x1601, "rxPDO");
//         WRITE(0x1c13, 1, buf16, 0x1A01, "txPDO");
         }

        int32 ob2;int os;
        for (i = 1; i <= ec_slavecount; i++)
        {
           os=sizeof(ob2); ob2 = 0x16020001;
           ec_SDOwrite(i,0x1c12,0,TRUE,os,&ob2,EC_TIMEOUTRXM);
           os=sizeof(ob2); ob2 = 0x1a020001;
           ec_SDOwrite(i,0x1c13,0, TRUE, os,&ob2,EC_TIMEOUTRXM);
        }

//         WRITE(0x1c12, 0, buf32, 0x16010001, "rxPDO");
//         WRITE(0x1c13, 0, buf32, 0x1A010001, "txPDO");
        for (i = 1; i <= ec_slavecount; i++)
        {
         READ_i(i, 0x1c12, 0, buf32, "rxPDO:0");
         READ_i(i, 0x1c13, 0, buf32, "txPDO:0");

         READ_i(i, 0x1c12, 1, buf32, "rxPDO:1");
         READ_i(i, 0x1c13, 1, buf32, "txPDO:1");
       }

         /** if CA disable => automapping works */
         ec_config_map(&IOmap);

         /** let DC off for the time being */
//         ec_configdc();

        for (i = 1; i <= ec_slavecount; i++)
        {
         printf("\nSlave:%d\n Name:%s\n Output size: %dbits\n Input size: %dbits\n State: %d\n Delay: %d[ns]\n Has DC: %d\n",
               i, ec_slave[i].name, ec_slave[i].Obits, ec_slave[i].Ibits,
               ec_slave[i].state, ec_slave[i].pdelay, ec_slave[i].hasdc);
        }

         /** disable heartbeat alarm */
        for (i = 1; i <= ec_slavecount; i++)
        {     
         READ_i(i, 0x10F1, 2, buf32, "Heartbeat?");
         WRITE_i(i, 0x10F1, 2, buf32, 1, "Heartbeat");


         WRITE_i(i, 0x60c2, 1, buf8, 2, "Time period");
         WRITE_i(i, 0x2f75, 0, buf16, 2, "Interpolation timeout");
        }

         printf("Slaves mapped, state to SAFE_OP.\n");

         int timestep = 700;

         /* wait for all slaves to reach SAFE_OP state */
         ec_statecheck(0, EC_STATE_SAFE_OP,  EC_TIMEOUTSTATE * 4);

         /** old SOEM code, inactive */
         oloop = ec_slave[0].Obytes;
         if ((oloop == 0) && (ec_slave[0].Obits > 0)) oloop = 1;
         if (oloop > 20) oloop = 8;
         iloop = ec_slave[0].Ibytes;
         if ((iloop == 0) && (ec_slave[0].Ibits > 0)) iloop = 1;
         if (iloop > 20) iloop = 8;

         printf("segments : %d : %d %d %d %d\n",ec_group[0].nsegments ,ec_group[0].IOsegment[0],ec_group[0].IOsegment[1],ec_group[0].IOsegment[2],ec_group[0].IOsegment[3]);

         printf("Request operational state for all slaves\n");
         expectedWKC = (ec_group[0].outputsWKC * 2) + ec_group[0].inputsWKC;
         printf("Calculated workcounter %d\n", expectedWKC);

         /** going operational */
         ec_slave[0].state = EC_STATE_OPERATIONAL;
         /* send one valid process data to make outputs in slaves happy*/
         ec_send_processdata();
         ec_receive_processdata(EC_TIMEOUTRET);

         for (i = 1; i <= ec_slavecount; i++)
         {
           READ_i(i, 0x6083, 0, buf32, "Profile acceleration");
           READ_i(i, 0x6084, 0, buf32, "Profile deceleration");
           READ_i(i, 0x6085, 0, buf32, "Quick stop deceleration");
         }
         /* request OP state for all slaves */
         ec_writestate(0);
         chk = 40;
         /* wait for all slaves to reach OP state */
         do
         {
            ec_send_processdata();
            ec_receive_processdata(EC_TIMEOUTRET);
            ec_statecheck(0, EC_STATE_OPERATIONAL, 50000);
         }
         while (chk-- && (ec_slave[0].state != EC_STATE_OPERATIONAL));
         if (ec_slave[0].state == EC_STATE_OPERATIONAL )
         {
            printf("Operational state reached for all slaves.\n");
            inOP = TRUE;

            int reachedInitial[MAX_EC_SLAVECOUNT+1];
            for (i = 0; i <= ec_slavecount; i++)
              reachedInitial[i] = 0;

            /**
             * Drive state machine transistions
             *   0 -> 6 -> 7 -> 15
             */
            for (i = 1; i <= ec_slavecount; i++)
            {
              READ_i(i, 0x6041, 0, buf16, "*status word*");
              if(buf16 == 0x218){
                  WRITE_i(i, 0x6040, 0, buf16, 128, "*control word*"); usleep(100000);
                  READ_i(i, 0x6041, 0, buf16, "*status word*");
              }

              WRITE_i(i, 0x6040, 0, buf16, 0, "*control word*"); usleep(100000);
              READ_i(i, 0x6041, 0, buf16, "*status word*");

              WRITE_i(i, 0x6040, 0, buf16, 6, "*control word*"); usleep(100000);
              READ_i(i, 0x6041, 0, buf16, "*status word*");

              WRITE_i(i, 0x6040, 0, buf16, 7, "*control word*"); usleep(100000);
              READ_i(i, 0x6041, 0, buf16, "*status word*");

              WRITE_i(i, 0x6040, 0, buf16, 15, "*control word*"); usleep(100000);
              READ_i(i, 0x6041, 0, buf16, "*status word*");

              CHECKERROR_i(i);
              READ_i(i, 0x1a0b, 0, buf8, "OpMode Display");
            }

            for (i = 1; i <= ec_slavecount; i++)
            {

              READ_i(i, 0x1001, 0, buf8, "Error");

              /* cyclic loop */
              target[i] = (struct TorqueOut *)(ec_slave[i].outputs);
              val[i] = (struct TorqueIn *)(ec_slave[i].inputs);
            }


            for(i = 1; i <= 100000; i++)
            {
               /** PDO I/O refresh */
               ec_send_processdata();
               wkc = ec_receive_processdata(EC_TIMEOUTRET);

                    if(wkc >= expectedWKC)
                    {
                        printf("Processdata cycle %4d, WKC %d,", i, wkc);
                        for (slave = 1; slave <= ec_slavecount; slave ++)
                        {
                          printf("  pos: 0x%x, tor: 0x%x, stat: 0x%x, mode: 0x%x", val[slave]->position, val[slave]->torque, val[slave]->status, val[slave]->profile);

                          /** if in fault or in the way to normal status, we update the state machine */
                          switch(target[slave]->status){
                          case 0:
                              target[slave]->status = 6;
                              break;
                          case 6:
                              target[slave]->status = 7;
                              break;
                          case 7:
                              target[slave]->status = 15;
                              break;
                          case 128:
                              target[slave]->status = 0;
                              break;
                          default:
                              if(val[slave]->status >> 3 & 0x01){
                                  READ_i(slave, 0x1001, 0, buf8, "Error");
                                  target[slave]->status = 128;
                              }
  //                            break;
                          }
                        }

                        for (slave = 1; slave <= ec_slavecount; slave ++)
                        {
                          /** we wait to be in ready-to-run mode and with initial value reached */
                          if(reachedInitial[slave] == 0 /*&& val->position == INITIAL_POS */&& (val[slave]->status & 0x0fff) == 0x0237){
                              reachedInitial[slave] = 1;
                          }

                          if((val[slave]->status & 0x0fff) == 0x0237 && reachedInitial[slave]){
                              controller_loop(&controller, val, target);
                          }

                          printf("  Target: 0x%x, control: 0x%x, slave %d\n", target[slave]->torque, target[slave]->status, slave);
                        }
                      
                        printf("\r");
                        needlf = TRUE;
                    }
                    usleep(timestep);

                }
                inOP = FALSE;
            }
            else
            {
                printf("Not all slaves reached operational state.\n");
                ec_readstate();
                for(i = 1; i<=ec_slavecount ; i++)
                {
                    if(ec_slave[i].state != EC_STATE_OPERATIONAL)
                    {
                        printf("Slave %d State=0x%2.2x StatusCode=0x%4.4x : %s\n",
                            i, ec_slave[i].state, ec_slave[i].ALstatuscode, ec_ALstatuscode2string(ec_slave[i].ALstatuscode));
                    }
                }
            }


            printf("\nRequest init state for all slaves\n");
            for (i = 1; i <= ec_slavecount; i++)
            {
              WRITE_i(i, 0x10F1, 2, buf32, 0, "Heartbeat");
            }
            ec_slave[0].state = EC_STATE_INIT;
            /* request INIT state for all slaves */
            ec_writestate(0);
        }
        else
        {
            printf("No slaves found!\n");
        }
        printf("End simple test, close socket\n");
        /* stop SOEM, close socket */
        ec_close();
    }
    else
    {
        printf("No socket connection on %s\nExcecute as root\n",ifname);
    }
}

void ecatcheck( void *ptr )
{
    int slave;
    if (ptr == NULL)
        {;} // just to make the compiler happy

    while(1)
    {
        if( inOP && ((wkc < expectedWKC) || ec_group[currentgroup].docheckstate))
        {
            if (needlf)
            {
               needlf = FALSE;
               printf("\n");
            }
            /* one ore more slaves are not responding */
            ec_group[currentgroup].docheckstate = FALSE;
            ec_readstate();
            for (slave = 1; slave <= ec_slavecount; slave++)
            {
               if ((ec_slave[slave].group == currentgroup) && (ec_slave[slave].state != EC_STATE_OPERATIONAL))
               {
                  ec_group[currentgroup].docheckstate = TRUE;
                  if (ec_slave[slave].state == (EC_STATE_SAFE_OP + EC_STATE_ERROR))
                  {
                     printf("ERROR : slave %d is in SAFE_OP + ERROR, attempting ack.\n", slave);
                     ec_slave[slave].state = (EC_STATE_SAFE_OP + EC_STATE_ACK);
                     ec_writestate(slave);
                  }
                  else if(ec_slave[slave].state == EC_STATE_SAFE_OP)
                  {
                     printf("WARNING : slave %d is in SAFE_OP, change to OPERATIONAL.\n", slave);
                     ec_slave[slave].state = EC_STATE_OPERATIONAL;
                     ec_writestate(slave);
                  }
                  else if(ec_slave[slave].state > 0)
                  {
                     if (ec_reconfig_slave(slave, EC_TIMEOUTMON))
                     {
                        ec_slave[slave].islost = FALSE;
                        printf("MESSAGE : slave %d reconfigured\n",slave);
                     }
                  }
                  else if(!ec_slave[slave].islost)
                  {
                     /* re-check state */
                     ec_statecheck(slave, EC_STATE_OPERATIONAL, EC_TIMEOUTRET);
                     if (!ec_slave[slave].state)
                     {
                        ec_slave[slave].islost = TRUE;
                        printf("ERROR : slave %d lost\n",slave);
                     }
                  }
               }
               if (ec_slave[slave].islost)
               {
                  if(!ec_slave[slave].state)
                  {
                     if (ec_recover_slave(slave, EC_TIMEOUTMON))
                     {
                        ec_slave[slave].islost = FALSE;
                        printf("MESSAGE : slave %d recovered\n",slave);
                     }
                  }
                  else
                  {
                     ec_slave[slave].islost = FALSE;
                     printf("MESSAGE : slave %d found\n",slave);
                  }
               }
            }
            if(!ec_group[currentgroup].docheckstate)
               printf(".");
        }
        usleep(250);
    }
}

int main(int argc, char *argv[])
{
   
   printf("SOEM (Simple Open EtherCAT Master)\nSimple test\n");

   if (argc > 1)
   {
      /* create thread to handle slave error handling in OP */
      pthread_create( &thread1, NULL, (void *) &ecatcheck, (void*) &ctime);
      //  int iret1;
      // iret = ...
      /* start cyclic part */
      simpletest(argv[1]);
   }
   else
   {
      printf("Usage: simple_test ifname1\nifname = eth0 for example\n");
   }

   printf("End program\n");
   return (0);
}
