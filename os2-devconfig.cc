
// This bit of code came from BCC/2 help IIRC, it is has been modified to help
// me learn how to create the SerialPort wrapper code I was working on.


#define INCL_DOS
#define INCL_SUB
#define INCL_DOSDEVICES
#define INCL_DOSDATETIME

#include <os2.h>
#include <stdio.h>
#include <strings.h>
#include <stdlib.h>

#define SERIAL		0x01
#define SETBAUDRATE	0x41
#define SETLINECTRL	0x42
#define SETDCBINFO	0x53
#define GETDCBINFO	0x73

#define STACK_SIZE	2048*2
#define BPS		4800
#define KBD_HANDLE	0
#define VIO_HANDLE	0

#define MACHINE_MODEL 5
#define DISPLAY_TYPE 6
#define FIND_COPROCESSOR 3

BYTE DeviceInfo;
APIRET retCode;





struct {
  unsigned char dataBits;
  unsigned char parity;
  unsigned char stopBits;
  }  lineCtrl = { 8,		//8 data bits
		  0,		//No parity
		  0 };		//1 stop bits

struct {
  unsigned short usWriteTimeOut;
  unsigned short usReadTimeOut;
  unsigned char  bFlags1;
  unsigned char  bFlags2;
  unsigned char  bFlags3;
  unsigned char  bErrorReplacementChar;
  unsigned char  bBreakReplacementChar;
  unsigned char  bXONChar;
  unsigned char  bXOFFChar;
  } dcbInfo;


HFILE hCom;			//COM handle
unsigned char inBuffer[256];	//input buffer

void * ComThread(ULONG args);

/************************************************************************/
/*  main								*/
/************************************************************************/
void main(void) 
{

  ULONG		usAction=0;
  USHORT  rc=0;
  USHORT		usBaudRate = BPS;
  unsigned		threadID;
  static char		comThdStk[STACK_SIZE];
  struct _KBDKEYINFO	kbdData;
  ULONG		usBytesWritten=0;

	/* Open and initialize COM1 */

  if (DosOpen((unsigned char *)"COM2", &hCom, &usAction, 0L, 0, 1, 0x12, 0L)) 
  {
    printf("COM2 not available or COM0x.SYS not loaded\n");
    exit(1);
  }
  printf("COM port opened\n");
  DosSleep(1000);

// Set data rate to 4800bps and line format to N81
//  rc= DosDevIOCtl(&hCom, 0L, &usBaudRate, SETBAUDRATE, SERIAL );
//  rc= DosDevIOCtl(0L, &lineCtrl,   SETLINECTRL, SERIAL, hCom);


//  Set device control block parameters
//  rc= DosDevIOCtl(&dcbInfo,    0L, GETDCBINFO, SERIAL, hCom);
//  dcbInfo.usReadTimeOut  = 6000;		//60 second readtimeout
//  dcbInfo.bFlags1 = 0x01;			//enable DTR
//  dcbInfo.bFlags2 = 0x40;			//enable RTS
//  dcbInfo.bFlags3 = 0x04;			//wait-for-something reads
//  rc= DosDevIOCtl(0L, &dcbInfo,    SETDCBINFO, SERIAL, hCom);

// Create a thread to monitor the serial port

  TID ThreadID=0;
  PFNTHREAD ThreadAddr=(PFNTHREAD)ComThread;
  ULONG ThreadArgs=0;
  ULONG ThreadFlags=0;

  ULONG StackSize=(ULONG)(STACK_SIZE+(PBYTE)(comThdStk));
  APIRET returnCode=0;

  returnCode= DosCreateThread(&ThreadID,ThreadAddr, ThreadArgs, ThreadFlags,
     StackSize);

  printf("ThreadID %d \n ReturnCode %d \n",ThreadID,returnCode);
//  rc= DosCreateThread((PFNTHREAD)ComThread, (PTID)&threadID,
//    (PBYTE)comThdStk+STACK_SIZE);

//Monitor the keyboard and output typed characters

  do {
    KbdCharIn(&kbdData, IO_WAIT, KBD_HANDLE);
    if ((kbdData.chChar != 0) && (kbdData.chChar != 0x1B)) 
      {
      VioWrtTTY(&kbdData.chChar, 1, VIO_HANDLE);
      DosWrite(hCom, &kbdData.chChar, 1, &usBytesWritten);
      }
    } while (kbdData.chChar != 0x1B);	//stop on ESCAPE character

  DosExit(EXIT_PROCESS,0);

}


