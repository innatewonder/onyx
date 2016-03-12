#include "CommonPrecompiled.h"
#include "ArgParser.h"

bool ArgParser::Command::operator==(const String& str)
{
  return str == command || str == shortcut;
}

ArgParser::Command* ArgParser::Command::MakeCommand(const char* str)
{
  //This is done before custom memory is created, so we must use default new
  Command* ret = new Command(shortcut, command);
  ret->val.type = val.type;
  switch(val.type)
  {
    case Value::INT:
      ret->val.iVal = atoi(str);
      break;
    case Value::FLOAT:
      ret->val.fVal = (f32)atof(str);
      break;
    case Value::STRING:
      ret->val.sVal = str;
      break;
  }
  return ret;
}

ArgParser::ArgParser(int argc, char** argv)
  : m_argv(argv), m_argc(argc)
{}

ArgParser::~ArgParser()
{
  for(auto c : m_argList)
  {
    delete c;
  }
}

void ArgParser::Parse()
{
  for(int i = 1; i < m_argc; ++i)
  {
    for(auto cmd : m_commands)
    {
      if(cmd == m_argv[i])
      {
        if(cmd.val.type != Value::NONE)
        {
          assertion(i + 1 < m_argc);
          m_argList.push_back(cmd.MakeCommand(m_argv[i + 1]));
        }
        else
          m_argList.push_back(cmd.MakeCommand(m_argv[i]));
        break;
      }
    }
  }
}

void ArgParser::AddCommand(const String& shortcut, const String& command, 
                  bool hasIntValue, bool hasStringValue, 
                  bool hasFloatValue)
{
  int numVals = hasIntValue ? 1 : 0;
  numVals += hasStringValue ? 1 : 0;
  numVals += hasFloatValue ? 1 : 0;
  assertion(numVals <= 1);

  Command cmd(shortcut, command);

  if(hasIntValue)
  {
    cmd.val.type = Value::INT;
  }
  else if(hasStringValue)
  {
    cmd.val.type = Value::STRING;
  }
  else if(hasFloatValue)
  {
    cmd.val.type = Value::FLOAT;
  }

  m_commands.push_back(cmd);
}

int ArgParser::GetArgC() const
{
  return m_argc;
}

char** ArgParser::GetArgV() const
{
  return m_argv;
}

bool ArgParser::Has(const String& str) const
{
  for(auto c : m_argList)
  {
    if(*c == str)
      return true;
  }
  return false;
}

ArgParser::Command* ArgParser::Get(const String& str) const
{
  for(auto c : m_argList)
  {
    if(*c == str)
      return c;
  }
  return nullptr;
}
