//
//    Copyright 2015 CutePoisonX (CutePoisonXI@gmail.com)
//
//    This file is part of PoisonConvert.
//
//    PoisonConvert is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    PoisonConvert is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with PoisonConvert.  If not, see <http://www.gnu.org/licenses/>.
//

#include <cstdlib>
#include <string>
#include <tclap/CmdLine.h>

#include "VectorSourceManager.h"
#include "UserInterface.h"
#include "ConfigMode.h"
#include "FileManager.h"
#include "FileReadException.h"
#include "SettingsMode.h"
#include "StartMode.h"
#include "AnalyzeMedia.h"
#include "Settings.h"
#include "YesNoSetting.h"
 
using namespace std;

string const version = "1.4.1";

bool readOutSettingFile(UserInterface& ui, FileManager& filemanager, std::string& settings_error_msg)
{
  try
  {
    if (filemanager.readSettings() == false)
    {
      settings_error_msg = "There occured an error at reading out the \"Settings\"-file (some settings were corrupt). Please check settings-menu and focus on the red settings.";
      return false;
    }
  } catch (OpenFileException)
  {
    settings_error_msg = "Created default settings-file \"PoisonConvert_Settings\" in /usr/syno/etc/poisonconvert/. Please check settings-menu.";
    try
    {
      filemanager.saveSettingsToFile();
    }
    catch (exception)
    {
      return false;
    }
    
    return false;
  } catch (exception)
  {
    settings_error_msg = "There occured an error at reading out the \"Settings\"-file. Please check settings-menu.";
        
    return false;
  }
    
  return true;
}

void setCommandLineSetting(UserInterface& ui, SettingsMode& settingsmode, std::string const& settings_value, SettingsMode::SETTING_SPECIFIER set_spec)
{
  if (settingsmode.getSettingsParam(set_spec) != settings_value)
  {
    if (settingsmode.writeParam(settings_value, set_spec, true, false) != Settings::PARAM_CHANGE_SUCCESS)
    {
      ui.writeString("Warning: Ignoring invalid value for option: " + settingsmode.getSettingsName(set_spec) + ".", true, "yellow");
    }
  }
}

