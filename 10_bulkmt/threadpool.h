#pragma once

#include <chrono>
#include <condition_variable>
#include <iostream>
#include <queue>
#include <thread>
#include <atomic>




template<class Args, class func>
class ThreadPool
{
public:
   const std::size_t threadQty;

   ThreadPool (func foo, std::size_t threadQty = 1) : threadQty(threadQty)
   {
      cv_m.lock();
      work = true;
      cv_m.unlock();
      threads.reserve (threadQty);
      while (threadQty--)
         threads.push_back (
            std::make_unique<std::thread> (
               worker,
               foo,
               std::ref(msgs),
               std::ref(work),
               std::ref(cv),
               std::ref(cv_m)
            )
         );
   }

   ~ThreadPool () {
      stopWork();
   }


   void addMessage (Args msg)
   {
      cv_m.lock();
      msgs.push(msg);
      cv_m.unlock();
      cv.notify_one();
      throwException();
   }


   void join()
   {
      waitAll();
      stopWork();
   }

private:
   static void worker (
      func foo,
      std::queue<Args>& q_args,
      std::atomic<bool>& work,
      std::condition_variable& cv,
      std::mutex& cv_m
   ) {
      try {
         bool tmp;
         while (cv_m.lock(), tmp = work, cv_m.unlock(), tmp) {
            std::unique_lock<std::mutex> lk(cv_m);
            cv.wait(lk, [&](){ return not q_args.empty() or not work; });
            if (work) {
               auto args = q_args.front();
               q_args.pop();
               lk.unlock();
               foo(args);
            } else {
               lk.unlock();
            }
         }
      } catch (...) {
         pException = std::current_exception();
      }
   };


   void stopWork()
   {
      cv_m.lock();
      work = false;
      cv_m.unlock();
      cv.notify_all();
      for (auto& t : threads)
         if (t->joinable())
            t->join();
   }

   void waitAll()
   {
      decltype(msgs.size()) tmp;
      while (cv_m.lock(), tmp = msgs.size(), cv_m.unlock(), tmp) {
         cv.notify_one();
         throwException();
      }
   }

   void throwException()
   {
      if (pException) {
         std::rethrow_exception(pException);
      }
   }

   std::vector<std::unique_ptr<std::thread>> threads;
   std::queue<Args> msgs;
   std::condition_variable cv;
   std::mutex cv_m;
   static std::exception_ptr pException;
   std::atomic<bool> work;
};
template<class Args, class func>
std::exception_ptr ThreadPool<Args,func>::pException;


