/*
 * ListSetting.h
 *
 *  Created on: 11.11.2015
 *      Author: Christoph
 */

#ifndef LISTSETTING_H_
#define LISTSETTING_H_

#include "Settings.h"
#include <vector>
#include <string>

using namespace std;

class ListSetting: public Settings
{
  public:
    ListSetting(UserInterface ui, string name, string description,
                string one_word_description, string default_param);
    ListSetting(const ListSetting& orig);
    virtual ~ListSetting();

    virtual Settings::PARAM_CHANGE_RETURN checkParam(string const& new_param, bool ui_output);
    bool stringIsIncluded(string string_to_check) const;

  private:
      string const setParam();
      void attemptToChangeParam(string new_param);
      vector<string> splitValuesIntoVector() const;
      string removeFromSettingsString(string string_to_remove) const;

      string const ONE_WORD_DESCRIPTION_;
};

#endif /* LISTSETTING_H_ */