/************************************************************************/
/*  Thread to read characters from COM1 and write to screen		*/
/************************************************************************/
#pragma check_stack(off)	//disable stack checking

void* ComThread(ULONG args) 
{  
/*
   while(1)
   {
	if(!DosDevConfig(&DeviceInfo, MACHINE_MODEL))
	{
		printf("Model Type %d\n", DeviceInfo);
	}
	if(!DosDevConfig(&DeviceInfo, DISPLAY_TYPE))
	{
		printf("Model Type %d\n", DeviceInfo);
	}
   }
*/
  unsigned long usBytesRead=0;
  unsigned long i=0;
  unsigned long category=1;

  PVOID parmList= new BYTE[255];
  memset(parmList,0,255);
  ULONG parmLengthMax=254;
  PULONG parmLengthInOut=0;

  PVOID dataArea= new BYTE[255];
  memset(dataArea,0,255);
  ULONG dataLengthMax=254;
  PULONG dataLengthInOut=0;

  APIRET resultCode=0;

  UCHAR GET_MODEM_OUTPUT_SIGNALS_A=0x66;
  UCHAR SET_MODEM_OUTPUT_SIGNALS_A=0x46;
  UCHAR GET_MODEM_CONTROL_INPUT_B=0x67;
  UCHAR SET_MODEM_CONTROL_INPUT_B=0x47;
  UCHAR DTR_MASK_A=0x01;
  UCHAR RTS_MASK_A=0x02;
  UCHAR CTS_MASK_B=0x10;
  UCHAR DSR_MASK_B=0x20;
  UCHAR RI_MASK_B =0x40;
  UCHAR DCD_MASK_B=0x80;

   printf("Initing UPS\n");

   UCHAR initUPS=0xFE;
   ULONG length = 1;
   PULONG pointerLength=&length;

   resultCode=DosDevIOCtl(hCom,category,SET_MODEM_OUTPUT_SIGNALS_A,
      (void*)&initUPS,parmLengthMax,pointerLength,(void*)&initUPS,dataLengthMax,
      0UL);
  if(resultCode)
  {
     printf("result code %d\n", resultCode);
  }
  else
  {
     printf("DTR set low : code %d\n", resultCode);
  }
   DosSleep(1000);

   initUPS=0x02;
   resultCode=DosDevIOCtl(hCom,category,SET_MODEM_OUTPUT_SIGNALS_A,
      (void*)&initUPS,parmLengthMax,pointerLength,(void*)&initUPS,dataLengthMax,
      0UL);
  if(resultCode)
  {
     printf("result code %d\n", resultCode);
  }
  else
  {
     printf("RTS set high : code %d\n", resultCode);
  }
   DosSleep(1000);





  if(resultCode)
  {
     printf("result code %d\n", resultCode);
  }

  while (!DosRead(hCom, inBuffer, 256, &usBytesRead))
  {
    if (usBytesRead) 
    {
      for (i=0; i<usBytesRead; i++)  inBuffer[i] &= 0x7f;
      {
      VioWrtTTY(inBuffer, usBytesRead, VIO_HANDLE);
      }

     }
     else
     {
/*
        printf("No Data Read\n");
*/
        resultCode=DosDevIOCtl(hCom,category,GET_MODEM_CONTROL_INPUT_B,
           parmList,parmLengthMax,parmLengthInOut,dataArea,dataLengthMax,
           dataLengthInOut);

        if(!resultCode)
        {
/*
           printf("dataLengthInOut %d, parmLengthInOut %d\n", dataLengthInOut,
              parmLengthInOut);
           printf("dataArea[0] %d, parmList[0] %d\n", 
              ((unsigned char*)dataArea)[0],((unsigned char*)parmList)[0]);
*/
           unsigned char modemControlInput=((unsigned char*)dataArea)[0];
           printf("Bit 0-7 %d %d %d %d-%d %d %d %d\n", 
              modemControlInput&0x01,
              modemControlInput&0x02,
              modemControlInput&0x04,
              modemControlInput&0x08,
              modemControlInput&0x10,
              modemControlInput&0x20,
              modemControlInput&0x40,
              modemControlInput&0x80);

           if((modemControlInput&0x10)!=0x10)
           {
              printf("PowerFail, %d\n",modemControlInput&0x10);
              DosBeep(1380,1000);
           }
           else
           {
              printf("PowerOk, %d\n",modemControlInput&0x10);
           }

        }
        else
        {
           printf("error code %d\n", resultCode);
        }
     }
   }

  DosExit(EXIT_THREAD, 1);
}
