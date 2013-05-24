/* 
 * File:   ConfigMode.cpp
 * Author: CutePoisonX
 * 
 * Created on 04. Februar 2013, 14:34
 */

#include "ConfigMode.h"
#include "UserInterface.h"
#include "VectorSourceManager.h"
#include "StreamPreference.h"
#include <string>
#include <vector>

using namespace std;

ConfigMode::ConfigMode(UserInterface& ui, VectorSourceManager& man)
: Mode(ui), man_(man), priority_(0), identifier_(0)
{
}

ConfigMode::ConfigMode(const ConfigMode& orig)
: Mode(orig), man_(orig.man_)
{
}

ConfigMode::~ConfigMode()
{
}

int ConfigMode::executeCommand()
{
  bool endthis = false;
  bool going_back = true;
  
  do
  {
    if (going_back == true)
    {
      ui_.writeString("", true);
      ui_.writeString("Configuration Mode!", true, "red");
      ui_.writeString("  ["); ui_.writeString("new", false, "yellow");
      ui_.writeString("]    - create new configuration", true);
      ui_.writeString("  [");
      ui_.writeString("list", false, "yellow");
      ui_.writeString("]   - list all configurations", true);
      ui_.writeString("  [");
      ui_.writeString("delete", false, "yellow");
      ui_.writeString("] - delete configurations", true);
      ui_.writeString("  [");
      ui_.writeString("exit", false, "yellow");
      ui_.writeString("]   - go to main screen", true);
      ui_.writeString("", true);
      going_back = false;
      
    }
    
    input_ = ui_.readStringNoCapitalize();
    if (input_ == "new")
    {
      newConfig();
      going_back = true;
    } else if (input_ == "list")
    {
      list();
      going_back = true;
    } else if (input_ == "delete")
    {
      deleteConfig();
      going_back = true;
    } else if (input_ == "exit")
    {
      return 0;
    } else
    {
      going_back = false;
      ui_.writeString("Please enter one of the options listed above (without ");
      ui_.writeString("the []).", true);
      endthis = false;
    }

  } while (endthis == false);

}

int ConfigMode::newConfig()
{
  bool endthis = false;
  bool going_back = true;
  
  do
  {
    if (going_back == true)
    {
      ui_.writeString("", true);
      ui_.writeString("Creating a new preference ...", true, "red");
      ui_.writeString("  Do you want to create an output-stream or a new rule?", true);
      ui_.writeString("  [");
      ui_.writeString("rule", false, "yellow");
      ui_.writeString("]   - create new rule", true); //OK
      ui_.writeString("  [");
      ui_.writeString("target", false, "yellow");
      ui_.writeString("] - create new output-stream", true);
      ui_.writeString("  [");
      ui_.writeString("exit", false, "yellow");
      ui_.writeString("]   - go back", true);
      ui_.writeString("", true);
      going_back = false;
      
    }
    input_ = ui_.readStringNoCapitalize();
    if (input_ == "rule")
    {
      if (createRule() == 0)
      {
        return 0;
      } else
      {
        going_back = true;
        endthis = false;
      }
    } else if (input_ == "target")
    {
      if (createOutput() == 0)
      {
        return 0;
      } else
      {
        going_back = true;
        endthis = false;
      }    
    } else if (input_ == "exit")
    {
      return -1;
    } else
    {
      going_back = false;
      ui_.writeString("Please enter one of the options listed above (without ");
      ui_.writeString("the []).", true);
      endthis = false;
    }
  } while (endthis == false);
}

int ConfigMode::createRule()
{
  bool going_back = true;
  bool end_inner = false;
  bool end_outher = false;
  unsigned int loops = 0;

  string prompt1;
  string prompt2;
  string prompt3;
  string prompt4;
  string prompt5;

  if (man_.getVectorLen(TARVIDEO) == 0 && man_.getVectorLen(TARAUDIO) == 0 &&
      man_.getVectorLen(TARSUB) == 0)
  {
    ui_.writeString("You have to create an output-stream first!", true, "green");
    ui_.readString(false);
  } else
  {
    do
    {
      do
      {
        if (going_back == true)
        {
          ui_.writeString("", true);
          ui_.writeString("Creating new rule ...", true, "red");
          ui_.writeString("  Please select the stream-type of the source:", true);
          ui_.writeString("  [");
          ui_.writeString("video", false, "yellow");
          ui_.writeString("]  - rule for a video-stream", true); ///OK
          ui_.writeString("  [");
          ui_.writeString("audio", false, "yellow");
          ui_.writeString("]  - rule for an audio-stream", true); //OK
          ui_.writeString("  [");
          ui_.writeString("sub", false, "yellow");
          ui_.writeString("]    - rule for a subtitle-stream", true); //OK
          ui_.writeString("  [");
          ui_.writeString("exit", false, "yellow");
          ui_.writeString("]   - go back", true); //OK
          ui_.writeString("", true);
        }
        input_ = ui_.readStringNoCapitalize();
        if (input_ == "video")
        {
          if (man_.getVectorLen(TARVIDEO) == 0)
          {
            ui_.writeString("You have to create a video-output-stream first!", true, "green");
            ui_.readString(false);
            going_back = true;
          } else
          {
            prompt1 = "Container";
            prompt2 = "Codec";
            prompt3 = "Bitrate";
            prompt4 = "Resolution";
            prompt5 = "Fps";
            identifier_ = SRCVIDEO;
            standartRuleParameterInputPrompt();
            createRuleVideo();
            setPriority();
            end_inner = true;
            going_back = true;
          }
        } else if (input_ == "sub")
        {
          if (man_.getVectorLen(TARSUB) == 0)
          {
            ui_.writeString("You have to create a subtitle-output-stream first!", true, "green");
            ui_.readString(false);
            going_back = true;
          } else
          {
            prompt1 = "Codec";
            prompt2 = "-";
            prompt3 = "-";
            prompt4 = "-";
            prompt5 = "-";
            identifier_ = SRCSUB;
            standartRuleParameterInputPrompt();
            createRuleSub();
            setPriority();
            end_inner = true;
            going_back = true;
          }
        } else if (input_ == "audio")
        {
          if (man_.getVectorLen(TARAUDIO) == 0)
          {
            ui_.writeString("You have to create an audio-output-stream first!", true, "green");
            ui_.readString(false);
            going_back = true;
          } else
          {
            prompt1 = "Codec";
            prompt2 = "Channels";
            prompt3 = "Language";
            prompt4 = "Bitrate";
            prompt5 = "Sample Rate";
            identifier_ = SRCAUDIO;
            standartRuleParameterInputPrompt();
            createRuleAudio();
            setPriority();
            end_inner = true;
            going_back = true;
          }
        } else if (input_ == "exit")
        {
          return -1;
        } else
        {
          ui_.writeString("Please enter one of the options listed above (without ");
          ui_.writeString("the []).", true);
          end_inner = false;
        }
      } while (end_inner == false);

      man_.saveToVector(param1_, param2_, param3_, param4_, param5_, priority_,
                        identifier_);
      loops = specifyTarget(prompt1, prompt2, prompt3, prompt4, prompt5);
      showRuleSummary(prompt1, prompt2, prompt3, prompt4, prompt5, loops);
      end_outher = proceedConfirmation();
    } while (end_outher == false);
    return 0;
  }
}

