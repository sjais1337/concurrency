#include <iostream>
#include <vector>
#include <thread>
#include <numeric>
#include <chrono>
#include <mutex>

std::mutex mtx;

void singleThreaded(long long iterations);
void multiThreaded(long long iterations, int threads);



int main (int argc, char *argv[]) {
  const long long iterations = 1'000'000'000;
  std::cout << "Available threads " << std::max(1u, std::thread::hardware_concurrency()) << "\n" << std::endl;
  singleThreaded(iterations);
  std::cout << "\nRunning multi-threaded loop.\n" << std::endl;
  multiThreaded(iterations, 1);
  multiThreaded(iterations, 2);
  multiThreaded(iterations, 4);
  multiThreaded(iterations, 6);
  multiThreaded(iterations, 8);
  multiThreaded(iterations, 10);
  multiThreaded(iterations, 12);
  multiThreaded(iterations, 14);
  multiThreaded(iterations, 16);
  multiThreaded(iterations, 20);
  multiThreaded(iterations, 32);
  multiThreaded(iterations, 64);
  multiThreaded(iterations, 128);
  multiThreaded(iterations, 256);
  multiThreaded(iterations, 1024);
  multiThreaded(iterations, 4096);
  return 0;
}

void singleThreaded(long long iterations)
{
  auto start_time = std::chrono::high_resolution_clock::now();

  long long sum = 0;
  for(int i = 0; i < iterations; i++)
  {
    sum++;
  }

  auto end_time = std::chrono::high_resolution_clock::now();

  std::chrono::duration<double>time_difference = end_time - start_time;
  std::cout << "(No Threading) Time over single run: " << 1000*time_difference.count() << "ms" << std::endl;
}

void workerFunction(long long start, long long end, long long& total_sum)
{
  long long partial_sum = 0;

  for (long long i = start; i <= end; i++)
  {
    partial_sum += i;
  }
  
  std::lock_guard<std::mutex> lock(mtx);
  total_sum = total_sum + partial_sum; 
}

void multiThreaded(long long iterations, int thread_count)
{
  const int num_runs = 15;
  double total_duration_seconds = 0.0;

  for (int run = 0; run < num_runs; ++run) {
    auto start_time = std::chrono::high_resolution_clock::now();

    std::vector<std::thread> threads;
    threads.reserve(thread_count);

    long long total_sum = 0; 
    long long chunk_size = iterations / thread_count;
    long long start = 1;

    for(int i = 0; i < thread_count; i++){
        long long end = start + chunk_size - 1;

        if(i == thread_count - 1){
            end = iterations;
        }
        
        threads.emplace_back(workerFunction, start, end, std::ref(total_sum));

        start = end + 1;
    }
    
    for(auto& t: threads)
    {
        t.join();
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> time_difference = end_time - start_time;
    total_duration_seconds += time_difference.count();
  }

  double average_seconds = total_duration_seconds / num_runs;

  std::cout << "(" << thread_count << " Threads) Average time over " << num_runs << " runs: " << average_seconds * 1000.0 << " ms" << std::endl;
}
