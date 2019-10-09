#include <vector>
#include <memory>
#include <thread>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>

class thread_pool
{
public:
    thread_pool(int nr_threads)
    {
        for (int i = 0; i < nr_threads; i++)
        {
            threads_.emplace_back(new std::thread(&thread_pool::run_worker_thread, this));
        }
    };
    ~thread_pool()
    {
        run_threads_ = false;
        for (const auto& thread : threads_)
        {
            add_work([](){});
            thread->join();
        }
    };

    void add_work(const std::function<void()>& work)
    {
        std::lock_guard<std::mutex> lock(queue_mutex_);
        work_queue_.emplace(work);
        work_available_.notify_all();
    }

private:
    bool run_threads_ = true;
    std::vector<std::unique_ptr<std::thread>> threads_;
    std::queue<std::function<void()>> work_queue_;
    std::mutex queue_mutex_;
    std::condition_variable work_available_;

    void run_worker_thread()
    {
        while (run_threads_)
        {
            std::unique_lock<std::mutex> lock(queue_mutex_);
            if (work_queue_.empty())
            {
                work_available_.wait(lock);
            }
            auto function = work_queue_.front();
            work_queue_.pop();
            function();
        }
    }
};