int ConfigMode::createRuleVideo()
{

  bool container_not = false;
  bool codec_not = false;
  bool bitrate_not = false;
  bool resolution_not = false;
  bool fps_not = false;

  unsigned int tmp_number = 0;
  string res_width;
  string res_height;

  bool overjump_res = false;

  do
  {
    ui_.writeString("Container: ");
    param1_ = ui_.readStringNoCapitalize(false);
    if (param1_.compare(0, 3, "not") == 0)
    {
      container_not = true;
      ui_.writeString("You can not place a 'not' before a container.", true);
    }
    else
    {
      container_not = false;
    }
    if (param1_ == "")
      ui_.writeString("Please enter something.", true);
  } while (param1_ == "" || container_not == true);

  do
  {
    ui_.writeString("Codec: ");
    param2_ = ui_.readStringNoCapitalize(false);
    if (param2_.compare(0, 3, "not") == 0)
    {
      codec_not = true;
      param2_.erase(0, 3);
    }
    if (param2_ == "")
      ui_.writeString("Please enter something.", true);
  } while (param2_ == "");
  
  do
  {
    ui_.writeString("Bitrate (b/s): ");
    param3_ = ui_.readStringNoCapitalize(false);
    if (param3_.compare(0, 3, "not") == 0)
    {
      bitrate_not = true;
      param3_.erase(0, 3);
    }
    if (ui_.stringToUnsignedInt(param3_, tmp_number) == false &&
        param3_ != "-")
    {
      ui_.writeString("You have to enter a number or '-'", true);
    }
  } while (ui_.stringToUnsignedInt(param3_, tmp_number) == false &&
           param3_ != "-");
  do
  {
    ui_.writeString("Resolution: Width ");
    res_width = ui_.readStringNoCapitalize(false);
    if (res_width.compare(0, 3, "not") == 0)
    {
      resolution_not = true;
      res_width.erase(0, 3);
    }
    if (ui_.stringToUnsignedInt(res_width, tmp_number) == false &&
        res_width != "-")
    {
      ui_.writeString("You have to enter a number or '-'", true);
    }
    if (res_width == "-")
    {
      overjump_res = true;
    }
  } while (ui_.stringToUnsignedInt(res_width, tmp_number) == false &&
           res_width != "-");
  if (overjump_res == false)
  {
    do
    {
      ui_.writeString("            Height ");
      res_height = ui_.readStringNoCapitalize(false);
      if (ui_.stringToUnsignedInt(res_height, tmp_number) == false &&
          res_height != "-")
      {
        ui_.writeString("You have to enter a number or '-'", true);
      }
    } while (ui_.stringToUnsignedInt(res_height, tmp_number) == false &&
             res_height != "-");
  }
  if (res_height != "-" && res_width != "-")
  {
    param4_ = res_width;
    param4_.append("x");
    param4_.append(res_height);
  } else
  {
    param4_ = "-";
  }

  do
  {
    ui_.writeString("Frames per second: ");
    param5_ = ui_.readStringNoCapitalize(false);
    if (param5_.compare(0, 3, "not") == 0)
    {
      fps_not = true;
      param5_.erase(0, 3);
    }
    if (param5_ == "")
      ui_.writeString("Please enter something.", true);
  } while (param5_ == "");
  
  insertNots(container_not, codec_not, bitrate_not, resolution_not, fps_not);

}

int ConfigMode::createRuleAudio()
{

  bool codec_not = false;
  bool channels_not = false;
  bool language_not = false;
  bool bitrate_not = false;
  bool sample_rate_not = false;
  
  unsigned int tmp_number = 0;

  //Codec
  do
  {
    ui_.writeString("Codec: ");
    param1_ = ui_.readStringNoCapitalize(false);
    if (param1_.compare(0, 3, "not") == 0)
    {
      codec_not = true;
      param1_.erase(0, 3);
    }
    if (param1_ == "")
      ui_.writeString("Please enter something.", true);
  } while (param1_ == "");
  
  //Channels
  do
  {
    ui_.writeString("Channels: ");
    param2_ = ui_.readStringNoCapitalize(false);
    if (param2_.compare(0, 3, "not") == 0)
    {
      channels_not = true;
      param2_.erase(0, 3);
    }
    if (param2_ == "")
      ui_.writeString("Please enter something.", true);
  } while (param2_ == "");
  
  //Language
  do
  {
    ui_.writeString("Language: ");
    param3_ = ui_.readStringNoCapitalize(false);
    if (param3_.compare(0, 3, "not") == 0)
    {
      language_not = true;
      param3_.erase(0, 3);
    }
    if (param3_ == "")
      ui_.writeString("Please enter something.", true);
  } while (param3_ == "");
  
  //Bitrate
  do
  {
    ui_.writeString("Bitrate (b/s): ");
    param4_ = ui_.readStringNoCapitalize(false);
    if (param4_.compare(0, 3, "not") == 0)
    {
      bitrate_not = true;
      param4_.erase(0, 3);
    }
    if (ui_.stringToUnsignedInt(param4_, tmp_number) == false &&
        param4_ != "-")
    {
      ui_.writeString("You have to enter a number or '-'", true);
    }
  } while (ui_.stringToUnsignedInt(param4_, tmp_number) == false &&
           param4_ != "-");

  //Sample Rate
  do
  {
    ui_.writeString("Sample Rate (Hz): ");
    param5_ = ui_.readStringNoCapitalize(false);
    if (param5_.compare(0, 3, "not") == 0)
    {
      sample_rate_not = true;
      param5_.erase(0, 3);
    }
    if (ui_.stringToUnsignedInt(param5_, tmp_number) == false &&
        param5_ != "-")
    {
      ui_.writeString("You have to enter a number or '-'", true);
    }
  } while (ui_.stringToUnsignedInt(param5_, tmp_number) == false &&
           param5_ != "-");

  insertNots(codec_not, channels_not, language_not, bitrate_not, sample_rate_not);

}

 int ConfigMode::createRuleSub()
{
  bool codec_not = false;
  bool language_not = false;

  param3_ = "-";
  param4_ = "-";
  param5_ = "-";

    do
  {
    ui_.writeString("Codec: ");
    param1_ = ui_.readStringNoCapitalize(false);
    if (param1_.compare(0, 3, "not") == 0)
    {
      codec_not = true;
      param1_.erase(0, 3);
    }
    if (param1_ == "")
      ui_.writeString("Please enter something.", true);
  } while (param1_ == "");
  
  //Language
  do
  {
    ui_.writeString("Language: ");
    param2_ = ui_.readStringNoCapitalize(false);
    if (param2_.compare(0, 3, "not") == 0)
    {
      language_not = true;
      param2_.erase(0, 3);
    }
    if (param2_ == "")
      ui_.writeString("Please enter something.", true);
  } while (param2_ == "");
  
  insertNots(codec_not, language_not, false, false, false);
 }
 
unsigned int ConfigMode::setPriority()
{
  bool num_ok = false;

  ui_.writeString("", true);
  if (man_.getVectorLen(identifier_) != 0)
  {
    ui_.writeString("Which priority do you give this rule?", true);

    do
    {
      ui_.writeString("Enter a number between 1 and ");
      ui_.writeNumber(man_.getVectorLen(identifier_) + 1, true);

      input_ = ui_.readStringNoCapitalize();
      if (ui_.stringToUnsignedInt(input_, priority_) == true)
      {
        if (priority_ > 0)
          num_ok = true;
        if (priority_ > man_.getVectorLen(identifier_) + 1)
        {
          ui_.writeString("Warning: Priority to high! Converted priority to ");
          ui_.writeNumber(man_.getVectorLen(identifier_) + 1, true);
          priority_ = man_.getVectorLen(identifier_) + 1;
        }
      }
    } while (num_ok = false);
  } else
  {
    priority_ = 1;
  }
}

