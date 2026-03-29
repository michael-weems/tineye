#include <filesystem> 
#include <format>
#include <iostream> 
#include <vector> 

#include "tineye/tineye.h"
#include "tineye/thread_pool.h"

int main(int argc, char* argv[])
{  
   if (argc < 4)
   {
      std::cerr << std::format("ERROR: please pass a directory, a search term, and an outfile") << std::endl;
      return 1;
   }

   std::string dirpath(argv[1]);
   std::string query(argv[2]);
   std::string outfile(argv[3]);

   tineye::thread_pool::start(std::thread::hardware_concurrency() - 1);

   std::filesystem::path dir(dirpath);
   std::vector<tineye::entry> entries = tineye::search_directory(dir, query);

   std::filesystem::path output(outfile);
   tineye::write_results_to_file(output, entries);


/*
   for (int i{0}; i < entries.size(); ++i)
   {
      tineye::entry& e = entries[i];
      if (e.error)
      {
         std::cout << std::format("ERROR: {}", e.error_message) << std::endl;
      }
      else
      {
         std::cout << std::format("{} | {} | {} | {}", e.filepath.string(), e.line, e.column, e.text) << std::endl;
      }
   }
   std::cout << "---------------------------------------------------------------------------" << std::endl;
   std::cout << std::format("found {}", entries.size()) << std::endl;
*/

   tineye::thread_pool::stop(); 

   return 0;
}
