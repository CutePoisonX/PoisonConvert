/* 
 * File:   SettingsMode.cpp
 * Author: CutePoisonX
 * 
 * Created on 08. Februar 2013, 09:40
 */

#include "SettingsMode.h"
#include "UserInterface.h"


SettingsMode::SettingsMode(UserInterface& ui, string pref_filename, 
                           bool erase_original, bool optimize_streaming)
: Mode(ui), pref_filename_(pref_filename), erase_original_(erase_original), 
        optimize_streaming_(optimize_streaming)
{
}

SettingsMode::SettingsMode(const SettingsMode& orig)
: Mode(orig), erase_original_(orig.erase_original_), pref_filename_(pref_filename_),
optimize_streaming_(orig.optimize_streaming_)
{
}

SettingsMode::~SettingsMode()
{
}

int SettingsMode::executeCommand()
{
  bool endthis = false;
  bool going_back = true;

  do
  {
    if (going_back == true)
    {
      standartExecutePrompt();
      going_back = false;
    }

    input_ = ui_.readStringNoCapitalize();
    if (input_ == first_selection)
    {
      changeFilename();
      going_back = true;
    } else if (input_ == second_selection)
    {
      changeDeleteOriginal();
      going_back = true;
    } else if (input_ == "exit")
    {
      return -1;
    } else if (input_ == third_selection)
    {
      changeOptimizeForStreaming();
      going_back = true;
    }
    else
    {
      going_back = false;
      ui_.writeString("Please enter one of the options listed above (without ");
      ui_.writeString("the []).", true);
      endthis = false;
    }
  } while (endthis == false);
}

int SettingsMode::listSettings()
{
  ui_.writeString("--------------------------------------------------------", true);
  ui_.writeString(" -> ");  ui_.writeString(first_list_setting); ui_.writeString(": ");
  ui_. writeString(pref_filename_, true, "green");
    
  ui_.writeString(" -> "); ui_.writeString(second_list_setting); ui_.writeString(": ");
  if(erase_original_ == true)
    ui_.writeString("Yes", true, "green");
  else
    ui_.writeString("No", true, "green");
  
    ui_.writeString(" -> "); ui_.writeString(third_list_setting); ui_.writeString(": ");
  if(optimize_streaming_ == true)
    ui_.writeString("Yes", true, "green");
  else
    ui_.writeString("No", true, "green");   
  ui_.writeString("--------------------------------------------------------", true);
}

void SettingsMode::standartExecutePrompt()
{
  ui_.writeString("", true);
  ui_.writeString("Settings Mode!", true, "red");
  ui_.writeString("These are your currrent settings:", true);
  listSettings();
  ui_.writeString("Which of them do you want to change?", true);
  
  ui_.writeString("  [");
  ui_.writeString(first_selection, false, "yellow");
  ui_.writeString("] - ");
  ui_.writeString(first_list_setting, true);
  
  ui_.writeString("  [");
  ui_.writeString(second_selection,  false, "yellow");
  ui_.writeString("]   - ");
  ui_.writeString(second_list_setting, true);
  
    ui_.writeString("  [");
  ui_.writeString(third_selection,  false, "yellow");
  ui_.writeString("]   - ");
  ui_.writeString(third_list_setting, true);
  
  ui_.writeString("  ["); ui_.writeString("exit", false, "yellow");
   ui_.writeString("]     - go back to main screen");
  ui_.writeString("", true);
}

int SettingsMode::changeFilename()
{
  ui_.writeString("Please enter the filename of the configuration-file ", false, "green");
  ui_.writeString("without the fileextension.", true, "green");
  pref_filename_ = ui_.readString();
  ui_.writeString("Saved. Please restart to load this file.", true, "green");
  ui_.readString(false);
}

void SettingsMode::changeDeleteOriginal()
{
  ui_.writeString("Do you want to delete the original file after the successful conversion? NOT RECOMMENDET ([y/n])", true, "green");
  do
  {
    input_ = ui_.readStringNoCapitalize();
    if (input_ == "y")
    {
      erase_original_ = true;
    } else if (input_ != "n")
    {
      ui_.writeString("Plaese enter [y] for 'yes' and [n] for 'no'!", true);
    } else
    {
      erase_original_ = false;
    }
  } while (input_ != "y" && input_ != "n");
  ui_.writeString("Saved.", true, "green");
}

 void SettingsMode::changeOptimizeForStreaming()
 {
  ui_.writeString("Do you want to optimize file for streaming (mov/mp4/m4v only , [y/n])?", true, "green");
  do
  {
    input_ = ui_.readStringNoCapitalize();
    if (input_ == "y")
    {
      optimize_streaming_ = true;
    } else if (input_ != "n")
    {
      ui_.writeString("Plaese enter [y] for 'yes' and [n] for 'no'!", true);
    } else
    {
      optimize_streaming_ = false;
    }
  } while (input_ != "y" && input_ != "n");
  ui_.writeString("Saved.", true, "green");
}