unsigned int ConfigMode::specifyTarget(const string& param1, const string& param2,
                                       const string& param3, const string& param4,
                                       const string& param5)
{
  unsigned int target_identifier = 0;
  unsigned int loops = 0;
  unsigned int target_id;
  bool num_ok = false;
  bool additional = false;

  string container = "TEMP";
  
  switch (identifier_)
  {
    case SRCVIDEO:
      target_identifier = TARVIDEO;
      break;
    case SRCAUDIO:
      target_identifier = TARAUDIO;
      break;
    case SRCSUB:
      target_identifier = TARSUB;
      break;
  }

  if (man_.getVectorLen(target_identifier) > 1)
  {
    ui_.writeString("To which stream do you want to convert?", true);
    for (int i = 1; i <= man_.getVectorLen(target_identifier); i++)
    {
      ui_.writeString("ID ");
      ui_.writeNumber(i);
      ui_.writeString(" -> "); ui_.writeString(param1); ui_.writeString(": ");
      ui_.writeString(man_.sendSinglePreference(i, target_identifier, 1));
      if(param2 != "-")
      {
        ui_.writeString(", ");
        ui_.writeString(param2);
        ui_.writeString(": ");
        ui_.writeString(man_.sendSinglePreference(i, target_identifier, 2));
      }
      if (param3 != "-")
      {
        ui_.writeString(", ");
        ui_.writeString(param3);
        ui_.writeString(": ");
        ui_.writeString(man_.sendSinglePreference(i, target_identifier, 3));
      }
      if (param4 != "-")
      {
        ui_.writeString(", ");
        ui_.writeString(param4);
        ui_.writeString(": ");
        ui_.writeString(man_.sendSinglePreference(i, target_identifier, 4));
      }
      if (param5 != "-")
      {
        ui_.writeString(", ");
        ui_.writeString(param5);
        ui_.writeString(": ");
        ui_.writeString(man_.sendSinglePreference(i, target_identifier, 5));
      }
      ui_.writeString("", true);
    }

    do
    {
      ui_.writeString("Select an ID between 1 and ");
      ui_.writeNumber(man_.getVectorLen(target_identifier), true);

      input_ = ui_.readStringNoCapitalize();
      if (ui_.stringToUnsignedInt(input_, target_id) == true)
      {

        if (target_id > 0 && target_id <= man_.getVectorLen(target_identifier))
        {
          if ((container == "TEMP" ||
              man_.sendSinglePreference(target_id, TARVIDEO, 1) == container)
              || identifier_ != SRCVIDEO)
          {
            num_ok = true;
            man_.linkNewTarget(target_id, identifier_, priority_);
            if (container == "TEMP" && identifier_ == SRCVIDEO)
              container = man_.sendSinglePreference(target_id, TARVIDEO, 1);
            loops++;
          } else
          {
            ui_.writeString("You can not link this ID because the parameters 'container' seems to differ. ", false, "yellow");
            ui_.writeString("A video can only have one container.", true, "yellow");
            ui_.readString(false);
          }
          ui_.writeString("Do you want to link an additional one ([y/n]) ?", true);
          ui_.writeString("(You can also link the same one again)", true);
          do
          {
            input_ = ui_.readStringNoCapitalize();
            if (input_ == "y")
            {
              additional = true;
            } else if (input_ != "n")
            {
              ui_.writeString("Plaese enter [y] for 'yes' and [n] for 'no'!", true);
            } else
            {
              additional = false;
            }
          } while (input_ != "y" && input_ != "n");
        }

      }
    } while (num_ok == false || additional == true);
  } else
  {
    ui_.writeString("You only specified one target, so this stream ");
    ui_.writeString("will be converted to this one:", true);

    ui_.writeString("ID 1");
    ui_.writeString(" -> "); ui_.writeString(param1); ui_.writeString(": ");
    ui_.writeString(man_.sendSinglePreference(1, target_identifier, 1));
    if(param2 != "-")
    {
      ui_.writeString(", "); ui_.writeString(param2); ui_.writeString(": ");
      ui_.writeString(man_.sendSinglePreference(1, target_identifier, 2));
    }
    if(param3 != "-")
    {
      ui_.writeString(", "); ui_.writeString(param3); ui_.writeString(": ");
      ui_.writeString(man_.sendSinglePreference(1, target_identifier, 3));
    }
    if(param4 != "-")
    {
      ui_.writeString(", "); ui_.writeString(param4); ui_.writeString(": ");
      ui_.writeString(man_.sendSinglePreference(1, target_identifier, 4));
    }
    if(param5 != "-")
    {
      ui_.writeString(", "); ui_.writeString(param5); ui_.writeString(": ");
      ui_.writeString(man_.sendSinglePreference(1, target_identifier, 5));
    }
    ui_.writeString("", true);
    ui_.writeString("", true);
    loops = 1;
    man_.linkNewTarget(1, identifier_, priority_);
  }
  return loops;
}

void ConfigMode::standartRuleParameterInputPrompt()
{
  ui_.writeString("Please select parameters!", true, "green");
  ui_.writeString("(If you write 'not' before a parameter, this rule will ");
  ui_.writeString("only be executed, if this parameter doesnt apply.", true);
  ui_.writeString("If you write a '-' for a parameter, this value will be ");
  ui_.writeString("ignored.", true);
}

void ConfigMode::insertNots(bool param1_not, bool param2_not, bool param3_not,
                            bool param4_not, bool param5_not)
{
  if (param1_not == true)
    param1_.insert(0, "NOT");
  if (param2_not == true)
    param2_.insert(0, "NOT");
  if (param3_not == true)
    param3_.insert(0, "NOT");
  if (param4_not == true)
    param4_.insert(0, "NOT");
  if (param5_not == true)
    param5_.insert(0, "NOT");  
}

