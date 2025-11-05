#include "Analyzer.hpp"
#include <fstream>
#include <sstream>
#include <tree_sitter/api.h>

// extern grammar function from tree-sitter-typescript; link with its C library
extern "C" TSLanguage *tree_sitter_typescript (); // ensure you build/link tree-sitter-typescript

nlohmann::json
Report::to_json () const
{
  nlohmann::json j;
  j["file"] = file;
  j["metrics"] = { { "functions", functions }, { "imports", imports } };
  j["diagnostics"] = nlohmann::json::array ();
  for (auto &d : diagnostics)
    j["diagnostics"].push_back ({ { "rule", d.rule }, { "message", d.message }, { "line", d.line } });
  return j;
}

Analyzer::Analyzer () {}

static int
byte_to_line (const std::string &s, size_t byte_index)
{
  int line = 1;
  for (size_t i = 0; i < std::min (byte_index, s.size ()); ++i)
    if (s[i] == '\n')
      ++line;
  return line;
}

Report
Analyzer::analyze_source (const std::string &source, const std::string &path)
{
  Report r;
  r.file = path;
  // setup parser
  TSParser *parser = ts_parser_new ();
  ts_parser_set_language (parser, tree_sitter_typescript ());
  TSTree *tree = ts_parser_parse_string (parser, nullptr, source.c_str (), source.size ());
  TSNode root = ts_tree_root_node (tree);

  // simple walk: count function_declaration and import_declaration nodes
  std::vector<TSNode> stack = { root };
  while (!stack.empty ())
    {
      TSNode n = stack.back ();
      stack.pop_back ();
      const char *type = ts_node_type (n);
      if (strcmp (type, "function_declaration") == 0 || strcmp (type, "function") == 0)
        {
          r.functions++;
          // naive check: count parameters by scanning source substring for commas inside param list
          TSNode params = ts_node_named_child (n, 1); // best-effort
          if (ts_node_is_null (params) == false)
            {
              uint32_t start = ts_node_start_byte (params), end = ts_node_end_byte (params);
              std::string slice = source.substr (start, end - start);
              int params_count = 0;
              for (char c : slice)
                if (c == ',')
                  params_count++;
              // params_count is commas; number of params = commas+ (non-empty ? 1 : 0)
              if (slice.find_first_not_of (" \t\n\r") != std::string::npos)
                params_count++;
              if (params_count > 4)
                {
                  Diagnostic d;
                  d.rule = "long-parameter-list";
                  d.message = "Function has many parameters";
                  d.line = byte_to_line (source, ts_node_start_byte (n));
                  r.diagnostics.push_back (d);
                }
            }
        }
      else if (strcmp (type, "import_statement") == 0 || strcmp (type, "import_declaration") == 0)
        {
          r.imports++;
        }
      // push children
      uint32_t child_count = ts_node_named_child_count (n);
      for (uint32_t i = 0; i < child_count; ++i)
        stack.push_back (ts_node_named_child (n, i));
    }

  ts_tree_delete (tree);
  ts_parser_delete (parser);
  return r;
}

