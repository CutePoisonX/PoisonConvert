//
//    Copyright 2014 CutePoisonX (CutePoisonXI@gmail.com)
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

#include "FileManager.h"
#include "FileWriteException.h"
#include "VectorSourceManager.h"
#include "AnalyzeMedia.h"
#include "SettingsMode.h"
#include "Settings.h"
#include <sstream>
#include <vector>
#include <cstdlib>
#include <math.h>

using namespace std;

FileManager::FileManager(SettingsMode& settings, VectorSourceManager& vec_man,
                         AnalyzeMedia& analyze)
: setting_(settings), vec_man_(vec_man), analyze_(analyze)
{
}

FileManager::FileManager(const FileManager& orig)
: setting_(orig.setting_), vec_man_(orig.vec_man_), analyze_(orig.analyze_)
{
}

FileManager::~FileManager()
{
}

int FileManager::readSettings() throw(FileReadException,
                                      OpenFileException)
{ 
  unsigned int num_settings;
  ifstream readfile;
  string tmp_setting;
  string tmp_string;
   
  num_settings = setting_.getVectorLen();
  //Head
  readfile.open("/etc/PoisonConvert_Settings");
  if (readfile.is_open() == false)
  {
    throw OpenFileException();
  }
  getline(readfile, tmp_string, '\n');
  ReadFileError(readfile);
  if (tmp_string != "-----Settings-----")
  {
    readfile.close();
    throw FileReadException();
  }
  
  for(unsigned int i=0; i<num_settings; i++)
  {
    getline(readfile, tmp_string, ' ');

    ReadFileError(readfile);
    if (tmp_string != setting_.getSettingsName(i))
    {
      readfile.close();
      throw FileReadException();
    }
    getline(readfile, tmp_setting, '\n');

    ReadFileError(readfile);
    if (setting_.writeParam(tmp_setting, tmp_string, false) == Settings::PARAM_CHANGE_ERROR)
    {
      throw FileReadException();
    }
  }

  //End of settings
  getline(readfile, tmp_string, '\n');
  ReadFileError(readfile);
  if (tmp_string != "-----Settings-----")
  {
    readfile.close();
    throw FileReadException();
  }
  
  getline(readfile, tmp_string, '\n');
  if (readfile.eof() != true)
  {
    readfile.close();
    throw FileReadException();
  }
  readfile.close();
}

void FileManager::saveSettingsToFile() throw(FileWriteException)
{
  ofstream writefile;
  unsigned int num_settings;
   
  num_settings = setting_.getVectorLen();
  
  writefile.open("/etc/PoisonConvert_Settings", ostream::out);
    if (writefile.is_open() == false)
    {
      writefile.close();
      throw FileWriteException();
    }
  
  //Head
  writefile << "-----Settings-----" << endl;
  
  //First setting (filename)
  for(unsigned int i=0; i<num_settings; i++)
  {
    writefile << setting_.getSettingsName(i); writefile << " ";
    writefile << setting_.getSettingsParam(i) << endl;
  }
  
  //End of file
  writefile << "-----Settings-----" << endl;
  
  writefile.close();
}

void FileManager::ReadFileError(ifstream& readfile) throw(FileReadException)
{
  if (readfile.good() != true)
  {
    readfile.close();
    throw FileReadException();
  }
}

int FileManager::savePreferencesToFile() throw(FileWriteException)
{
  ofstream writefile;
  string filename = setting_.getSettingsParam(SettingsMode::CONFIGNAME);
  string config_file;
  unsigned int identifier = 0;
  
  config_file = checkPathToConfig();
  if(config_file == "")
  {
    return -1; //no path to config-file specified
  }
  
  config_file.append(filename);
 
  //filename.append(".txt");
  //Try open stream
  writefile.open(config_file.c_str(), ostream::out);
  if (writefile.is_open() == false)
  {
    writefile.close();
    throw FileWriteException();
  }
  
  //Head
  writefile << "-----Preferences-----" << endl;
  
  //Targets
  for (identifier = TARVIDEO; identifier <= TARSUB; identifier++)
  {
    writefile << "-----START-TARGETS-----" << endl;
    writefile << vec_man_.getVectorLen(identifier) << endl;
    writePrefs(writefile, identifier);
    writefile << "------END-TARGETS------" << endl;
  }

  //Rules
  for (identifier = SRCVIDEO; identifier <= SRCSUB; identifier++)
  {
    writefile << "------START-RULES------" << endl;
    writefile << vec_man_.getVectorLen(identifier) << endl;
    writePrefs(writefile, identifier);
    writefile << "-------END-RULES-------" << endl;
  }
  
  //End of file
  writefile << "-----Preferences-----" << endl;
  writefile.close();
  
  return 0;
}