void ConfigMode::showRuleSummary(const string& param1, const string& param2,
                                 const string& param3, const string& param4,
                                 const string& param5, unsigned int quantity_targets)
{
  unsigned int target_count;
  unsigned int target_identifier;
  
  ui_.writeString("", true);
  ui_.writeString("Summary: A stream matching:", true, "green");

  if (param1_ != "-")
  {
    ui_.writeString("  "); ui_.writeString(param1);
    if(param1_.compare(0,3,"NOT") == 0)
    {
      ui_.writeString(" is not ");
      ui_.writeString(param1_.erase(0,3), true);
    }
    else
    {
      ui_.writeString(" is ");
      ui_.writeString(param1_, true);
    } 
  }
  if (param2_ != "-")
  {
    ui_.writeString("  "); ui_.writeString(param2);
    if(param2_.compare(0,3,"NOT") == 0)
    {
      ui_.writeString(" is not ");
      ui_.writeString(param2_.erase(0,3), true);
    }
    else
    {
      ui_.writeString(" is ");
      ui_.writeString(param2_, true);
    }
  }
  if (param3_ != "-")
  {
    ui_.writeString("  "); ui_.writeString(param3);
    if(param3_.compare(0,3,"NOT") == 0)
    {
      ui_.writeString(" is not ");
      ui_.writeString(param3_.erase(0,3), true);
    }
    else
    {
      ui_.writeString(" is ");
      ui_.writeString(param3_, true);
    }
  }
  if (param4_ != "-")
  {
    ui_.writeString("  "); ui_.writeString(param4);
    if(param4_.compare(0,3,"NOT") == 0)
    {
      ui_.writeString(" is not ");
      ui_.writeString(param4_.erase(0,3), true);
    }
    else
    {
      ui_.writeString(" is ");
      ui_.writeString(param4_, true);
    } 
  }
  if (param5_ != "-")
  {
    ui_.writeString("  "); ui_.writeString(param5);
    if(param5_.compare(0,3,"NOT") == 0)
    {
      ui_.writeString(" is not ");
      ui_.writeString(param5_.erase(0,3), true);
    }
    else
    {
      ui_.writeString(" is ");
      ui_.writeString(param5_, true);
    }
  }
  if(param1_ == "-" && param2_ == "-" && param3_ == "-" && param4_ == "-"
     && param5_ == "-")
  {
    if(identifier_ == SRCVIDEO)
      ui_.writeString("Any video-stream ");
    if(identifier_ == SRCAUDIO)
      ui_.writeString("Any audio-stream ");
    if(identifier_ == SRCSUB)
      ui_.writeString("Any subtitle-stream ");
  }
  ui_.writeString("  (priority is ");
  ui_.writeNumber(priority_);
  ui_.writeString(") will be converted to: ", true);

  switch (identifier_)
  {
    case SRCVIDEO:
      target_identifier = TARVIDEO;
      break;
    case SRCAUDIO:
      target_identifier = TARAUDIO;
      break;
    case SRCSUB:
      target_identifier = TARSUB;
      break;
  }

  for (unsigned int i = 1; i <= quantity_targets; i++)
  {
    target_count = man_.getTargetIDfromSource(priority_, identifier_, i);
    ui_.writeString("  ");
    ui_.writeNumber(i);
    ui_.writeString(") "); ui_.writeString(param1); ui_.writeString(": ");
    if(man_.sendSinglePreference(target_count, target_identifier, 1) != "-")
    {
      ui_.writeString(man_.sendSinglePreference(target_count, target_identifier, 1));
    }
    else
    {
      ui_.writeString("unchanged");
    }
    if (param2 != "-")
    {
      ui_.writeString(", "); ui_.writeString(param2); ui_.writeString(": ");
      if (man_.sendSinglePreference(target_count, target_identifier, 2) != "-")
      {
        ui_.writeString(man_.sendSinglePreference(target_count, target_identifier, 2));
      } else
      {
        ui_.writeString("unchanged");
      }
    }
    if(param3 != "-")
    {
      ui_.writeString(", "); ui_.writeString(param3); ui_.writeString(": ");
      if (man_.sendSinglePreference(target_count, target_identifier, 3) != "-")
      {
        ui_.writeString(man_.sendSinglePreference(target_count, target_identifier, 3));
      } else
      {
        ui_.writeString("unchanged");
      }
    }
    if(param4 != "-")
    {
      ui_.writeString(", "); ui_.writeString(param4); ui_.writeString(": ");
      if (man_.sendSinglePreference(target_count, target_identifier, 4) != "-")
      {
        ui_.writeString(man_.sendSinglePreference(target_count, target_identifier, 4));
      } else
      {
        ui_.writeString("unchanged");
      }
    }
    if(param5 != "-")
    {
      ui_.writeString(", "); ui_.writeString(param5); ui_.writeString(": ");
      if (man_.sendSinglePreference(target_count, target_identifier, 5) != "-")
      {
        ui_.writeString(man_.sendSinglePreference(target_count, target_identifier, 5));
      } else
      {
        ui_.writeString("unchanged");
      }
    }
    ui_.writeString("", true);

  }
}

bool ConfigMode::proceedConfirmation()
{
  do
  {
    ui_.writeString("Do you wish to proceed ([y/n]) ?", true, "yellow");
    input_ = ui_.readStringNoCapitalize();
    if (input_ == "y")
    {
      ui_.writeString("Successfully created!", true);
      return true;
    } else if (input_ != "n")
    {
      ui_.writeString("Plaese enter [y] for 'yes' and [n] for 'no'!", true);
    }
  } while (input_ != "n");
  
  man_.deleteFromVector(priority_, identifier_);
  return false;
}

int ConfigMode::createOutput()
{ 
  bool end_inner = false;
  bool end_outher = false;
  bool going_back = true;

  do
  {
    do
    {
      if (going_back == true)
      {
        ui_.writeString("", true);
        ui_.writeString("Creating new output-stream ...", true, "red");
        ui_.writeString("  Which stream do you want to add?", true);
        ui_.writeString("  [");
        ui_.writeString("video", false, "yellow");
        ui_.writeString("]  - add a video-stream", true);
        ui_.writeString("  [");
        ui_.writeString("audio", false, "yellow");
        ui_.writeString("]  - add an audio-stream", true);
        ui_.writeString("  [");
        ui_.writeString("sub", false, "yellow");
        ui_.writeString("]    - add a subtitle-stream", true);
        ui_.writeString("  [");
        ui_.writeString("exit", false, "yellow");
        ui_.writeString("]   - go back", true);
        ui_.writeString("", true);
        going_back = false;
      }
      input_ = ui_.readStringNoCapitalize();
      if (input_ == "video")
      {
        identifier_ = TARVIDEO;
        standartTargetParameterInputPrompt();
        createOutputVideo();
        showTargetSummary("Container", "Codec", "Bitrate", "Resolution", "Fps");
        end_inner = true;
        going_back = true;
      } else if (input_ == "audio")
      {
        identifier_ = TARAUDIO;
        standartTargetParameterInputPrompt();
        createOutputAudio();
        showTargetSummary("Codec", "Channels", "Language", "Bitrate", "Sample Rate");
        end_inner = true;
        going_back = true;
      } else if (input_ == "sub")
      {
        identifier_ = TARSUB;
        standartTargetParameterInputPrompt();
        createOutputSub();
        //Own Summmary
        ui_.writeString("  Codec:");
        if (param1_ == "copy")
        {
          ui_.writeString(" will be copied", true);
        } else
        {
          ui_.writeString(" will be ");
          ui_.writeString(param1_, true);
        }
        //Own Summary stop
        end_inner = true;
        going_back = true;
      } else if (input_ == "exit")
      {
        return -1;
      } else
      {
        ui_.writeString("Please enter one of the options listed above (without ");
        ui_.writeString("the []).", true);
        end_inner = false;
      }
    } while (end_inner == false);
    priority_ = man_.getVectorLen(identifier_) + 1;
    man_.saveToVector(param1_, param2_, param3_, param4_, param5_, priority_,
                      identifier_);
    
    end_outher = proceedConfirmation();
  } while (end_outher == false);
  return 0;
}

int ConfigMode::createOutputVideo()
{
  unsigned int tmp_number = 0;
  string res_width;
  string res_height;
  bool overjump_res = false;
  
  do
  {
    ui_.writeString("Container: ");
    param1_ = ui_.readStringNoCapitalize(false);
    if(param1_ == "")
      ui_.writeString("Please enter something.", true);
  } while (param1_ == "");

  do
  {
    ui_.writeString("Codec: ");
    param2_ = ui_.readStringNoCapitalize(false);
    if (param2_ == "")
      ui_.writeString("Please enter something.", true);
  } while (param2_ == "");
  
  if (param2_ != "copy")
  {
    do
    {
      ui_.writeString("Bitrate (b/s): ");
      param3_ = ui_.readStringNoCapitalize(false);
      
      if (ui_.stringToUnsignedInt(param3_, tmp_number) == false &&
          param3_ != "-" && param3_ != "copy")
      {
        ui_.writeString("You have to enter a number or '-'", true);
      }
    } while (ui_.stringToUnsignedInt(param3_, tmp_number) == false &&
             param3_ != "-" && param3_ != "copy");
    do
    {
      ui_.writeString("Resolution: Width ");
      res_width = ui_.readStringNoCapitalize(false);

      if (ui_.stringToUnsignedInt(res_width, tmp_number) == false &&
          res_width != "-" && res_width != "copy")
      {
        ui_.writeString("You have to enter a number or '-'", true);
      }
      if (res_width == "-" || res_width == "copy")
      {
        overjump_res = true;
      }
    } while (ui_.stringToUnsignedInt(res_width, tmp_number) == false &&
             res_width != "-" && res_width != "copy");
    if (overjump_res == false)
    {
      do
      {
        ui_.writeString("            Height ");
        res_height = ui_.readStringNoCapitalize(false);
        if (ui_.stringToUnsignedInt(res_height, tmp_number) == false &&
            res_height != "-" && res_height != "copy")
        {
          ui_.writeString("You have to enter a number or '-'", true);
        }
      } while (ui_.stringToUnsignedInt(res_height, tmp_number) == false &&
               res_height != "-" && res_height != "copy");
    }
    if (res_height == "-" || res_width == "-")
    {
      param4_ = "-";
    } else if(res_height == "copy" || res_width == "copy")
    {
      param4_ = "copy";
    } else
    {
      param4_ = res_width;
      param4_.append("x");
      param4_.append(res_height);
    }

    do
    {
      ui_.writeString("Frames per second: ");
      param5_ = ui_.readStringNoCapitalize(false);
      if (param5_ == "")
        ui_.writeString("Please enter something.", true);
    } while (param5_ == "");
    
  } else
  {
    param3_ = "-";
    param4_ = "-";
    param5_ = "-";
  }
  if (param1_ == "-" && param2_ == "-" && param3_ == "-" && param4_ == "-"
      && param5_ == "-")
  {
    param2_ = "copy";
  }

}

