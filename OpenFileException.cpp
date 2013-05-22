/* 
 * File:   OpenFileException.cpp
 * Author: CutePoisonX
 * 
 * Created on 09. Februar 2013, 12:25
 */

#include "OpenFileException.h"

OpenFileException::OpenFileException()
{
}

OpenFileException::OpenFileException(const OpenFileException& orig)
{
}

OpenFileException::~OpenFileException() throw()
        
{
}

const char* OpenFileException::what() const throw()
{
  return "Could not open File";
}
