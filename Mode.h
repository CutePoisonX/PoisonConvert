/* 
 * File:   Mode.h
 * Author: CutePoisonX
 *
 * Created on 08. Februar 2013, 09:41
 */

#ifndef MODE_H
#define	MODE_H

#include <string>

using namespace std;

class UserInterface;

class Mode {
public:
  
                Mode(UserInterface ui);
                Mode(const Mode& orig);
  virtual      ~Mode();
  
  virtual int executeCommand() = 0;
  
protected:
  
  //----------------------------------------------------------------------------
  UserInterface& ui_;
  string input_;
  //----------------------------------------------------------------------------
};

#endif	/* MODE_H */