int ConfigMode::createOutputAudio()
{
  unsigned int tmp_number = 0;

  //Codec
  do
  {
    ui_.writeString("Codec: ");
    param1_ = ui_.readStringNoCapitalize(false);
    if (param1_ == "")
      ui_.writeString("Please enter something.", true);
  } while (param1_ == "");
  
  if (param1_ != "copy")
  {
    //Channels
    do
    {
      ui_.writeString("Channels: ");
      param2_ = ui_.readStringNoCapitalize(false);
      if (param2_ == "")
        ui_.writeString("Please enter something.", true);
    } while (param2_ == "");
  
    //Language
    do
    {
      ui_.writeString("Language: ");
      param3_ = ui_.readStringNoCapitalize(false);
      if (param3_ == "")
        ui_.writeString("Please enter something.", true);
    } while (param3_ == "");
  
    //Bitrate
    do
    {
      ui_.writeString("Bitrate (b/s): ");
      param4_ = ui_.readStringNoCapitalize(false);
      if (ui_.stringToUnsignedInt(param4_, tmp_number) == false &&
          param4_ != "-" && param4_ != "copy")
      {
        ui_.writeString("You have to enter a number or '-'", true);
      }
    } while (ui_.stringToUnsignedInt(param4_, tmp_number) == false &&
             param4_ != "-" && param4_ != "copy");

    //Sample Rate
    do
    {
      ui_.writeString("Sample Rate (Hz): ");
      param5_ = ui_.readStringNoCapitalize(false);
      if (ui_.stringToUnsignedInt(param5_, tmp_number) == false &&
          param5_ != "-" && param5_ != "copy")
      {
        ui_.writeString("You have to enter a number or '-'", true);
      }
    } while (ui_.stringToUnsignedInt(param5_, tmp_number) == false &&
             param5_ != "-" && param5_ != "copy");
  } else
  {
    param2_ = "-";
    param3_ = "-";
    param4_ = "-";
    param5_ = "-";
  }
  if (param1_ == "-" && param2_ == "-" && param3_ == "-" && param4_ == "-"
      && param5_ == "-")
  {
    param1_ = "copy";
  }
}

int ConfigMode::createOutputSub()
{

  param2_ = "OFF";
  param3_ = "OFF";
  param4_ = "OFF";
  param5_ = "OFF";

  //Copy or not? ///////////////////////////////Old Verrsion
//    do
//  {
//    ui_.writeString("Copy subtitle stream? [y/n]", true);
//    input_ = ui_.readStringNoCapitalize(false);
//    if (input_ == "y")
//    {
//      param1_ = "copy";
//    } else if (input_ == "n")
//    {
//      return -1;
//    } else
//    {
//      ui_.writeString("Plaese enter [y] for 'yes' and [n] for 'no'!", true);
//    }
//  } while (input_ != "y");

  do
  {
    ui_.writeString("Codec: ");
    param1_ = ui_.readStringNoCapitalize(false);
    if (param1_ == "")
      ui_.writeString("Please enter something.", true);
  } while (param1_ == "");
  
  return 0;
  
}

void ConfigMode::standartTargetParameterInputPrompt()
{
  ui_.writeString("Please select parameters!", true, "green");
  ui_.writeString("(If you write '-' for a parameter, this value will not ");
  ui_.writeString("be explicitly specified to ffmpeg (in most cases simply copied).",true);
  ui_.writeString("Note: if you write 'copy' for the codec, the ");
  ui_.writeString("whole stream will be copied.)");
  ui_.writeString("", true);
}

void ConfigMode::showTargetSummary(const string& param1, const string& param2,
                                   const string& param3, const string& param4,
                                   const string& param5)
{
  ui_.writeString("Summary:", true);

  ui_.writeString("  ");
  ui_.writeString(param1);
  if (param1_ == "-")
  {
    ui_.writeString(" will not be touched", true);
  } else if (param1_ == "copy")
  {
    ui_.writeString(" will be copied", true);
  } else
  {
    ui_.writeString(" will be ");
    ui_.writeString(param1_, true);
  }
  
  //param 2
  ui_.writeString("  ");
  ui_.writeString(param2);
  if(param2_ == "-")
  {
    ui_.writeString(" will not be touched", true);
  }
  else if(param2_ == "copy")
  {
    ui_.writeString(" will be copied", true);
  }
  else
  {
    ui_.writeString(" will be "); ui_.writeString(param2_, true);
  }
  
  //param3
  ui_.writeString("  ");
  ui_.writeString(param3);
  if(param3_ == "-")
  {
    ui_.writeString(" will not be touched", true);
  }
  else if(param3_ == "copy")
  {
    ui_.writeString(" will be copied", true);
  }
  else
  {
    ui_.writeString(" will be "); ui_.writeString(param3_, true);
  }
  
  //param4
  ui_.writeString("  ");
  ui_.writeString(param4);
  if(param4_ == "-")
  {
    ui_.writeString(" will not be touched", true);
  }
  else if(param4_ == "copy")
  {
    ui_.writeString(" will be copied", true);
  }
  else
  {
    ui_.writeString(" will be "); ui_.writeString(param4_, true);
  }
  
  //param5
  ui_.writeString("  ");
  ui_.writeString(param5);
  if(param5_ == "-")
  {
    ui_.writeString(" will not be touched", true);
  }
  else if(param5_ == "copy")
  {
    ui_.writeString(" will be copied", true);
  }
  else
  {
    ui_.writeString(" will be "); ui_.writeString(param5_, true);
  }
}

