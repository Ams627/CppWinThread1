#pragma once

template <typename T> class ConQueue
{
public:
	ConQueue() :
		_shutdown(false)
	{

	}

	void Enqueue(T item)
	{
		std::unique_lock<std::mutex> lock(_queue_mutex);
		bool was_empty = _queue.empty();
		_queue.push(std::move(item));
		lock.unlock();

		if (was_empty)
			_condition_variable.notify_one();
	}

	bool Dequeue(T& item)
	{
		std::unique_lock<std::mutex> lock(_queue_mutex);

		while (!_shutdown && _queue.empty())
			_condition_variable.wait(lock);

		if (!_shutdown)
		{
			item = std::move(_queue.front());
			_queue.pop();

			return true;
		}

		return false;
	}

	bool IsEmpty()
	{
		std::lock_guard<std::mutex> lock(_queue_mutex);
		return _queue.empty();
	}

	void Shutdown()
	{
		_shutdown = true;
		_condition_variable.notify_all();
	}

private:
	std::mutex _queue_mutex;
	std::condition_variable _condition_variable;
	std::queue<T> _queue;
	std::atomic<bool> _shutdown;
};
