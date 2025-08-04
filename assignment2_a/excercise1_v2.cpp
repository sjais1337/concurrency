#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <memory>
#include <utility>
#include <mutex>
#include <random>

struct DeliveryOrder {
  int order_id;
  std::string destination;

  std::string execute() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(500, 2000);
    int ms = distrib(gen);
    std::this_thread::sleep_for(std::chrono::milliseconds(ms)); 
    return "Order " + std::to_string(order_id) + " delivered at " + destination + "."; 
  }
};


class Driver;

Driver find_driver(std::unique_ptr<DeliveryOrder> order, std::vector<std::string>& logbook, std::mutex& mtx);

class Driver {
private:
  std::thread worker_thread;
  std::unique_ptr<DeliveryOrder> order_details;

  // optional design choice to prevent calling the Driver constructor from main.
  friend Driver find_driver(std::unique_ptr<DeliveryOrder> order, std::vector<std::string>& logbook, std::mutex& mtx);
  explicit Driver(std::unique_ptr<DeliveryOrder> order, std::vector<std::string>& logbook, std::mutex& mtx)
  {
    // worker_thread = std::thread(&DeliveryOrder::execute, order_details.get());

    // Using lambdas, we have the following. This is a better approach,since 
    // without using lambdas we cannot have the output of the order execution in
    // a logbook properly. It is still possible, and is given as a excercise.
    // Note however, it would require a complete overhaul in the patttern.

    worker_thread = std::thread(
      [order = std::move(order), &logbook, &mtx]() {
        {
            std::lock_guard<std::mutex> lock(mtx);
            std::cout << "Dispatching order " << order->order_id << " to " << order->destination << "." << std::endl;
        } 
        std::string result = order->execute(); 
        {
            std::lock_guard<std::mutex> lock(mtx);
            logbook.push_back(result);
        }
      }
    );
  }

public:
  ~Driver(){
    if(worker_thread.joinable()){
      worker_thread.join();
    }
  }

  Driver(const Driver&) = delete;
  Driver& operator=(const Driver&) = delete;

  // Not necessary, since the default move operations are sufficient, but provided for completeness.
  Driver(Driver&& other) noexcept
    : worker_thread(std::move(other.worker_thread)), order_details(std::move(other.order_details)){}

  Driver& operator=(Driver&& other) noexcept {
    if(this != &other) {
      if(worker_thread.joinable()){
        worker_thread.join();
      }

      worker_thread = std::move(other.worker_thread);
      order_details = std::move(other.order_details);
    }
    return *this;
  }
};


Driver find_driver(std::unique_ptr<DeliveryOrder> order, std::vector<std::string>& logbook, std::mutex& mtx) {
  return Driver(std::move(order), logbook, mtx);
}

int main() {
  std::cout << "Starting Program..." << std::endl;

  std::vector<std::string> parent_logbook;
  std::mutex mtx; 

  const int num_threads = std::thread::hardware_concurrency(); 

  std::vector<Driver> drivers;

  drivers.reserve(num_threads);
  
  std::cout << "\n--- Creating " << num_threads << " orders ---" << std::endl;

  for(int i = 0; i < num_threads; i++){
    auto order = std::make_unique<DeliveryOrder>();
    order->order_id=i;
    order->destination = "Sector " + std::to_string(i);
    drivers.push_back(find_driver(std::move(order), parent_logbook, mtx));
  }

  // Note that here, you do not have to do joins, since the thread manager handles it for you.

  drivers.clear();
  std::cout << "\n--- All drivers have completed their work and returned. ---" << std::endl;

  std::cout << "\n--- Logbook Contents ---" << std::endl;

  for(const auto& entry: parent_logbook){
    std::cout << entry << std::endl;
  }

  std::cout << "Finished." << std::endl;

  return 0;
} 