bool parseCmdLineOptions(int argc, char** argv, UserInterface& ui, FileManager& filemanager, SettingsMode& settingsmode, std::vector<std::string>& config_files)
{
  std::string settings_error_msg;
  bool delete_file_default;
  bool optimize_file_default;
  bool reading_out_settings_succeeded = readOutSettingFile(ui, filemanager, settings_error_msg);
    
  try
  {
    TCLAP::CmdLine cmd("PoisonConvert version " + version + " is part of the MediaWare Factory collection (http://www.mediaware-factory.com).\n"
                       "If you experience any bugs, please file an issue on github. For any other feedback, requests, etc. please use the contact form at MediaWare Factories homepage.\n"
                       "Developed by Christoph Ebner and licensed under the GPLv3 (see: https://www.gnu.org/licenses/gpl)."
                       , ' ', version);
    TCLAP::SwitchArg start_arg("s", "start", "Start converting.", false);
    TCLAP::MultiArg<std::string> conf_file_arg("c", settingsmode.getSettingsName(SettingsMode::CONFIGNAME), "Config file you want to use.", false, "filename");
    TCLAP::ValueArg<std::string> conf_path_arg("p", settingsmode.getSettingsName(SettingsMode::CONFIGLOC), "Path to config files.", false, settingsmode.getSettingsParam(SettingsMode::CONFIGLOC), "path");
    TCLAP::ValueArg<std::string> ffmpeg_cmd_arg("F", settingsmode.getSettingsName(SettingsMode::FFMPEG_CMD), "ffmpeg command.", false, settingsmode.getSettingsParam(SettingsMode::FFMPEG_CMD), "path");
    TCLAP::ValueArg<std::string> ffprobe_cmd_arg("f", settingsmode.getSettingsName(SettingsMode::FFPROBE_CMD), "ffprobe command.", false, settingsmode.getSettingsParam(SettingsMode::FFPROBE_CMD), "path");
    TCLAP::ValueArg<std::string> qt_cmd_arg("q", settingsmode.getSettingsName(SettingsMode::QT_CMD), "qt-faststart command.", false, settingsmode.getSettingsParam(SettingsMode::QT_CMD), "path");

    if (settingsmode.getSettingsParam(SettingsMode::DELETESET) == "Yes")
    {
      delete_file_default = true;
    }
    else
    {
      delete_file_default = false;
    }
    TCLAP::SwitchArg delete_arg("D",settingsmode.getSettingsName(SettingsMode::DELETESET), "Delete original file after conversion.", delete_file_default);

    if (settingsmode.getSettingsParam(SettingsMode::OPTIMIZESET) == "Yes")
    {
      optimize_file_default = true;
    }
    else
    {
      optimize_file_default = false;
    }
    TCLAP::SwitchArg optimize_arg("o", settingsmode.getSettingsName(SettingsMode::OPTIMIZESET), "Optimize file for streaming.", optimize_file_default);

    TCLAP::ValueArg<std::string> log_path_arg("l", settingsmode.getSettingsName(SettingsMode::LOGPATH), "Location of logfiles.", false, settingsmode.getSettingsParam(SettingsMode::LOGPATH), "path");
    TCLAP::ValueArg<std::string> movie_path_arg("m", settingsmode.getSettingsName(SettingsMode::MOVIEPATH), "Where to look for movies.", false, settingsmode.getSettingsParam(SettingsMode::MOVIEPATH), "path");
    TCLAP::ValueArg<std::string> dest_path_arg("d", settingsmode.getSettingsName(SettingsMode::DESTINATION), "Where to save processed movies.", false, settingsmode.getSettingsParam(SettingsMode::DESTINATION), "path");
    TCLAP::ValueArg<std::string> exlude_ext_arg("e", settingsmode.getSettingsName(SettingsMode::EXCLUDE), "List of fileextensions that are never processed.", false, settingsmode.getSettingsParam(SettingsMode::EXCLUDE), "list");

    cmd.add(start_arg);
    cmd.add(conf_file_arg);
    cmd.add(conf_path_arg);
    cmd.add(ffmpeg_cmd_arg);
    cmd.add(ffprobe_cmd_arg);
    cmd.add(qt_cmd_arg);
    cmd.add(delete_arg);
    cmd.add(optimize_arg);
    cmd.add(log_path_arg);
    cmd.add(movie_path_arg);
    cmd.add(dest_path_arg);
    cmd.add(exlude_ext_arg);

    cmd.parse(argc, argv);
    
    if (reading_out_settings_succeeded)
    {
      setCommandLineSetting(ui, settingsmode, conf_path_arg.getValue(), SettingsMode::CONFIGLOC);
      if (delete_arg.getValue() == true)
      {
        setCommandLineSetting(ui, settingsmode, "Yes", SettingsMode::DELETESET);
      }
      if (optimize_arg.getValue() == true)
      {
        setCommandLineSetting(ui, settingsmode, "Yes", SettingsMode::OPTIMIZESET);
      }
      setCommandLineSetting(ui, settingsmode, ffmpeg_cmd_arg.getValue(), SettingsMode::FFMPEG_CMD);
      setCommandLineSetting(ui, settingsmode, ffprobe_cmd_arg.getValue(), SettingsMode::FFPROBE_CMD);
      setCommandLineSetting(ui, settingsmode, qt_cmd_arg.getValue(), SettingsMode::QT_CMD);
      setCommandLineSetting(ui, settingsmode, log_path_arg.getValue(), SettingsMode::LOGPATH);
      setCommandLineSetting(ui, settingsmode, movie_path_arg.getValue(), SettingsMode::MOVIEPATH);
      setCommandLineSetting(ui, settingsmode, dest_path_arg.getValue(), SettingsMode::DESTINATION);
      setCommandLineSetting(ui, settingsmode, exlude_ext_arg.getValue(), SettingsMode::EXCLUDE);
    }
    else
    {
      ui.writeString(settings_error_msg, true, "red");
      ui.writeString("Ignored any command line options.", true, "yellow");
      return false;
    }
    
    config_files = conf_file_arg.getValue();  //the config files get checked later if they exist, etc ...
    if (start_arg.getValue())
    {
      return true;
    }
    else
    {
      return false;
    }
  }
  catch(std::exception const& except)
  {
  	ui.writeString(except.what(), true, "red");
  	exit(-1);
  }
}