string FileManager::checkPathToConfig()
{
  string config_file = setting_.getSettingsParam(SettingsMode::CONFIGLOC);

  return config_file;
}

void FileManager::writePrefs(ofstream& writefile, unsigned int identifier)
{
  for(unsigned int priority = 1; priority <= vec_man_.getVectorLen(identifier); priority++)
  {
    for(unsigned int param_nr = 1; param_nr <= 5; param_nr++)
    {
      writefile << vec_man_.sendSinglePreference(priority, identifier, param_nr).c_str();
      writefile << "HEXT";
    }
    writefile << endl;
    if(identifier <= SRCSUB)
    {
      writefile << vec_man_.getTargetVectorLenFromSrc(identifier, priority) << endl;
      for(unsigned int tar_item = 1; tar_item <= vec_man_.getTargetVectorLenFromSrc(identifier, priority);
          tar_item++)
      {
        writefile << vec_man_.getTargetIDfromSource(priority, identifier, tar_item) << endl;
      }
    }
  }
}

void FileManager::readPreferences() throw (FileReadException, OpenFileException)
{
  ifstream readfile;
  string tmp_string;
  string filename = setting_.getSettingsParam(SettingsMode::CONFIGNAME);
  string config_file = setting_.getSettingsParam(SettingsMode::CONFIGLOC);
  
  config_file.append(filename);
  
  unsigned int tmp_count = 0;
  unsigned int identifier;
  //filename.append(".txt");
  
  if(readfile.is_open() == true)
    readfile.close();
  readfile.open(config_file.c_str());
  if (readfile.is_open() == false)
  {
    readfile.close();
    throw OpenFileException();
  }
    
  //Head
  getline(readfile, tmp_string, '\n');
  ReadFileError(readfile);
  if (tmp_string != "-----Preferences-----")
  {
    readfile.close();
    throw FileReadException();
  }

  //Targets
  for (identifier = TARVIDEO; identifier <= TARSUB; identifier++)
  {
    getline(readfile, tmp_string, '\n');
    ReadFileError(readfile);
    if (tmp_string != "-----START-TARGETS-----")
    {
      readfile.close();
      throw FileReadException();
    }
    
    getline(readfile, tmp_string, '\n');
    ReadFileError(readfile);
    if(stringToInt(tmp_string, tmp_count) == false)
    {
      readfile.close();
      throw FileReadException();
    }
    if(tmp_count != 0)
    {
          
      readPrefs(readfile, identifier, tmp_count);
      //getline(readfile, tmp_string, '\n');
    }
    getline(readfile, tmp_string, '\n');

    ReadFileError(readfile);
    if (tmp_string != "------END-TARGETS------")
    {
      readfile.close();
      throw FileReadException();
    }
  }

  //Rules
  for (identifier = SRCVIDEO; identifier <= SRCSUB; identifier++)
  {
    getline(readfile, tmp_string, '\n');
    ReadFileError(readfile);
    if (tmp_string != "------START-RULES------")
    {
      readfile.close();
      throw FileReadException();
    }

    getline(readfile, tmp_string, '\n');
    ReadFileError(readfile);
    if (stringToInt(tmp_string, tmp_count) == false)
    {
      readfile.close();
      throw FileReadException();
    }
    if (tmp_count != 0)
    {
      readPrefs(readfile, identifier, tmp_count);
    }
    getline(readfile, tmp_string, '\n');
    ReadFileError(readfile);
    if (tmp_string != "-------END-RULES-------")
    {
      readfile.close();
      throw FileReadException();
    }
  }
  
  readfile.close();
}

