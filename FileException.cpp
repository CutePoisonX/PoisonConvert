/* 
 * File:   FileException.cpp
 * Author: CutePoisonX
 * 
 * Created on 08. Februar 2013, 10:43
 */

#include "FileException.h"

FileException::FileException()
{
}

FileException::FileException(const FileException& orig)
{
}

FileException::~FileException() throw()
{
}

const char* FileException::what() const throw()
{
  return "There occured an error at reading out a file";
}