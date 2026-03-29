#include "tineye/thread_pool.h"

#include <functional>
#include <mutex>
#include <thread>
#include <vector>

namespace tineye
{
   namespace thread_pool 
   {

      bool started{false};
      unsigned int thread_count{0};

      bool should_terminate = false;           
      std::mutex queue_mutex;                 
      std::condition_variable mutex_condition; 
      std::vector<std::thread> threads;
      std::queue<std::function<void()>> jobs;

      void start(uint32_t num_threads) 
      {
         started = true;

         if (num_threads <= 0) 
         {
            num_threads = 1;
         }

         thread_count = num_threads;

         for (uint32_t i = 0; i < thread_count; ++i) 
         {
            threads.emplace_back(std::thread(_internal_thread_loop));
         }
      }

      void queue_job(const std::function<void()>& job) 
      {
         if (!started)
         {
            start();
         }

         {
            std::unique_lock<std::mutex> lock(queue_mutex);
            jobs.push(job);
         }

         mutex_condition.notify_one();
      }

      bool is_busy() 
      {
         if (!started)
         {
            start();
         }

         bool busy{false};
         std::unique_lock<std::mutex> lock(queue_mutex);
         return !jobs.empty();
      }

      // NOTE: doesn't cancel running jobs, just waits for them to finish
      void stop() 
      {
         if (!started)
         {
            start();
         }

         {
            std::unique_lock<std::mutex> lock(queue_mutex);
            should_terminate = true;
         }

         mutex_condition.notify_all();
         for (std::thread& active_thread : threads) 
         {
            active_thread.join();
         }
         threads.clear();
      }

      
      void _internal_thread_loop()
      {
         while (true) 
         {
            std::function<void()> job;
            {
               std::unique_lock<std::mutex> lock( queue_mutex );
               mutex_condition.wait(lock, []{
                  return !jobs.empty() || should_terminate;
               });
               if (should_terminate) {
                  return;
               }
               job = jobs.front();
               jobs.pop();
            }
            job();
         }
      }
         
   };
};