int ConfigMode::list()
{
  if (man_.getVectorLen(TARVIDEO) != 0 || man_.getVectorLen(TARAUDIO) != 0 ||
      man_.getVectorLen(TARSUB) != 0)
  {
    bool endthis = false;

    unsigned int tmp_vec_src_tar_len = 0;
    unsigned int tmp_vec_len = 0;
    unsigned int tmp_priority = 0;
    unsigned int tmp_identifier = 0;
    string disp_now;

    string param_prompt1 = "Container";
    string param_prompt2 = "Codec";
    string param_prompt3 = "Bitrate";
    string param_prompt4 = "Resolution";
    string param_prompt5 = "Fps";

    ui_.writeString("", true);
    ui_.writeString("List configurations ...", true, "red");
    ui_.writeString("  [");
    ui_.writeString("map", false, "yellow");
    ui_.writeString("]     - show stream map", true); //OK
    ui_.writeString("  [");
    ui_.writeString("rules", false, "yellow");
    ui_.writeString("]   - list all rules", true);
    ui_.writeString("  [");
    ui_.writeString("targets", false, "yellow");
    ui_.writeString("] - list all targets", true);
    ui_.writeString("  [");
    ui_.writeString("exit", false, "yellow");
    ui_.writeString("]    - go back", true);
    ui_.writeString("", true);

    do
    {
      input_ = ui_.readStringNoCapitalize();
      if (input_ == "map")
      {
        if (man_.getVectorLen(SRCVIDEO) != 0 || man_.getVectorLen(SRCAUDIO) != 0 ||
            man_.getVectorLen(SRCSUB) != 0)
        {
          identifier_ = SRCVIDEO;
          tmp_vec_len = man_.getVectorLen(SRCVIDEO);
          disp_now = "  VIDEO-RULES:";
          for (unsigned short pref_counter = 0; pref_counter < 3; pref_counter++)
          {
            if (tmp_vec_len > 0)
            {
              ui_.writeString(disp_now, true, "red");
              for (priority_ = 1; priority_ <= tmp_vec_len; priority_++)
              {
                ui_.writeString("  ");
                ui_.writeNumber(priority_, false, "yellow");
                ui_.writeString(") ", false, "yellow");
                listRule(param_prompt1, param_prompt2,
                         param_prompt3, param_prompt4,
                         param_prompt5);
                tmp_vec_src_tar_len = man_.getTargetVectorLenFromSrc(identifier_, priority_);
                tmp_identifier = identifier_;
                if (pref_counter == 0)
                {
                  identifier_ = TARVIDEO;
                } else if (pref_counter == 1)
                {
                  identifier_ = TARAUDIO;
                } else if (pref_counter == 2)
                {
                  identifier_ = TARSUB;
                }
                tmp_priority = priority_;
                for (unsigned int target_item = 1; target_item <= tmp_vec_src_tar_len; target_item++)
                {
                  priority_ = man_.getTargetIDfromSource(tmp_priority, tmp_identifier, target_item);
                  ui_.writeString("      -> ", false, "green");
                  ui_.writeNumber(priority_, false, "green");
                  ui_.writeString(") ", false, "green");
                  listTarget(param_prompt1, param_prompt2, param_prompt3, param_prompt4,
                             param_prompt5);
                }
                identifier_ = tmp_identifier;
                priority_ = tmp_priority;
              }
            }
            ui_.writeString("", true);
            if (pref_counter == 0)
            {
              tmp_vec_len = man_.getVectorLen(SRCAUDIO);
              identifier_ = SRCAUDIO;
              disp_now = "  AUDIO-RULES:";
              param_prompt1 = "Codec";
              param_prompt2 = "Channels";
              param_prompt3 = "Language";
              param_prompt4 = "Bitrate";
              param_prompt5 = "Sample Rate";
            }
            if (pref_counter == 1)
            {
              tmp_vec_len = man_.getVectorLen(SRCSUB);
              identifier_ = SRCSUB;
              disp_now = "  SUBTITLE-RULES:";
              param_prompt1 = "Codec";
              param_prompt2 = "Language";
              param_prompt3 = "OFF";
              param_prompt4 = "OFF";
              param_prompt5 = "OFF";
            }
          }
          input_ = ui_.readStringNoCapitalize(false);
          return 0;
        } else
        {
          ui_.writeString("You have to specify rules first.", true, "green");
        }
      } else if (input_ == "rules")
      {
        if (man_.getVectorLen(SRCVIDEO) != 0 || man_.getVectorLen(SRCAUDIO) != 0 ||
            man_.getVectorLen(SRCSUB) != 0)
        {
          identifier_ = SRCVIDEO;
          tmp_vec_len = man_.getVectorLen(SRCVIDEO);
          disp_now = "  VIDEO-RULES:";
          for (unsigned short pref_counter = 0; pref_counter < 3; pref_counter++)
          {
            if (tmp_vec_len > 0)
            {
              ui_.writeString(disp_now, true, "red");
              for (priority_ = 1; priority_ <= tmp_vec_len; priority_++)
              {
                ui_.writeString("    ");
                ui_.writeNumber(priority_, false, "yellow");
                ui_.writeString(") ", false, "yellow");
                listRule(param_prompt1, param_prompt2, param_prompt3, param_prompt4,
                         param_prompt5);
              }
            }
            ui_.writeString("", true);
            if (pref_counter == 0)
            {
              tmp_vec_len = man_.getVectorLen(SRCAUDIO);
              identifier_ = SRCAUDIO;
              disp_now = "  AUDIO-RULES:";
              param_prompt1 = "Codec";
              param_prompt2 = "Channels";
              param_prompt3 = "Language";
              param_prompt4 = "Bitrate";
              param_prompt5 = "Sample Rate";
            }
            if (pref_counter == 1)
            {
              tmp_vec_len = man_.getVectorLen(SRCSUB);
              identifier_ = SRCSUB;
              disp_now = "  SUBTITLE-RULES:";
              param_prompt1 = "Language";
              param_prompt2 = "OFF";
              param_prompt3 = "OFF";
              param_prompt4 = "OFF";
              param_prompt5 = "OFF";
            }
          }
          input_ = ui_.readStringNoCapitalize(false);
          return 0;
        } else
        {
          ui_.writeString("You have to specify rules first.", true, "green");
        }
      } else if (input_ == "targets")
      {
        identifier_ = TARVIDEO;
        tmp_vec_len = man_.getVectorLen(TARVIDEO);
        disp_now = "  VIDEO-TARGETS:";
        for (unsigned short pref_counter = 0; pref_counter < 3; pref_counter++)
        {
          if (tmp_vec_len > 0)
          {
            ui_.writeString(disp_now, true, "red");
            for (priority_ = 1; priority_ <= tmp_vec_len; priority_++)
            {
              ui_.writeString("    ");
              ui_.writeNumber(priority_, false, "yellow");
              ui_.writeString(") ", false, "yellow");
              listTarget(param_prompt1, param_prompt2, param_prompt3, param_prompt4,
                         param_prompt5);
            }
          }
          ui_.writeString("", true);
          if (pref_counter == 0)
          {
            tmp_vec_len = man_.getVectorLen(TARAUDIO);
            identifier_ = TARAUDIO;
            disp_now = "  AUDIO-TARGETS:";
            param_prompt1 = "Codec";
            param_prompt2 = "Channels";
            param_prompt3 = "Language";
            param_prompt4 = "Bitrate";
            param_prompt5 = "Sample Rate";
          }
          if (pref_counter == 1)
          {
            tmp_vec_len = man_.getVectorLen(TARSUB);
            identifier_ = TARSUB;
            disp_now = "  SUBTITLE-TARGETS:";
            param_prompt1 = "Codec";
            param_prompt2 = "OFF";
            param_prompt3 = "OFF";
            param_prompt4 = "OFF";
            param_prompt5 = "OFF";
          }
        }
        input_ = ui_.readStringNoCapitalize(false);
        return 0;
      } else if (input_ == "exit")
      {
        return 0;
      } else
      {
        ui_.writeString("Please enter one of the options listed above (without ");
        ui_.writeString("the []).", true);
        endthis = false;
      }
    } while (endthis == false);
  } else
  {
    ui_.writeString("Nothing to list", true, "green");
    ui_.readString(false);
  }
}

