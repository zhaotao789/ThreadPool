#include <iostream>
#include <vector>
#include <chrono>
#include "ThreadPool.h"

int main() {
	// Without ThreadPool
	auto start = std::chrono::steady_clock::now();
	std::vector<int> results;

	for (int i = 0; i < 100; ++i) {
		std::this_thread::sleep_for(std::chrono::seconds(1));
		results.push_back(i * i);
	}

	auto end = std::chrono::steady_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
	std::cout << "Without ThreadPool: " << duration << " seconds" << std::endl;

	// With ThreadPool
	start = std::chrono::steady_clock::now();
	ThreadPool pool(100);
	std::vector<std::future<int>> threadResults;

	for (int i = 0; i < 100; ++i) {
		threadResults.emplace_back(
			pool.enqueue([i] {
			std::this_thread::sleep_for(std::chrono::seconds(1));
			return i * i;
		})
		);
	}

	for (auto&& result : threadResults)
		std::cout << result.get() << ' ';
	std::cout << std::endl;

	end = std::chrono::steady_clock::now();
	duration = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
	std::cout << "With ThreadPool: " << duration << " seconds" << std::endl;

	return 0;
}
