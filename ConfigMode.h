/* 
 * File:   ConfigMode.h
 * Author: CutePoisonX
 *
 * Created on 04. Februar 2013, 14:34
 */

#ifndef CONFIGMODE_H
#define	CONFIGMODE_H

#include <string>
#include "Mode.h"

using namespace std;

class UserInterface;
class VectorSourceManager;

class ConfigMode : public Mode {
public:
                ConfigMode(UserInterface& ui, VectorSourceManager& man);
                ConfigMode(const ConfigMode& orig);
  virtual      ~ConfigMode();
  
  int           executeCommand();

private:
  
  //----------------------------------------------------------------------------
  VectorSourceManager&  man_;
 
  string        param1_;
  string        param2_;
  string        param3_;
  string        param4_;
  string        param5_;
  
  unsigned int  priority_;
  unsigned int  identifier_;
  //----------------------------------------------------------------------------
  
  int           newConfig();
  int           createRule();
  int           createRuleVideo();
  int           createRuleAudio();
  int           createRuleSub();
  int           createOutput();
  int           createOutputVideo();
  int           createOutputAudio();
  int           createOutputSub();
  int           list();
  int           deleteConfig();
  int           deleteRule();
  int           deleteTarget();
  int           listRule(string& param_prompt1, string& param_prompt2,
                         string& param_prompt3, string& param_prompt4,
                         string& param_prompt5); 
  int           listTarget(string& param_prompt1, string& param_prompt2,
                           string& param_prompt3, string& param_prompt4,
                           string& param_prompt5);
  
  unsigned int  setPriority();
  unsigned int  specifyTarget(const string& param1, const string& param2,
                              const string& param3, const string& param4,
                              const string& param5);
  
  void          standartRuleParameterInputPrompt();
  void          standartTargetParameterInputPrompt();
  void          insertNots(bool param1_not, bool param2_not, bool param3_not,
                           bool param4_not, bool param5_not);
  void          showRuleSummary(const string& param1, const string& param2,
                                const string& param3, const string& param4,
                                const string& param5, unsigned int loops);
  void          showTargetSummary(const string& param1, const string& param2,
                                  const string& param3, const string& param4,
                                  const string& param5);
  
  bool          proceedConfirmation();
};

#endif	/* CONFIGMODE_H */

