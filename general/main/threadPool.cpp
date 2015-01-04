#ifndef __THREAD_POOL__
#define __THREAD_POOL__

class ThreadPool
{
public:		
        typedef std::function<void()> fn_type;
 
        class Worker
        {    
        public:
                Worker() : fqueue(),thread(&Worker::thread_fn, this), enabled(true){}
 
                ~Worker()
                {
					enabled = false;
					cv.notify_one();
					thread.join();
                }  
 
                void appendFn(fn_type fn)
                {
					std::unique_lock<std::mutex> locker(mutex);
					fqueue.push(fn);                 
					cv.notify_one();
                }      
 
                size_t getTaskCount() 
                {
					std::unique_lock<std::mutex> locker(mutex);
					return fqueue.size();          
                }
 
                bool isEmpty() 
                {
					std::unique_lock<std::mutex> locker(mutex);
					return fqueue.empty(); 
                }
 
        private:
               
                std::condition_variable cv;
                std::queue<fn_type>             fqueue;
                std::mutex                      mutex;  
                std::thread                     thread;
                bool                            enabled;
 
                void thread_fn()
                {
					while (enabled)
					{
						std::unique_lock<std::mutex> locker(mutex); 
						cv.wait(locker, [&](){ return !fqueue.empty() || !enabled; });
						while(!fqueue.empty())
						{
							fn_type fn = fqueue.front(); 
							fqueue.pop();
							locker.unlock();
							fn();
							locker.lock();
						}                              
					}
                }
        };
 
        typedef std::shared_ptr<Worker> worker_ptr;
        
        ThreadPool(size_t threads)
        {
			if (threads <= 0)threads = 1;
			for (size_t i = 0; i<threads; i++)
			{
				worker_ptr pWorker(new Worker); 
				_workers.push_back(pWorker);
			}
        }
 
		template<class _FN, class... _ARGS>
        void runAsync(_FN _fn, _ARGS... _args)
        {
			auto pWorker = getFreeWorker();
			if(pWorker.get() == nullptr) return;;
			pWorker->appendFn(std::bind(_fn,_args...));
        }  
private:
        worker_ptr getFreeWorker()
        {
			worker_ptr pWorker;
			size_t minTasks = UINT32_MAX;                          
			for (auto &it : _workers)
			{
				if (it->isEmpty()) 
				{
					return it;
				}
				else if (minTasks > it->getTaskCount())
				{
					minTasks = it->getTaskCount();
					pWorker = it;
				}
			}
			return pWorker;
        }
 
        std::vector<worker_ptr> _workers;
};

#endif