bool readOutPreferenceFile(UserInterface& ui, FileManager& filemanager, VectorSourceManager& vecman)
{
  try
  {
    filemanager.readPreferences();
  } catch (OpenFileException)
  {
    ui.writeString("Can not open config-file. Please check if file is present and you entered filename and path correctly.", true, "red");
    ui.writeString("Please go to settings-menu and enter the correct configuration-settings.", true, "red");
    vecman.clearAllInstances();
    return false;
  } catch (exception)
  {
    ui.writeString("There occured an error at reading out the config-file. It seems that it is corrupt.", true, "red");
    ui.writeString("Loaded without any configurations.", true, "red");
    vecman.clearAllInstances();
    return false;
  }

  return true;
}

bool executeStartMode(UserInterface& ui, SettingsMode& settingsmode, StartMode& startmode,
                      VectorSourceManager& vecman, AnalyzeMedia& analyze, bool automatic_mode)
{
  bool settings_fail = false;
  std::string warning_output_color;
    
  if (automatic_mode)
  {
    warning_output_color = "red";
  }
  else
  {
    warning_output_color = "green";
  }
    
  for (int i = 0; i < settingsmode.getVectorLen(); i++)
  {
    if (settingsmode.checkParam(static_cast<SettingsMode::SETTING_SPECIFIER>(i), false) == Settings::PARAM_CHANGE_ERROR)
    {
      ui.writeString("Invalid setting: ", false, "red");
      ui.writeString(settingsmode.getSettingsName(i), true, "red");
      settings_fail = true;
    }
  }

  if (settings_fail == true)
  {
    ui.writeString("Please enter listed setting(s). Go to 'settings - menu' to do so.", true, warning_output_color);
    if (automatic_mode == false)
    {
      ui.readString(false);
    }
    return false;
  }
  if (vecman.getVectorLen(SRCVIDEO) != 0 || vecman.getVectorLen(SRCAUDIO) != 0 ||
      vecman.getVectorLen(SRCSUB) != 0)
  {
    if (settings_fail == false)
    {
      try
      {
        startmode.executeCommand();
      } catch (exception)
      {
        ui.writeString("It seems that there occured an unknown error. Please report it, so we can fix this bug together.", true, "red");
        analyze.clearAllInstances();
        vecman.clearAllInstances();
        return false;
      }
      return true;
    }
  }
  else
  {
    ui.writeString("You have to specify rules first. Go to 'config - menu' to do so.", true, warning_output_color);
    if (automatic_mode == false)
    {
      ui.readString(false);
    }
    return false;
  }
}

