/* 
 * File:   FileManager.cpp
 * Author: CutePoisonX
 * 
 * Created on 08. Februar 2013, 09:29
 */

#include "FileManager.h"
#include "FileWriteException.h"
#include "VectorSourceManager.h"
#include "AnalyzeMedia.h"
#include "Settings.h"
#include <sstream>
#include <vector>
//#include<iostream>

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
  string tmp_string; //temporary informations to check if file is corrupt
   
  num_settings = setting_.getVectorLen();
  //Head
  readfile.open("/opt/etc/PoisonConvert_Settings"); // /opt/etc/Settings
  if (readfile.is_open() == false)
    throw OpenFileException();
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
    if (setting_.writeParam(tmp_setting, i) == -1)
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
  
  writefile.open("/opt/etc/PoisonConvert_Settings", ostream::out);
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
  string filename = setting_.getSettingsParam(CONFIGNAME);
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
  string config_file = setting_.getSettingsParam(CONFIGLOC);

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
  string filename = setting_.getSettingsParam(CONFIGNAME);
  string config_file = setting_.getSettingsParam(CONFIGLOC);
  
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
  
  readfile.open("/opt/tmp/poisonXfileslist");
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

void FileManager::readProperties(string filename) throw (FileReadException, OpenFileException)
{
  ifstream readfile;
  string line;
  string params[5] = {"poison"};
  
  unsigned int position = 0;
  unsigned int position_two = 0;
  unsigned int position_video = 0;
  
  readfile.open("/opt/tmp/poisonXprobelist");
  if (readfile.is_open() == false)
  {
    readfile.close();
    throw OpenFileException();
  }
  
  do
  {
    getline(readfile, line, '\n');
    if (line.find("Stream", 0) != string::npos)
    {
      position = line.find("): ", 0);
      position = position + 3;
      position_two = line.find(":", position);

      position_video = line.find("Video:", 0) + 4; //Going to the Codec param (important for video-> no language needed)

      if (line.find("Video") != string::npos)
      {
        position = filename.find_last_of(".");
        params[0] = filename.substr(position + 1, filename.length() - position);
        readPropsVideo(line, position_video + 3, params);
        analyze_.saveToVector(params[0], params[1], params[2], params[3],
                              params[4], 0, SRCVIDEO);
//        for(unsigned int nr = 0; nr < 5; nr++) //Test purposes only
//        {
//            cout << "param " << nr << " = " << params[nr] << endl;
//        }
        

      }else if (line.find("Audio") != string::npos)
      {
        position_two = line.find("(", 0);
        readPropsAudio(line, position_two, params);
        analyze_.saveToVector(params[0], params[1], params[2], params[3],
                              params[4], 0, SRCAUDIO);
//        for(unsigned int nr = 0; nr < 5; nr++) //Test purposes only
//        {
//            cout << "param " << nr << " = " << params[nr] << endl;
//        }
      } else if (line.find("Subtitle") != string::npos)
      {
        position_two = line.find("(", 0);
        readPropsSub(line, position_two, params[0], params[1]);
      //params[1] = "OFF";
        params[2] = "OFF";
        params[3] = "OFF";
        params[4] = "OFF";
        analyze_.saveToVector(params[0], params[1], params[2], params[3],
                              params[4], 0, SRCSUB);
//        for(unsigned int nr = 0; nr < 5; nr++) //Test purposes only
//        {
//            cout << "param " << nr << " = " << params[nr] << endl;
//        }
      }
    }
  } while (readfile.eof() == false);
  readfile.close();
}

void FileManager::readPropsVideo(string line, unsigned int pos_start,
                                 string (&params)[5])
{
  stringstream ss;
  unsigned int position = 0;
  unsigned int position_two = 0;
  unsigned int position_semicolon = 0;
  
  unsigned int bitrate_nr = 0;

  line.erase(0, pos_start);

  //codec:
  position = line.find(" ", 0);
  position_semicolon = line.find(",", 0);
  params[1] = line.substr(0, position);
  line.erase(0, position_semicolon + 2);
  
  //Now the beginning of the string containes for example: yuv420p, this is not relevant therefore we skip this information...:
  position_semicolon = line.find(",", 0);
  line.erase(0, position_semicolon + 2);
  
  //resolution
  position = line.find(" ", 0);
  position_semicolon = line.find(",", 0);
  params[3] = line.substr(0, position);
  line.erase(0, position_semicolon + 2);
  
  //Bitrate
  position = line.find(" ", 0);
  position_two = line.find(" ", position);
  position_semicolon = line.find(",", 0);
  params[2] = line.substr(0, position);
  
  if(line.substr(position + 1, position_two - 1) == "kb/s")
  {
    stringToInt(params[2], bitrate_nr);
    bitrate_nr = bitrate_nr*1000;
    ss << bitrate_nr;
    params[2] = ss.str();
  }
  line.erase(0, position_semicolon + 2);
  
  //Frames per second
  position = line.find(" ", 0);
  position_semicolon = line.find(",", 0);
  params[4] = line.substr(0, position);
  line.erase(0, position_semicolon + 2);
}

void FileManager::readPropsAudio(string line, unsigned int pos_start, string (&params)[5])
{
  stringstream ss;
  unsigned int position = 0;
  unsigned int position_two = 0;
  unsigned int position_semicolon = 0;

  unsigned int bitrate_nr = 0;

  line.erase(0, pos_start + 1);

  //Language:
  position = line.find(")", 0);
  params[2] = line.substr(0, position);
  line.erase(0, params[2].length() + 10);

  //Codec
  position = line.find(" ", 0);
  position_semicolon = line.find(",", 0);
  params[0] = line.substr(0, position);
  if(params[0].compare(params[0].length() - 1, 1, ",") == 0)
  {
      params[0].erase(params[0].length() - 1, 1);
  }
  line.erase(0, position_semicolon + 2);

  //Sample Rate
  position = line.find(" ", 0);
  position_semicolon = line.find(",", 0);
  params[4] = line.substr(0, position);
  line.erase(0, position_semicolon + 2);

  //Channels
  position = line.find(" ", 0);
  position_semicolon = line.find(",", 0);
  params[1] = line.substr(0, position);
  line.erase(0, position_semicolon + 2);

 if(params[1].find("(side)",0) != string::npos)
 {
   position_two = params[1].find("(");
   params[1].erase(position_two, position_two + 5);
 }
  else if(params[1].compare("stereo,") == 0)
 {
   params[1] = "stereo";
 }

  //Now the beginning of the string containes for example: fltp, this is not relevant therefore we skip this information...:
  position_semicolon = line.find(",", 0);
  line.erase(0, position_semicolon + 2);

  //Bitrate
  position = line.find(" ", 0);
  position_two = line.find('\n', position);
  position_semicolon = line.find(",", 0);
  params[3] = line.substr(0, position);

  if (line.substr(position + 1, position_two - 1) == "kb/s")
  {
    stringToInt(params[3], bitrate_nr);
    bitrate_nr = bitrate_nr * 1000;
    ss << bitrate_nr;
    params[3] = ss.str();
  }
  line.erase(0, position_semicolon + 2);
}

void FileManager::readPropsSub(string line, unsigned int pos_start, string& param0,
                               string& param1)
{
  unsigned int position = 0;
  unsigned int second_space = 0;
  
  line.erase(0, pos_start + 1);

  //Language:
  position = line.find(")", 0);
  param1 = line.substr(0, position);
  line.erase(0, param1.length() + 13);

  //Codec
  position = line.find(" ", 0);
  second_space = line.find(" ", position);
  param0 = line.substr(0, position);
  line.erase(0, second_space + 2);
}
