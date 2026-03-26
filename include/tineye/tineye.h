#ifndef TINEYE_H
#define TINEYE_H  

#include <string>
#include <vector>
#include <filesystem>

namespace tineye
{
   struct Entry
   {
      std::string text; // considering doing paragraph or line or other previews, would need to re-think storage, if need to store at all - possibly just pipe line and column to another tool to do the preview?
      std::filesystem::path filepath;
      int line;
      int column;
   };

   std::vector<Entry> search_directory(std::filesystem::path dirpath);
};


#endif // TINEYE_H
