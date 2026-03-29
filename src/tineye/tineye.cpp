#include "tineye/tineye.h"
 
#include <array>
#include <chrono>
#include <format>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>
#include <vector>
 
#include "tineye/thread_pool.h"

namespace fs = std::filesystem;

namespace tineye
{

   std::vector<fs::directory_entry> list_files_in_directory(const fs::path& dirpath)
   {
      std::vector<fs::directory_entry> paths;
      try 
      {
         //for (const auto& entry : fs::directory_iterator(directory_path)) 
         // TODO: change auto to actual type
         for (const auto& entry : fs::recursive_directory_iterator(dirpath)) 
         {
            if (fs::is_regular_file(entry.path())) 
            {
               paths.push_back(entry);
            }
         }
      } 
      catch (const fs::filesystem_error& e) 
      {
         std::cerr << "Filesystem error: " << e.what() << std::endl;
      }

      return paths;
   }
   
   // TODO: plan: output results to a file. open that file in a vim split buffer, use vim motions to move to a line, <Enter> takes me to that file and line. toggle this buffer open / closed with keystroke. 

   // TODO: implement with boyer-moore algorithm
   // https://github.com/likejazz/boyer-moore-string-search/blob/master/boyer-moore.c
   void search_file(std::vector<entry>& entries, const fs::directory_entry& file, std::string query)
   {
      std::ifstream file_stream(file.path());
      if (file_stream.is_open()) 
      {
         std::string line;
         int line_count = 0;
         while (std::getline(file_stream, line)) 
         {
            ++line_count;
            if (line.find(query) != std::string::npos)
            {
               int column{0}; // TODO:
               entries.emplace_back(line, file.path(), line_count, column, false, "");
            }
          }
          file_stream.close();
      } 
      else 
      {
         entries.emplace_back("", file.path(), 0, 0, true, std::format("could not open file: {}", file.path().string()));
      }
   }

   std::vector<entry> search_directory(fs::path filepath, std::string query)
   {
      std::vector<fs::directory_entry> files = list_files_in_directory(filepath);      
      std::vector<std::vector<entry>>  nested_entries(files.size());

      for (int i = 0; i < nested_entries.size(); ++i) 
      {
         const std::function<void()>& job = std::bind(&search_file, std::ref(nested_entries[i]), std::cref(files[i]), query);
         thread_pool::queue_job(job); 
      }
 
      while (thread_pool::is_busy())
      {
      }
 
      // flatten output

      int total_found{0};
      for (int i{0}; i < nested_entries.size(); ++i)
      {
         total_found += nested_entries[i].size();
      }
      
      int idx{0};
      std::vector<entry> out(total_found); 
      for (int i{0}; i < nested_entries.size(); ++i)
      {
         for (int j{0}; j < nested_entries[i].size(); ++j)
         {
            entry& e = nested_entries[i][j];
            //  std::cout << std::format("i {}: j {}: text {}: filepath {}: line {}", i, j, e.text, e.filepath.string(), e.line) << std::endl;
            out[idx].text = e.text; 
            out[idx].filepath = e.filepath;
            out[idx].line = e.line;
            out[idx].column = e.column;
            out[idx].error = e.error;
            out[idx].error_message = e.error_message;
            ++idx;
         }
      }
      
      return out;
   }

   void write_results_to_file(fs::path filepath, std::vector<entry>& entries)
   {
      std::ofstream file(filepath); 

      if (!file.is_open()) {
         return; 
      }
 
      for (int i{0}; i < entries.size(); ++i)
      {
         file << entries[i].filepath.string() << " | " << entries[i].line << std::endl;
         file << entries[i].text << std::endl;
         file << std::endl;
      }

      file.close(); 
   }

};
