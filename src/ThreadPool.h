#pragma once
#include <queue>
#include <condition_variable>
#include <functional>
#include <thread>
#include <mutex>
#include <vector>
#include <cassert>

class ThreadPool
{
public:
	explicit ThreadPool(unsigned threadCount = std::thread::hardware_concurrency());

	ThreadPool(const ThreadPool&) = delete;
	ThreadPool& operator=(const ThreadPool&) = delete;
	ThreadPool(ThreadPool&&) = delete;
	ThreadPool& operator=(ThreadPool&&) = delete;
	~ThreadPool();

	void addTask(std::function<void()> task);

private:
	using TaskType = std::function<void()>;

	std::queue<TaskType> tasks;
	std::condition_variable taskReady;
	std::mutex taskMutex;

	std::vector<std::thread> threads;
	bool stop;
	void workerMain();
};

ThreadPool::ThreadPool(unsigned threadCount) :
	stop(false)
{
	for (unsigned i = 0; i < threadCount; ++i)
		threads.emplace_back([this] { workerMain(); });
}

ThreadPool::~ThreadPool()
{
	stop = true;
	taskReady.notify_all();
	for (auto& thread : threads)
		thread.join();
}

void ThreadPool::workerMain()
{
	while (!stop)
	{
		TaskType task;

		{	// Retrieve next task from the queue
			std::unique_lock lock{taskMutex};
			taskReady.wait(lock, [this]{ return tasks.size() > 0 || stop; });
			if (stop)
				break;

			assert(tasks.size() > 0);
			task = std::move(tasks.front());
			tasks.pop();
		}

		// Execute the task
		task();
	}
}

void ThreadPool::addTask(std::function<void()> task)
{
	std::unique_lock lock{taskMutex};
	tasks.push(std::move(task));
	lock.unlock();
	taskReady.notify_one();
}