void FileManager::readPrefs(ifstream& readfile, unsigned int identifier, 
                            unsigned int tmp_count) throw(FileReadException)
{
  string tmp_string;
  string params[5] = {"HI"};
  unsigned int count_targets = 0;
  unsigned int target_id = 0;

  for (unsigned int priority = 1; priority <= tmp_count; priority++)
  {
    for (unsigned int param_nr = 1; param_nr <= 5; param_nr++)
    {
      getline(readfile, params[param_nr - 1], 'H');
      ReadFileError(readfile);
      if(param_nr == 5)
      {
        getline(readfile, tmp_string, '\n');
        ReadFileError(readfile);
        if (tmp_string != "EXT")
        {
          readfile.close();
          throw FileReadException();
        }
      } else
      {
        getline(readfile, tmp_string, 'T');
        ReadFileError(readfile);
        if (tmp_string != "EX")
        {
          readfile.close();
          throw FileReadException();
        }
      }
    }
    
    //in order to be compatible to older config files (versions <1.4.1), where
    //the parameter "audio channels" can have a label like stereo or 5.1, we 
    //convert the label to a number ...
    if (identifier == SRCAUDIO)
    {
    	if (params[1] == "stereo")
		  {
		  	params[1] = "2";
		  } else if (params[1] == "5.1")
		  {
		  	params[1] = "6";
		  }
    }

    vec_man_.saveToVector(params[0], params[1], params[2], params[3], params[4],
                          priority, identifier);
    if (identifier <= SRCSUB)
    {
      //getline(readfile, tmp_string, '\n');
      getline(readfile, tmp_string, '\n');
      ReadFileError(readfile);
      if (stringToInt(tmp_string, count_targets) == false)
      {
        throw FileReadException();
      }
      for (unsigned int tar_item = 1; tar_item <= count_targets; tar_item++)
      {
        getline(readfile, tmp_string, '\n');
        ReadFileError(readfile);
        if (stringToInt(tmp_string, target_id) == false)
        {
          throw FileReadException();
        }
        vec_man_.linkNewTarget(target_id, identifier, priority);
      }
    }
  }
}

bool FileManager::stringToInt(std::string& string_number,
                              unsigned int& value)
{
  std::istringstream stream(string_number);
  stream >> value;
  if(stream.fail() || ! stream.eof())
  {
    return false;
  }
  return true;
}

void FileManager::readImportantFiles(vector<string>& filenames) throw (FileReadException, OpenFileException)
{
  ifstream readfile;
  string tmp_filename = "start";
  
  readfile.open("/tmp/poisonXfileslist");
  if (readfile.is_open() == false)
  {
    readfile.close();
    throw OpenFileException();
  }
  while (tmp_filename != "")
  {
    getline(readfile, tmp_filename, '\n');
    filenames.push_back(tmp_filename);
  }
  filenames.pop_back();
  readfile.close();
}

void FileManager::readProperties(string filename, string& duration) throw (FileReadException, OpenFileException)
{
  ifstream readfile;
  string line;
  string outter_bitrate;
  string params[5] = {"poison"};
  
  readfile.open("/tmp/poisonXprobelist");
  if (readfile.is_open() == false)
  {
    readfile.close();
    throw OpenFileException();
  }
  
  do
  {
    getline(readfile, line, '\n');

    if (line.find("Duration", 0) != string::npos)
    {
      unsigned int duration_pos_beg = line.find_first_of(":") + 2;
      unsigned int duration_pos_end = line.find_first_of(",");
      string tmp_duration;

      tmp_duration = line.substr(duration_pos_beg, duration_pos_end - duration_pos_beg);
      if (line.find(".") != string::npos) //we don't need to compare fractions of seconds ...
      {
        tmp_duration = line.substr(0, line.find("."));
      }
      else
      {
        tmp_duration = line;
      }

      if (tmp_duration.empty() == false)
      {
        duration = tmp_duration;
      }
      
      //getting bitrate -> unfortunately not within the method readPropsVideo(...)
      outter_bitrate = line.substr(line.find("bitrate: ") + 9); //Note: this happens before we analyze any stream
    }
    else if (line.substr(0, 6) == "stream") //note: 6 is the length of "stream" -> if true, the line begins with "stream"
    {
      unsigned int position = line.find("codec_type=", 0);
      position = position + 11; //note: 11 is the length of "codec_type="
      unsigned int position_two = line.find("|", position);

      string type_of_stream = line.substr(position, position_two - position);

      if (type_of_stream == "video") //Important: we can not get the bitrate out of the "video" section -> it is analyzed above in the duration section!
      {
        position = filename.find_last_of(".");
        params[0] = filename.substr(position + 1, filename.length() - position);
        params[2] = outter_bitrate;
        readPropsVideo(line, params);
        
        analyze_.saveToVector(params[0], params[1], params[2], params[3],
                              params[4], 0, SRCVIDEO);
        
      } else if (type_of_stream == "audio")
      {
        readPropsAudio(line, params);
        analyze_.saveToVector(params[0], params[1], params[2], params[3],
                              params[4], 0, SRCAUDIO);

      } else if (type_of_stream == "subtitle")
      {
        readPropsSub(line, params[0], params[1]);
        params[2] = "OFF";
        params[3] = "OFF";
        params[4] = "OFF";
        analyze_.saveToVector(params[0], params[1], params[2], params[3],
                              params[4], 0, SRCSUB);
      }
    }
  } while (readfile.eof() == false);
  readfile.close();
}

