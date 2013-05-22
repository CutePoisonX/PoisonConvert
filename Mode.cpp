/* 
 * File:   Mode.cpp
 * Author: CutePoisonX
 * 
 * Created on 08. Februar 2013, 09:41
 */

#include "Mode.h"
#include "UserInterface.h"

Mode::Mode(UserInterface ui)
: ui_(ui)
{
}

Mode::Mode(const Mode& orig)
: ui_(orig.ui_)
{
}

Mode::~Mode()
{
}

