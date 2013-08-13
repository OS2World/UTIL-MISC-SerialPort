I originally wrote this code when trying to gain an understanding of my UPS and the signals it delviered with power state changes.

In working with this, I discovered what a pain it is to access the serial ports under OS/2.  (I have never tried to access them under any other OS.)

So I started writting a wrapper class.  It is not complete.  But it is useful to understand what is needed to access a serial port.  It can be extended to be a complete wrapper around a serial port without too much work.

I wrote it using pgcc.  I ran into what seemed to me to be bugs in the streaming.  You may find that they exist still in the GenException (GeneralException) class.  It may have been a problem in my install however.  I am not sure.  

From there I went on to try to find a way to fix the streaming bugs, and never got back to my original goal for writting the software.

Unfortunatly I no longer have OS/2 installed on a computer.  Rather than let this bit of information die, I am giving it away.

I hope it is helpful to you in some way.  Again, I no longer have OS/2 installed, so I can't really help with any problems that may arise.

Andrew Lundgren
