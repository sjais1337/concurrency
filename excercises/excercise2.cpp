#include <iostream>
#include <vector>
#include <mutex>
#include <thread>
#include <chrono>
#include <string>

class Account {
private:
    double balance;
    std::mutex mtx;
    std::string account_name;
public:
    explicit Account(double balance, const std::string& name) : balance(balance), account_name(name) {}

    Account(const Account&) = delete;
    Account& operator=(const Account&) = delete;

    void withdraw(double amount) {
        std::lock_guard<std::mutex> lock(mtx);
        if (amount > balance) {
            throw std::runtime_error("Insufficient funds");
        }
        balance -= amount;
    }

    void deposit(double amount) {
        std::lock_guard<std::mutex> lock(mtx);
        balance += amount;
    }

    double checkBalance() {
        std::lock_guard<std::mutex> lock(mtx);
        return balance;
    }

    friend void transfer(Account& from, Account& to, double amount);
};

void transfer(Account& from, Account& to, double amount) {
    std::cout << "Thread " << std::this_thread::get_id() << ": Transferring " << amount << " from " << from.account_name << " to " << to.account_name << std::endl;
    std::lock_guard<std::mutex> lock_from(from.mtx);
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    std::cout << "Thread " << std::this_thread::get_id() << ": Acquired lock on " << from.account_name << std::endl;
    std::lock_guard<std::mutex> lock_to(to.mtx);

    from.balance -= amount;
    to.balance += amount;
    std::cout << "Thread " << std::this_thread::get_id() << ": Transfer complete." << std::endl;
}

int main(){
  Account account1(1000.0, "a");
  Account account2(1000.0, "b");

  std::cout << "Initial Balances: " << std::endl;
  std::cout << "Account 1: " << account1.checkBalance() << std::endl;
  std::cout << "Account 2: " << account2.checkBalance() << std::endl;

  std:: thread t1([&]() {
    transfer(account1, account2, 10.0);
  });
  std::thread t2([&]() {
    transfer(account2, account1, 20.0);
  });

  t1.join();
  t2.join();

  std::cout << "Final Balances: " << std::endl;
  std::cout << "Account 1: " << account1.checkBalance() << std::endl;
  std::cout << "Account 2: " << account2.checkBalance() << std::endl;

  return 0;
}