
#include <GenException.h>
#include <SerialPort.h>
#include <stdio.h>
#include <exception>

SerialPort::SerialPort(const string& portNum):myPortName(portNum)
{
}

SerialPort::~SerialPort()
{
}

void SerialPort::open()
{
   unsigned long usAction=0;

   APIRET resultCode=DosOpen(
      reinterpret_cast<const unsigned char*>(myPortName.c_str()), &myPort, 
         &usAction, 0L, 0, 1, 0x12, 0L);

   if(resultCode) 
   {
      //  NOTE, THE STREAMS SEEM TO BE CORING WITH GCC
      // It may be a problem with my install.  If you have problems here
      // try re-writting this section without streams.  (Or maybe find my bug!)
      //
      cerr << __FILE__ << " " << __LINE__ << endl;
      ostrstream anError;
      cerr << __FILE__ << " " << __LINE__ << endl;
      anError << myPortName << " is not available. System Error code: "
              << resultCode << ends;
      cerr << __FILE__ << " " << __LINE__ << endl;

      char* buffer=anError.str();
      cerr << __FILE__ << " " << __LINE__ << endl;

      throw GenException(__FILE__,__FUNCTION__,__LINE__,buffer);
      cerr << __FILE__ << " " << __LINE__ << endl;
      delete []buffer;
      cerr << __FILE__ << " " << __LINE__ << endl;
   }
}

void SerialPort::setDTR()
{
   // DTR_OFF is defined in the INCL_DOSDEVIOCT
   //
   toggle(DTR_ON);
}

void SerialPort::clearDTR()
{
   // DTR_OFF is defined in the INCL_DOSDEVIOCT
   //
   toggle(DTR_OFF);
}

void SerialPort::setRTS()
{
   // RTS_ON is defined in the INCL_DOSDEVIOCT
   //
   toggle(RTS_ON);
}

void SerialPort::clearRTS()
{
   // RTS_ON is defined in the INCL_DOSDEVIOCT
   //
   toggle(RTS_OFF);
}

void SerialPort::toggle(unsigned char newSetting)
{
   unsigned char unUsed=0;
   void * unUsedPtr=reinterpret_cast<void*>(&unUsed);
   unsigned long unUsedLen=0;
   unsigned long* unUsedLenPtr=&unUsedLen;

   unsigned long length=1;
   unsigned long* lenPtr=&length;

   APIRET resultCode=DosDevIOCtl(myPort,IOCTL_ASYNC,ASYNC_SETMODEMCTRL,
       (void*)&newSetting,length,lenPtr,unUsedPtr,unUsedLen, unUsedLenPtr);
}
