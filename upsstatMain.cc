#define INCLUDE_DOSDATETIME

#include <SerialPort.h>
#include <unistd.h>
#include <GenException.h>
#include <stdio.h>

// This code was originally written to monitor the 
// com-port that my UPS was plugged into to discover the state
// changes.  That is why it manipulates DTR and RTS
// The commented out section is where I seemed to have found a
// bug in the ostrstream that is provided with gcc; unless
// I mis-used it, or had a bad setup. Either way the serialPort
// class is an example of how to access the serial ports under OS/2
// something I did not find very easily.

int main(int argc, const char** argv)
{

//cout << __LINE__ << endl;
//char *mine;
//cout << __LINE__ << endl;
//mine = new char[1024];
//cout << __LINE__ << endl;
//ostrstream a(mine,1024);
//cout << __LINE__ << endl;
//a << "bug" ;
//cout << __LINE__ << endl;
//a << ends;
//cout << __LINE__ << endl;
//char* temp = a.str();
//cout << __LINE__ << endl;
//cout << temp << endl;
//cout << __LINE__ << endl;
//delete []temp;
//cout << __LINE__ << endl;

   // Set the port to the arg1 that is passed in.

   SerialPort aPort(argv[1]);
   try
   {
      aPort.open();
      sleep(5);
      aPort.clearDTR();
      sleep(5);
      aPort.setRTS();
      sleep(50);
      aPort.clearRTS();
      sleep(5);
      aPort.setDTR();
   }
   catch(GenException &e)
   {
      cout << e.what() << endl;
   }
   catch(exception &e)
   {
      cout << e.what() << endl;
   }
   catch(...)
   {
      cout << "Caught unexpected exception. Exiting." << endl;
   }
}

