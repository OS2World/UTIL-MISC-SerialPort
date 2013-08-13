
#ifndef SerialPort_h
#define SerialPort_h

#define INCL_DOSDEVICES  // defines catagory
#define INCL_DOSDEVIOCTL // defines serial port interaction values

#include <bitset>
#include <string>

#include <os2.h> //HFILE

class SerialPort
{

   public:

      SerialPort(const string& portNum);
      ~SerialPort();

      void open();
      void close();
   
      void setDTR();
      void clearDTR();
      bool DTRstate();
   
      void setRTS();
      void clearRTS();
      bool RTSstate();
   
      void setCTS();
      void clearCTS();
      bool CTSstate();
   
      void setDSR();
      void clearDSR();
      bool DSRstate();
   
      void setRI();
      void clearRI();
      bool RIstate();
   
      void setDCD();
      void clearDCD();
      bool DCDstate();
   
      bitset<8> modemSignals();
   
      // This function does NOT take ownership of the buffer passed to it,
      // if you dynamically allocate the memory, you MUST delete it.
      //
      void sendBuffer(unsigned char* buffer, unsigned long length);
   
      // This function returns a pointer to the buffer it has read from the
      // serial port, as well as the length.  You MUST manage the memory
      // returned to you.
      //
      unsigned char* sendBuffer(unsigned long& length);
   
   protected:

      void toggle(unsigned char newSetting);

      string myPortName;
      HFILE myPort;

};
#endif

