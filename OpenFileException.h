/* 
 * File:   OpenFileException.h
 * Author: CutePoisonX
 *
 * Created on 09. Februar 2013, 12:25
 */

#ifndef OPENFILEEXCEPTION_H
#define	OPENFILEEXCEPTION_H

#include "FileException.h"


class OpenFileException : public FileException{
public:
  
                 OpenFileException();
                 OpenFileException(const OpenFileException& orig);
  virtual       ~OpenFileException() throw();

  virtual const char* what() const throw ();

};

#endif	/* OPENFILEEXCEPTION_H */

