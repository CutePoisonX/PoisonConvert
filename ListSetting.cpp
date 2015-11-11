/*
 * ListSetting.cpp
 *
 *  Created on: 11.11.2015
 *      Author: Christoph
 */

#include "ListSetting.h"
#include "UserInterface.h"
#include <iostream>

ListSetting::ListSetting(UserInterface ui, string name, string description,
                         string one_word_description, string default_param)
: Settings(ui, name, description, "", default_param),
  ONE_WORD_DESCRIPTION_(one_word_description)
{
}

ListSetting::ListSetting(const ListSetting& orig)
: Settings(orig)
{
}

ListSetting::~ListSetting()
{
}

void ListSetting::attemptToChangeParam(string new_param)
{
  new_param[0] = toupper(new_param[0]);

  if (!new_param.empty())
  {
    std::transform(new_param.begin() + 1, new_param.end(), new_param.begin() + 1,
                   ::tolower);
  }

  changeParam(new_param);
}

string const ListSetting::setParam()
{
  string user_selection;
  string new_setting;
  bool new_setting_set = false;

  ui_.writeString("What do you want to do?", true, "green");
  ui_.writeString("add     - add " + ONE_WORD_DESCRIPTION_ + " to the list", true);
  ui_.writeString("remove  - remove " + ONE_WORD_DESCRIPTION_ + " from the list", true);
  ui_.writeString("rewrite - rewrite the whole list", true);
  ui_.writeString("exit    - go back to all settings", true);

  do
  {
    user_selection = ui_.readStringNoCapitalize();

    if (user_selection == "exit")
    {
      return getParam();
    }
    else if (user_selection == "add")
    {
      ui_.writeString("Please enter a " + ONE_WORD_DESCRIPTION_ + " to add (if you want to add multiple " + ONE_WORD_DESCRIPTION_ + "s, separate them with \",\")", true);
      do
      {
        string value_to_add = ui_.readStringNoCapitalize();

        if (value_to_add == "exit")
        {
          return getParam();
        }
        else
        {
          if (getParam().empty())
          {
            new_setting = value_to_add;
          }
          else
          {
            new_setting = getParam() + "," + value_to_add;
          }
          while (*new_setting.rbegin() == ',')
          {
            new_setting = new_setting.substr(0, new_setting.size() - 1);
          }

          new_setting_set = true;
        }
      }
      while (!new_setting_set);
    }
    else if (user_selection == "remove")
    {
      ui_.writeString("Please enter the " + ONE_WORD_DESCRIPTION_ + " to remove", true);
      do
      {
        string value_to_remove = ui_.readStringNoCapitalize();

        if (value_to_remove.find(",") != std::string::npos)
        {
          ui_.writeString("Please enter a string without \",\"", true);
        }
        else if (value_to_remove == "exit")
        {
          return getParam();
        }
        else
        {
          if (stringIsIncluded(value_to_remove))
          {
            new_setting = removeFromSettingsString(value_to_remove);
            new_setting_set = true;
          }
          else
          {
            ui_.writeString(ONE_WORD_DESCRIPTION_ + " is not included in list.", true);
            ui_.readString(false);
            return getParam();
          }
        }
      }
      while (!new_setting_set);
    }
    else if (user_selection == "rewrite")
    {
      ui_.writeString("Please a new list of " + ONE_WORD_DESCRIPTION_ + "s (separate it with \",\")", true);
      string new_list = ui_.readStringNoCapitalize();

      if (new_list == "exit")
      {
        return getParam();
      }
      else
      {
        while (*new_list.rbegin() == ',')
        {
          new_list = new_list.substr(0, new_list.size() - 1);
        }
        new_setting = new_list;
        new_setting_set = true;
      }
    }
  } while (!new_setting_set);

  changeParam(new_setting);

  ui_.writeString("Saved.", true, "green");
  ui_.readString(false);

  return new_setting;
}

vector<string> ListSetting::splitValuesIntoVector() const
{
  vector<string> value_vec;
  string setting_value = getParam();
  int separate_pos;

  setting_value.append(","); //appending last "," so that the last value gets recognized
  while((separate_pos = setting_value.find(",")) != std::string::npos)
  {
    std::string single_setting = setting_value.substr(0, separate_pos);
    std::transform(single_setting.begin(), single_setting.end(), single_setting.begin(),
                   ::tolower);
    value_vec.push_back(single_setting);
    setting_value = setting_value.erase(0, separate_pos + 1);
  }

  return value_vec;
}

string ListSetting::removeFromSettingsString(string string_to_remove) const
{
  vector<string> values = splitValuesIntoVector();
  std::string new_settings_string;

  std::transform(string_to_remove.begin(), string_to_remove.end(),
                 string_to_remove.begin(), ::tolower);

  for (vector<string>::iterator it = values.begin(); it < values.end();)
  {
    if (*it == string_to_remove)
    {
      it = values.erase(it);
    }
    else
    {
      ++it;
    }
  }

  for (unsigned int i = 0; i < values.size(); ++i)
  {
    new_settings_string.append(values[i] + ",");
  }
  new_settings_string = new_settings_string.substr(0, new_settings_string.size() - 1);

  return new_settings_string;
}

bool ListSetting::stringIsIncluded(string string_to_check) const
{
  vector<string> values = splitValuesIntoVector();
  std::transform(string_to_check.begin(), string_to_check.end(), string_to_check.begin(),
                 ::tolower);

  for (unsigned int i = 0; i < values.size(); ++i)
  {
    string value = values[i];
    std::transform(value.begin(), value.end(), value.begin(), ::tolower);

    if (value == string_to_check)
    {
      return true;
    }
  }

  return false;
}

Settings::PARAM_CHANGE_RETURN ListSetting::checkParam(string const& new_param, bool ui_output)
{
  std::string new_param_lowercase = new_param;
  std::transform(new_param_lowercase.begin(), new_param_lowercase.end(), new_param_lowercase.begin(),
                 ::tolower);

  return PARAM_CHANGE_SUCCESS;
}