void FileManager::readPropsVideo(string line, string (&params)[5])
{
  stringstream bitrate_ss;
  
  unsigned int bitrate_nr = 0;
  
  string width;
  string height;
  string fps_param;

  //codec:
  params[1] = processOrdinaryParameter(line, "codec_name");
  
  //Bitrate
  //Note: due to certain limitations, the bitrate is set in the method: readProperties()
  if(params[2].substr(params[2].length() - 4) == "kb/s")
  {
    stringToInt(params[2], bitrate_nr);
    bitrate_nr = bitrate_nr*1000;
    bitrate_ss << bitrate_nr;
    params[2] = bitrate_ss.str();
  }
  
  //resolution
  width = processOrdinaryParameter(line, "width");
  height = processOrdinaryParameter(line, "height");
  params[3] =  width + "x" + height;
  
  //Frames per second
  fps_param = processOrdinaryParameter(line, "avg_frame_rate");
  if (fps_param.find("/") != string::npos)
  {
  	stringstream fps_ss;
  	string fps_numerator = fps_param.substr(0, fps_param.find("/"));
  	string fps_denominator = fps_param.substr(fps_param.find("/") + 1);
  	float fps_numerator_d = atof(fps_numerator.c_str());
  	float fps_denominator_d = atof(fps_denominator.c_str());
  	float fps_d = fps_numerator_d / fps_denominator_d;
  	
  	float factor = pow(10.0, 4 - ceil(log10(fabs(fps_d))));
    fps_d = round(fps_d * factor) / factor;
    
    fps_ss << fps_d;
    fps_param = fps_ss.str();
  }
  
  params[4] = fps_param;
}

void FileManager::readPropsAudio(string line, string (&params)[5])
{
  //codec:
  params[0] = processOrdinaryParameter(line, "codec_name");
  
  //channels:
  params[1] = processOrdinaryParameter(line, "channels");
  
  //language:
  params[2] = processOrdinaryParameter(line, "tag:language");

  //bitrate:
  params[3] = processOrdinaryParameter(line, "bit_rate");
  
  //sample rate:
  params[4] = processOrdinaryParameter(line, "sample_rate");
}

void FileManager::readPropsSub(string line, string& param0,
                               string& param1)
{
  //codec:
  param0 = processOrdinaryParameter(line, "codec_name");
  
  //language:
  param1 = processOrdinaryParameter(line, "tag:language");
}

string FileManager::processOrdinaryParameter(string const& line, string const& c_param_name)
{
	unsigned int param_begin;
	unsigned int param_end;
	string param_name = c_param_name + "=";
	string param;
	
	param_begin = line.find(param_name, 0);
	if (param_begin != string::npos)
  {
  	param_begin = param_begin + param_name.length();
		param_end  = line.find("|", param_begin);
	
		param = line.substr(param_begin, param_end - param_begin);
		
		if (param == "N/A")
		{
			param = "none";
		}
  } else
  {
  	return "none";
  }
  
  return param;
}
