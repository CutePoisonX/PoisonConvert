/* 
 * File:   FileWriteException.cpp
 * Author: CutePoisonX
 * 
 * Created on 09. Februar 2013, 13:03
 */

#include "FileWriteException.h"

FileWriteException::FileWriteException()
{
}

FileWriteException::FileWriteException(const FileWriteException& orig)
{
}

FileWriteException::~FileWriteException() throw()
{
}

const char* FileWriteException::what() const throw()
{
  return "Could not write File";
}