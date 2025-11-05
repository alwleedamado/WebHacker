#pragma once
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

struct Diagnostic
{
  std::string rule;
  std::string message;
  int line;
};
struct Report
{
  std::string file;
  std::vector<Diagnostic> diagnostics;
  int functions = 0;
  int imports = 0;
  nlohmann::json to_json () const;
};

class Analyzer
{
public:
  Analyzer ();
  Report analyze_source (const std::string &source, const std::string &path);
};
