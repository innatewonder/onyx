#pragma once

class ArgParser
{
public:
  struct Value
  {
    union
    {
      s32 iVal;
      f32 fVal;
      const char* sVal;
    };

    enum Type{ INT, FLOAT, STRING, NONE } type;

    Value(Type t) : type(t) {}
  };

  struct Command
  {
    Value val;
    String command;
    String shortcut;

    Command(const String& shortcut, const String& cmd = EMPTY_STRING) 
    : val(Value::NONE), shortcut(shortcut), command(cmd)
    {}

    bool operator==(const String& str);
    Command* MakeCommand(const char* str);
  };
  
  ArgParser(int argc, char** argv);
  ~ArgParser();

  void Parse();
  void AddCommand(const String& shortcut, const String& command = EMPTY_STRING, 
                  bool hasIntValue = false, bool hasStringValue = false, 
                  bool hasFloatValue = false);

  int GetArgC() const;
  char** GetArgV() const;

  bool Has(const String& str) const;
  Command* Get(const String& str) const;

private:
  int m_argc;
  char** m_argv;

  Containers::List<Command> m_commands;
  Containers::List<Command*> m_argList;
};

