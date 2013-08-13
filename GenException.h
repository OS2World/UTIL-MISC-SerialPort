#ifndef GenException_h
#define GenException_h

#include <stdexcept>
#include <strstream>
#include <string>

class GenException :public exception
{
   public:

      GenException(const char* file, const char* function, 
         unsigned long line, const char* errorMsg);
      ~GenException();

      const char* what() const;

   private:

      char* myErrorMessage;

};

inline GenException::GenException(const char* file, const char* function, 
   unsigned long line, const char* errorMsg)
{
   ostrstream aStr;
   aStr << "<" << file << "> <" << function << "> <" << line 
        << "> " << errorMsg << ends;
   myErrorMessage=aStr.str();
}

inline GenException::~GenException()
{
   delete myErrorMessage;
   myErrorMessage=0;
}

inline const char* GenException::what() const
{
   return myErrorMessage;
}

#endif
