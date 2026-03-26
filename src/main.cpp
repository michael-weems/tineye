#include <iostream> 
#include <filesystem> 
#include <vector> 

#include "tineye/tineye.h"

int main(int argc, char* argv[])
{  
   std::filesystem::path dir("src");
   std::vector<tineye::Entry> entries = tineye::search_directory(dir);
   std::cout << "yee" << std::endl;   
 
   return 0;
}
