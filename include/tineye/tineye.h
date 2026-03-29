#ifndef TINEYE_H
#define TINEYE_H  

#include <string>
#include <vector>
#include <filesystem>
 
namespace fs = std::filesystem;

namespace tineye
{
   struct entry
   {
      std::string text; // considering doing paragraph or line or other previews, would need to re-think storage, if need to store at all - possibly just pipe line and column to another tool to do the preview?
      fs::path filepath;
      int line;
      int column;
      bool error;
      std::string error_message;
 
      entry(){}
      entry(std::string text, fs::path filepath, int line, int column, bool error, std::string error_message) 
      : text(text), filepath(filepath), line(line), column(column), error(error), error_message(error_message)
      {}
   };

   std::vector<fs::directory_entry> list_files_in_directory(const fs::path& dirpath);

   void search_file(std::vector<entry>& entries, const fs::directory_entry& file, std::string query);

   std::vector<entry> search_directory(fs::path filepath, std::string query);
 
   void write_results_to_file(fs::path filepath, std::vector<entry>& entries);
};


#endif // TINEYE_H
