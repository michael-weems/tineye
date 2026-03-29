#ifndef TINEYE_THREAD_POOL_H
#define TINEYE_THREAD_POOL_H

#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

namespace tineye
{
   namespace thread_pool
   {
      extern bool started;
      extern unsigned int thread_count;

      extern bool should_terminate;           
      extern std::mutex queue_mutex;                 
      extern std::condition_variable mutex_condition; 
      extern std::vector<std::thread> threads;
      extern std::queue<std::function<void()>> jobs;

      void start(uint32_t num_threads = std::thread::hardware_concurrency() - 1);

      void queue_job(const std::function<void()>& job); 

      bool is_busy();

      // NOTE: doesn't cancel running jobs, just waits for them to finish
      void stop(); 
      
      void _internal_thread_loop();

   };
};

#endif // TINEYE_THREAD_POOL_H
