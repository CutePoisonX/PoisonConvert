/* 
 * File:   FileException.h
 * Author: CutePoisonX
 *
 * Created on 08. Februar 2013, 10:43
 */

#ifndef FILEEXCEPTION_H
#define	FILEEXCEPTION_H

#include <exception>

using namespace std;

class FileException : public exception{
public:
  
                FileException();
                FileException(const FileException& orig);
  virtual      ~FileException() throw();

  virtual const char* what() const throw ();
};

#endif	/* FILEEXCEPTION_H */

