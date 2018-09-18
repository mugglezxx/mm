/*
 * IDispatcher.hpp
 *
 *  Created on: Sep 24, 2017
 *      Author: suoalex
 */

#ifndef LIB_THREADING_DISPATCHER_HPP_
#define LIB_THREADING_DISPATCHER_HPP_

#include <atomic>
#include <cstdint>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#include <tbb/concurrent_queue.h>

#include <Poco/Hash.h>

#include "Runnable.hpp"
#include "DispatchKey.hpp"

namespace mm
{
	//
	// The dispatcher config items.
	//
	struct DispatcherConfig
	{
		// The dispatcher config
		static const std::string DISPATCHER_CONFIG;

		// The thread count.
		static const std::string THREAD_COUNT;
	};

	//
	// The basic execution unit of a hash dispatcher, this class is simply
	// a thread taking all tasks from a given queue.
	//
	// The queue is expected to be thread-safe and the thread will execute tasks from the queue.
	//
	template<typename Mutex = std::mutex> class TaskRunner
	{
	public:

		//
		// Constructor.
		//
		// waitOnEmpty : Flag if thread should enter passive wait when queue is empty.
		//
		TaskRunner(bool waitOnEmpty = true) : waitOnEmpty(waitOnEmpty), stopRequested(false), thread(nullptr)
		{
		}

		//
		// Destructor.
		//
		~TaskRunner()
		{
		}

		//
		// Start execution of the tasks.
		//
		void start()
		{
			// sanity check
			if (thread.get() != nullptr)
			{
				return;
			}

			thread.reset(new std::thread([this] ()
			{
				Runnable runnable;

				while (!stopRequested.load())
				{
					if (queue.try_pop(runnable))
					{
						runnable();
					}
					else if (waitOnEmpty)
					{
						// enter inactive thread wait
						std::unique_lock<Mutex> lock(mutex);
						while (queue.empty() && !stopRequested.load())
						{
							condition.wait(lock);
						}
					}
				}
			}));
		}

		//
		// Stop the runner and optionally wait for the tasks to finish.
		//
		// waitForDone : Flag if the thread should be stopped immediately.
		//
		void stop()
		{
			stopRequested.store(true);

			// signal the thread to check flag if needed
			if (waitOnEmpty && queue.empty())
			{
				std::lock_guard<Mutex> guard(mutex);
				condition.notify_all();
			}

			thread->join();
		}

		//
		// Submit a task into the runner.
		//
		// runnable : The task to be executed.
		//
		void submit(const Runnable& runnable)
		{
			bool wasEmpty = waitOnEmpty ? queue.empty() : false;

			queue.push(runnable);

			// notify where needed
			if (wasEmpty && waitOnEmpty)
			{
				std::lock_guard<Mutex> guard(mutex);
				condition.notify_all();
			}
		}

	private:

		// Flag if the thread should wait when queue is empty.
		const bool waitOnEmpty;

		// Flag if stop is called.
		std::atomic<bool> stopRequested;

		// Pointer to actual thread.
		std::unique_ptr<std::thread> thread;

		// Mutex used for queue notify.
		Mutex mutex;

		// The condition variable used.
		std::condition_variable condition;

		// The queue structure holding the tasks.
		tbb::concurrent_queue<Runnable> queue;

	};

	//
	// A hash dispatcher.
	//
	// The tasks are separated into queues based on the hash value of the key submitted.
	//
	template<
		typename Key = KeyType,
		typename Mutex = std::mutex,
		typename Hash = std::hash<Key>
	> class HashDispatcher
	{
	public:

		//
		// Constructor.
		//
		// threadCount : number of threads in the dispatcher.
		// start : Flag if to start at creation.
		//
		HashDispatcher(size_t threadCount = 4, bool start = true) : runners(threadCount), runningFlag(false)
		{
			if (start)
			{
				start();
			}
		}

		//
		// Destructor. Will stop all the execution internally.
		//
		~HashDispatcher()
		{
			stop();
		}

		//
		// Start the dispatcher.
		//
		void start()
		{
			bool expected = false;

			if (runningFlag.compare_exchange_strong(expected, true))
			{
				for (TaskRunner<Mutex>& runner : runners)
				{
					runner.start();
				}
			}
		}

		//
		// Stop the dispatcher.
		//
		void stop()
		{
			bool expected = true;

			if (runningFlag.compare_exchange_strong(expected, false))
			{
				for (TaskRunner<Mutex>& runner : runners)
				{
					runner.stop();
				}
			}
		}

		//
		// Submit a task for the dispatcher to run.
		//
		// key : The key for the task.
		// runnable : The task to be executed.
		//
		void submit(const Key& key, const Runnable& runnable)
		{
			runners[hash(key) % runners.size()].submit(runnable);
		}

	private:

		// The hash function / object used.
		Hash hash;

		// The task runners.
		std::vector<TaskRunner<Mutex> > runners;

		// Flag if the dispatcher is running.
		std::atomic<bool> runningFlag;
	};

	// Define the globally used dispatcher type.
	typedef HashDispatcher<> Dispatcher;
}

#endif /* LIB_THREADING_DISPATCHER_HPP_ */