int ConfigMode::listRule(string& param_prompt1, string& param_prompt2,
                          string& param_prompt3, string& param_prompt4,
                          string& param_prompt5)
{
  param1_ = man_.sendSinglePreference(priority_, identifier_, 1);
  param2_ = man_.sendSinglePreference(priority_, identifier_, 2);
  param3_ = man_.sendSinglePreference(priority_, identifier_, 3);
  param4_ = man_.sendSinglePreference(priority_, identifier_, 4);
  param5_ = man_.sendSinglePreference(priority_, identifier_, 5);

  if (param1_ != "-")
  {
    ui_.writeString(param_prompt1);
    if (param1_.compare(0, 3, "NOT") == 0)
    {
      ui_.writeString(" is not ");
      ui_.writeString(param1_.erase(0, 3));
    } else
    {
      ui_.writeString(" is ");
      ui_.writeString(param1_);
    }
  }
  if (param2_ != "-")
  {
    ui_.writeString(", ");
    ui_.writeString(param_prompt2);
    if (param2_.compare(0, 3, "NOT") == 0)
    {
      ui_.writeString(" is not ");
      ui_.writeString(param2_.erase(0, 3));
    } else
    {
      ui_.writeString(" is ");
      ui_.writeString(param2_);
    }
  }
  if (param3_ != "-")
  {
    ui_.writeString(", ");
    ui_.writeString(param_prompt3);
    if (param3_.compare(0, 3, "NOT") == 0)
    {
      ui_.writeString(" is not ");
      ui_.writeString(param3_.erase(0, 3));
    } else
    {
      ui_.writeString(" is ");
      ui_.writeString(param3_);
    }
  }
  if (param4_ != "-")
  {
    ui_.writeString(", ");
    ui_.writeString(param_prompt4);
    if (param4_.compare(0, 3, "NOT") == 0)
    {
      ui_.writeString(" is not ");
      ui_.writeString(param4_.erase(0, 3));
    } else
    {
      ui_.writeString(" is ");
      ui_.writeString(param4_);
    }
  }
  if (param5_ != "-")
  {
    ui_.writeString(", ");
    ui_.writeString(param_prompt5);
    if (param5_.compare(0, 3, "NOT") == 0)
    {
      ui_.writeString(" is not ");
      ui_.writeString(param5_.erase(0, 3));
    } else
    {
      ui_.writeString(" is ");
      ui_.writeString(param5_);
    }
  }
  if (param1_ == "-" && param2_ == "-" && param3_ == "-" && param4_ == "-"
      && param5_ == "-")
  {
    ui_.writeString("Any stream ");
  }
  ui_.writeString("", true);
}

int ConfigMode::listTarget(string& param_prompt1, string& param_prompt2,
                          string& param_prompt3, string& param_prompt4,
                          string& param_prompt5)
{
  param1_ = man_.sendSinglePreference(priority_, identifier_, 1);
  param2_ = man_.sendSinglePreference(priority_, identifier_, 2);
  param3_ = man_.sendSinglePreference(priority_, identifier_, 3);
  param4_ = man_.sendSinglePreference(priority_, identifier_, 4);
  param5_ = man_.sendSinglePreference(priority_, identifier_, 5);

  if(identifier_ == TARSUB)
    param_prompt1 = "Codec";
  
  ui_.writeString(param_prompt1);
  ui_.writeString(": ");
  if (param1_ != "-")
  {
    ui_.writeString(param1_);
  } else
  {
    ui_.writeString("unchanged");
  }
  if (param2_ != "OFF")
  {
    ui_.writeString(", ");
    ui_.writeString(param_prompt2);
    ui_.writeString(": ");
    if (param2_ != "-")
    {
      ui_.writeString(param2_);
    } else
    {
      ui_.writeString("unchanged");
    }
  }
  if (param3_ != "OFF")
  {
    ui_.writeString(", ");
    ui_.writeString(param_prompt3);
    ui_.writeString(": ");
    if (param3_ != "-")
    {
      ui_.writeString(param3_);
    } else
    {
      ui_.writeString("unchanged");
    }
  }
  if (param4_ != "OFF")
  {
    ui_.writeString(", ");
    ui_.writeString(param_prompt4);
    ui_.writeString(": ");
    if (param4_ != "-")
    {
      ui_.writeString(param4_);
    } else
    {
      ui_.writeString("unchanged");
    }
  }
  if (param5_ != "OFF")
  {
    ui_.writeString(", ");
    ui_.writeString(param_prompt5);
    ui_.writeString(": ");
    if (param5_ != "-")
    {
      ui_.writeString(param5_);
    } else
    {
      ui_.writeString("unchanged");
    }
  }
  ui_.writeString("", true);
}

int ConfigMode::deleteConfig()
{
  bool endthis = false;
  bool going_back = true;

  if (man_.getVectorLen(TARVIDEO) != 0 || man_.getVectorLen(TARAUDIO) != 0 ||
      man_.getVectorLen(TARSUB) != 0)
  {
    do
    {
      if (going_back == true)
      {
        ui_.writeString("", true);
        ui_.writeString("Deleting a preference ...", true, "red");
        ui_.writeString("  Do you want to delete an output-stream or a rule?", true);
        ui_.writeString("  [");
        ui_.writeString("rule", false, "yellow");
        ui_.writeString("]   - create new rule", true); //OK
        ui_.writeString("  [");
        ui_.writeString("target", false, "yellow");
        ui_.writeString("] - create new output-stream", true);
        ui_.writeString("  [");
        ui_.writeString("exit", false, "yellow");
        ui_.writeString("]   - go back", true);
        ui_.writeString("", true);

      }
      input_ = ui_.readStringNoCapitalize();
      if (input_ == "rule")
      {
        if (man_.getVectorLen(SRCVIDEO) != 0 || man_.getVectorLen(SRCAUDIO) != 0 ||
            man_.getVectorLen(SRCSUB) != 0)
        {
          if (deleteRule() == 0)
          {
            return 0;
          } else
          {
            going_back = true;
            endthis = false;
          }
          return 0;
        } else
        {
          ui_.writeString("You have to specify rules first.", true, "green");
          ui_.readString(false);
        }
      } else if (input_ == "target")
      {
        if (deleteTarget() == 0)
        {
          return 0;
        } else
        {
          going_back = true;
          endthis = false;
        }
      } else if (input_ == "exit")
      {
        return -1;
      } else
      {
        going_back = false;
        ui_.writeString("Please enter one of the options listed above (without ");
        ui_.writeString("the []).", true);
        endthis = false;
      }
    } while (endthis == false);
  } else
  {
    ui_.writeString("Nothing to delete", true, "green");
    ui_.readString(false);
  }
}