bool startInNormalMode(UserInterface& ui, FileManager& filemanager, VectorSourceManager& vecman,
                       SettingsMode& settingsmode, StartMode& startmode, AnalyzeMedia& analyze,
                       ConfigMode& configmode)
{
  string input;
  bool going_back = true;

  if (readOutPreferenceFile(ui, filemanager, vecman) == false)
  {
    ui.readString(false);
  }

  ui.writeString("", true);
  ui.writeString("**************************************************************", true);
  ui.writeString("****************** ");
  ui.writeString("WELCOME TO POISONCONVERT", false, "green");
  ui.writeString(" ******************", true);
  ui.writeString("**************************************************************", true);
  ui.writeString("", true);

  while (input != "exit")
  {
    if (going_back == true)
    {
      ui.writeString("Please select one of the following options:", true, "red");
      ui.writeString("  [");
      ui.writeString("start", false, "blue");
      ui.writeString("]    - start conversion with custom settings.", true);
      ui.writeString("  [");
      ui.writeString("config", false, "blue");
      ui.writeString("]   - videostream management.", true);
      ui.writeString("  [");
      ui.writeString("settings", false, "blue");
      ui.writeString("] - enter some settings.", true);
      ui.writeString("  [");
      ui.writeString("exit", false, "blue");
      ui.writeString("]     - quit program.", true);
      ui.writeString("", true);
      going_back = false;
    }
    input = ui.readStringNoCapitalize();
    if (input == "start")
    {
      going_back = executeStartMode(ui, settingsmode, startmode, vecman, analyze, false);
        
    } else if (input == "config")
    {
      try
      {
        if( filemanager.checkPathToConfig() == "" )
        {
          ui.writeString("Warning: You did not specify a path to the config-file in settings. Changes will not be saved.", true, "red");
          ui.readString(false);
        }
        configmode.executeCommand();
        if(filemanager.savePreferencesToFile() == -1)
        {
          ui.writeString("Did not save changes because you did not specify a path to a config-file in settings.", true, "red");
          ui.readString(false);
        }
      } catch (FileWriteException)
      {
        ui.writeString("Can not write config-file. Please check if poisonconvert has enough rights to do so.", true, "red");
      } catch (exception)
      {
        ui.writeString("It seems that there occured an unknown error. Please report it, so we can fix this bug together.", true, "red");
        analyze.clearAllInstances();
        vecman.clearAllInstances();
        return -1;
      }
      going_back = true;
    } else if (input == "settings")
    {
      try
      {
        if (settingsmode.executeCommand() == SettingsMode::SAVE_SETTINGS)
        {
          filemanager.saveSettingsToFile();
        }
      } catch (FileWriteException)
      {
        ui.writeString("Can not write settings-file. Please check if poisonconvert has enough rights to do so.", true, "red");
        ui.readString(false);
      } catch (exception)
      {
        ui.writeString("It seems that there occured an unknown error. Please report it, so we can fix this bug together.", true, "red");
        analyze.clearAllInstances();
        vecman.clearAllInstances();
        return -1;
      }
      if (filemanager.checkPathToConfig() != "")
      {
        try
        {
          vecman.clearAllInstances();
          filemanager.readPreferences();
        } catch (OpenFileException)
        {
          ui.writeString("Attention: Config file empty or not created (this is normal if you specified a new config-file. Just create some configurations).", true, "red");
          ui.readString(false);
        } catch (exception)
        {
          ui.writeString("There occured an error at reading out the config-file. It seems, that it is corrupt.", true, "red");
          ui.writeString("Loaded without any configurations.", true, "red");
          vecman.clearAllInstances();
          ui.readString(false);
        }
      } else
      {
        ui.writeString("Warning: You did not specify a path to the config-file in settings. Cannot read/create config-file.", true, "red");
        ui.readString(false);
      }
      going_back = true;
    } else if (input != "exit")
    {
      ui.writeString("Please enter one of the listed options!", true);
    }
  }
}

int main(int argc, char** argv)
{
  bool immediate_start;
  std::vector<std::string> config_files;

  UserInterface ui;
  VectorSourceManager vecman;
  AnalyzeMedia analyze;
  ConfigMode configmode(ui, vecman);
  SettingsMode settingsmode(ui);

  FileManager filemanager(settingsmode, vecman, analyze);
  StartMode startmode(ui, vecman, filemanager, analyze, settingsmode);

  immediate_start = parseCmdLineOptions(argc, argv, ui, filemanager, settingsmode, config_files);

  if (immediate_start)
  {
    if (config_files.empty())
    {
      config_files.push_back(settingsmode.getSettingsParam(SettingsMode::CONFIGNAME)); //using default config file
    }
    for (unsigned int config_file_nr = 0; config_file_nr < config_files.size(); ++config_file_nr)
    {
      std::string config_file = config_files[config_file_nr];
      settingsmode.writeParam(config_file, SettingsMode::CONFIGNAME, true, false);

      ui.writeString("Processing config-file: " + config_file, true);
      if (readOutPreferenceFile(ui, filemanager, vecman) == true)
      {
        if (executeStartMode(ui, settingsmode, startmode, vecman, analyze, true) == false)
        {
          return -1;
        }
      }
      ui.writeString("", true);
    }
      
    return 0;
  }
  else if (config_files.size() > 1)
  {
    ui.writeString("Illegal option: " + settingsmode.getSettingsName(SettingsMode::CONFIGNAME) + ". Multiple config files are only allowed in start-mode (invoke with -s or --start).", true, "red");
    return -1;
  }
  else if (config_files.empty() == false)
  {
    settingsmode.writeParam(config_files[0], SettingsMode::CONFIGNAME, true, false);
  }

  analyze.clearAllInstances();
  vecman.clearAllInstances();
  return startInNormalMode(ui, filemanager, vecman, settingsmode, startmode, analyze, configmode);
}
