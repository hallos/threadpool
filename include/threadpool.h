#include <vector>
#include <memory>
#include <thread>
#include <queue>
#include <functional>
#include <future>
#include <mutex>
#include <condition_variable>

namespace hallos
{
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
            std::function<int()> func = [](){ return 6; };
            add_work(func);
            thread->join();
        }
    };

    template <typename T, typename... ARGS>
    std::future<T> add_work(std::function<T(ARGS...)> function, ARGS... arguments)
    {
        auto task_ptr = std::make_shared<std::packaged_task<T(ARGS...)>>(function);
        auto work = [task_ptr, arguments...]()
                        {
                            (*task_ptr)(arguments...);
                        };
        {
            std::lock_guard<std::mutex> lock(queue_mutex_);
            work_queue_.emplace(work);
        }

        work_available_.notify_one();
        return task_ptr->get_future();
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
            lock.unlock();

            function();
        }
    }
};
} //namespace hallos