int ConfigMode::deleteRule()
{
  unsigned int tmp_vec_len = 0;
  unsigned int stream_nr = 0;
  unsigned int entered_number = 0;
  unsigned int tmp_vec_src_tar_len = 0;
  unsigned int tmp_identifier = 0;
  unsigned int tmp_priority = 0;
  string disp_now;

  string param_prompt1 = "Container";
  string param_prompt2 = "Codec";
  string param_prompt3 = "Bitrate";
  string param_prompt4 = "Resolution";
  string param_prompt5 = "Frames per Second";

  bool number = false;
  identifier_ = SRCVIDEO;
  tmp_vec_len = man_.getVectorLen(SRCVIDEO);
  disp_now = "  VIDEO-RULES:";
  for (unsigned short pref_counter = 0; pref_counter < 3; pref_counter++)
  {
    if (tmp_vec_len > 0)
    {
      ui_.writeString(disp_now, true);
      for (priority_ = 1; priority_ <= tmp_vec_len; priority_++)
      {
        stream_nr++;
        ui_.writeString("  ");
        ui_.writeNumber(stream_nr, false, "red");
        ui_.writeString(") ", false, "red");
        listRule(param_prompt1, param_prompt2,
                 param_prompt3, param_prompt4,
                 param_prompt5);
        tmp_vec_src_tar_len = man_.getTargetVectorLenFromSrc(identifier_, priority_);
        tmp_identifier = identifier_;
        if (pref_counter == 0)
        {
          identifier_ = TARVIDEO;
        } else if (pref_counter == 1)
        {
          identifier_ = TARAUDIO;
        } else if (pref_counter == 2)
        {
          identifier_ = TARSUB;
        }
        tmp_priority = priority_;
        for (unsigned int target_item = 1; target_item <= tmp_vec_src_tar_len; target_item++)
        {
          priority_ = man_.getTargetIDfromSource(tmp_priority, tmp_identifier, target_item);
          ui_.writeString("      -> ");
          ui_.writeNumber(priority_);
          ui_.writeString(") ");
          listTarget(param_prompt1, param_prompt2, param_prompt3, param_prompt4,
                     param_prompt5);
        }
        identifier_ = tmp_identifier;
        priority_ = tmp_priority;
      }
    }
    ui_.writeString("", true);
    if (pref_counter == 0)
    {
      tmp_vec_len = man_.getVectorLen(SRCAUDIO);
      identifier_ = SRCAUDIO;
      disp_now = "  AUDIO-RULES:";
      param_prompt1 = "Codec";
      param_prompt2 = "Channels";
      param_prompt3 = "Language";
      param_prompt4 = "Bitrate";
      param_prompt5 = "Sample Rate";
    }
    if (pref_counter == 1)
    {
      tmp_vec_len = man_.getVectorLen(SRCSUB);
      identifier_ = SRCSUB;
      disp_now = "  SUBTITLE-RULES:";
      param_prompt1 = "Codec";
      param_prompt2 = "Language";
      param_prompt3 = "OFF";
      param_prompt4 = "OFF";
      param_prompt5 = "OFF";
    }
  }
  ui_.writeString("P");
  do
  {
    ui_.writeString("lease enter a number between 1 and ");
    ui_.writeNumber(stream_nr);
    ui_.writeString(" (enter exit to abort).", true);
    input_ = ui_.readStringNoCapitalize();
    if (ui_.stringToUnsignedInt(input_, entered_number) == false
        && input_ != "exit")
    {
      ui_.writeString("Not a valid number, p");
    } else if (input_ == "exit")
    {
      return -1;
    } else if (entered_number < 1 || entered_number > stream_nr)
    {
      ui_.writeString("Not a valid number, p");
    } else
    {
      number = true;
    }
  } while (number == false);

  number = false;
  tmp_vec_len = 0;
  for (identifier_ = SRCVIDEO; number == false; identifier_++)
  {
    tmp_vec_len = man_.getVectorLen(identifier_);
    if (entered_number > tmp_vec_len)
    {
      entered_number = entered_number - tmp_vec_len;
    } else
    {
      priority_ = entered_number;
      number = true;
    }
  }
  identifier_--;
  man_.deleteFromVector(priority_, identifier_);
  ui_.writeString("Successfully deleted!", true);
}

int ConfigMode::deleteTarget()
{
  unsigned int tmp_vec_len = 0;
  unsigned int stream_nr = 0;
  unsigned int entered_number = 0;
  unsigned int temp_id = 0;
  unsigned int vec_len = 0;
  unsigned int target_index_len = 0;
  unsigned int source_identifier = 0;
  string disp_now;

  string param_prompt1 = "Container";
  string param_prompt2 = "Codec";
  string param_prompt3 = "Bitrate";
  string param_prompt4 = "Resolution";
  string param_prompt5 = "Frames per Second";

  bool can_delete = true;
  bool number = false;
  identifier_ = TARVIDEO;
  tmp_vec_len = man_.getVectorLen(TARVIDEO);
  disp_now = "  VIDEO-TARGETS:";
  
  for (unsigned short pref_counter = 0; pref_counter < 3; pref_counter++)
  {
    if (tmp_vec_len > 0)
    {
      ui_.writeString(disp_now, true);
      for (priority_ = 1; priority_ <= tmp_vec_len; priority_++)
      {
        stream_nr++;
        ui_.writeString("    ");
        ui_.writeNumber(stream_nr, false, "red");
        ui_.writeString(") ", false, "red");
        listTarget(param_prompt1, param_prompt2, param_prompt3, param_prompt4,
                   param_prompt5);
      }
    }
    ui_.writeString("", true);
    if (pref_counter == 0)
    {
      tmp_vec_len = man_.getVectorLen(TARAUDIO);
      identifier_ = TARAUDIO;
      disp_now = "  AUDIO-TARGETS:";
      param_prompt1 = "Codec";
      param_prompt2 = "Channels";
      param_prompt3 = "Language";
      param_prompt4 = "Bitrate";
      param_prompt5 = "Sample Rate";
    }
    if (pref_counter == 1)
    {
      tmp_vec_len = man_.getVectorLen(TARSUB);
      identifier_ = TARSUB;
      disp_now = "  SUBTITLE-TARGETS:";
      param_prompt1 = "Codec";
      param_prompt2 = "OFF";
      param_prompt3 = "OFF";
      param_prompt4 = "OFF";
      param_prompt5 = "OFF";
    }
  }
  ui_.writeString("P");
  do
  {
    ui_.writeString("lease enter a number between 1 and ");
    ui_.writeNumber(stream_nr);
    ui_.writeString(" (enter exit to abort).", true);
    input_ = ui_.readStringNoCapitalize();
    if (ui_.stringToUnsignedInt(input_, entered_number) == false &&
        input_ != "exit")
    {
      ui_.writeString("Not a valid number, p");
    } else if (input_ == "exit")
    {
      return -1;
    } else if (entered_number < 1 || entered_number > stream_nr)
    {
      ui_.writeString("Not a valid number, p");
    } else
    {
      number = true;
    }
  } while (number == false);

  number = false;
  tmp_vec_len = 0;
  for (identifier_ = TARVIDEO; number == false; identifier_++)
  {
    tmp_vec_len = man_.getVectorLen(identifier_);
    if (entered_number > tmp_vec_len)
    {
      entered_number = entered_number - tmp_vec_len;
    } else
    {
      priority_ = entered_number;
      number = true;
    }
  }
  identifier_--; //This is now the true identifier!
  source_identifier = identifier_ - 3;
  vec_len = man_.getVectorLen(source_identifier);

  //unlink target from sources
  for (unsigned int priority = 1; priority <= vec_len; priority++)
  {
    target_index_len = man_.getTargetVectorLenFromSrc(source_identifier, priority);
    if (man_.deleteAllTarget(priority, source_identifier, priority_) == -1)
    {
      ui_.writeString("Could not delete target because this would cause a rule ", false, "green");
      ui_.writeString("to lose all targets.", true, "green");
      ui_.writeString("Canceled operation.", true, "green");
      ui_.readString(false);
      can_delete = false;
    }
  }
  if (can_delete == true)
  {
    //delete target
    man_.deleteFromVector(priority_, identifier_);

    //Adjust the ids
    vec_len = man_.getVectorLen(source_identifier);
    for (unsigned int priority = 1; priority <= vec_len; priority++)
    {
      target_index_len = man_.getTargetVectorLenFromSrc(source_identifier, priority);
      for (unsigned int target_index = 1; target_index <= target_index_len; target_index++)
      {
        temp_id = man_.getTargetIDfromSource(priority, source_identifier, target_index);
        if (temp_id > priority_)
        {
          man_.editTarget(priority, source_identifier, temp_id - 1, target_index);
        }
      }
    }
    ui_.writeString("Successfully deleted!", true);
    return 0;
  }
}
