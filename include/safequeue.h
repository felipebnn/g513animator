#pragma once

#include <condition_variable>
#include <mutex>
#include <queue>

template <class T>
class SafeQueue {
	std::queue<T> q;
	std::mutex m;
	std::condition_variable cv;

public:
	void push(T v) {
		std::lock_guard<std::mutex> lock(m);
		q.push(std::move(v));
		cv.notify_one();
	}

	T pop() {
		std::unique_lock<std::mutex> lock(m);
		cv.wait(lock, [=] { return !q.empty(); });

		T v = q.front();
		q.pop();
		return v;
	}

	void clear() {
		std::lock_guard<std::mutex> lock(m);
		q = {};
	}
};
