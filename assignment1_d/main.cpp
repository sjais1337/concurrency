#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <chrono> 
#include "config.h"
#include <thread>
#include "file_processor.h"
#include "thread_safe.h"
#include "logger.h"
#include <shared_mutex>

using namespace std;

void print_usage(const string& program_name)
{
    Logger::getInstance().logError("A grep-like tool with replacing capabilities. \n");
    Logger::getInstance().logError("USAGE:");
    Logger::getInstance().logError("  " + program_name + " [OPTIONS] <pattern> <file1> [file2]...");
    Logger::getInstance().logError("  " + program_name + " [OPTIONS] -r <replacement> <pattern> <file1> [file2]...");
    Logger::getInstance().logError("OPTIONS:");
    Logger::getInstance().logError("   -r, --replace <TEXT>   Enable find-and-replace mode.");
    Logger::getInstance().logError("   -i, --ignore-case      Perform case-insensitive matching.");
    Logger::getInstance().logError("   -n, --line-number      Prefix each line of output with its line number.");
    Logger::getInstance().logError("   -v, --invert-match     Select non-matching lines.");
    Logger::getInstance().logError("   -h, --help             Display this help message.");
}

void reporter(Shared& data){
    while (true) {
        
        std::shared_lock<std::shared_mutex> lock(data.data_mtx);
    
        if(data.complete) {
            break;
        }

        size_t total = data.total_occ;
        
        lock.unlock();

        Logger::getInstance().log("Total occurrences found so far: " + std::to_string(total)); 

        this_thread::sleep_for(chrono::milliseconds(100));
    }
}

int main(int argc, char* argv[])
{
    if(argc < 3)
    {
        print_usage(argv[0]);
        return 1;
    }

    Config config;
    vector<string> args(argv + 1, argv + argc);

    try{
        size_t i = 0;
        while(i < args.size())
        {
            const string& arg = args[i];
            if(arg == "-h" || arg == "--help")
            {
                print_usage(argv[0]);
                return 0;
            } 
            else if (arg == "-i" || arg == "--ignore-case") {
                config.ignore_case = true;
                i++;
            } 
            else if (arg == "-n" || arg == "--line-number") {
                config.line_number = true;
                i++;
            } 
            else if (arg == "-v" || arg == "--invert-match") {
                config.invert_match = true;
                i++;
            } 
            else if (arg == "-r" || arg == "--replace") {
                config.replace_mode = true;
                if(i+1 >= args.size()) 
                    throw runtime_error("Missing replacement text after " + arg);
                config.replacement = args[i+1];
                i += 2;
            } else if(arg[0] == '-')
            {
                throw runtime_error("Unknown flag: " + arg);
            }
            else {
                if(config.pattern.empty())
                {
                    config.pattern = arg;
                } else {
                    config.files.push_back(arg);
                }
                i++;
            }
        }

        if (config.pattern.empty()) throw runtime_error("Pattern not specified.");
        if (config.files.empty()) throw runtime_error("No input files specified."); 
    } 
    catch (const exception& e)
    {
        Logger::getInstance().logError("Argument Error: " + string(e.what()));
        print_usage(argv[0]);
        return 1;
    }
    
    auto start_pool = chrono::high_resolution_clock::now();
    vector<thread> threads;
    Shared shared_data;

    thread reporter_thread(reporter, ref(shared_data));

    for(const auto& file : config.files)
    {
        try {
            if(config.replace_mode)
            {
                execute_replace(file, config);
            }
            else
            {
        //      auto start_time = chrono::high_resolution_clock::now();
              threads.emplace_back(execute_search, file, ref(config), ref(shared_data));
        //      auto end_time = chrono::high_resolution_clock::now();
        //      chrono::duration<double, milli> elapsed = end_time - start_time;
        //      cout << "\n--- Processed " << file << " in " << elapsed.count() << " ms. ---" << endl;
            }
        }
        catch (const exception& e)
        {
            Logger::getInstance().logError("Error processing file " + file + ": " + e.what());
        }
    }

    for(auto& t: threads) {
        if(t.joinable()){
            t.join();
        }
    }

    {
        std::unique_lock<std::shared_mutex> data_lock(shared_data.data_mtx);
        shared_data.complete = true;
    }

    if(reporter_thread.joinable()){
        reporter_thread.join();
    }

    auto end_pool = chrono::high_resolution_clock::now();
    
    chrono::duration<double, milli> elapsed = end_pool - start_pool;
    Logger::getInstance().log("Total occurrences found: " + std::to_string(shared_data.total_occ));
    Logger::getInstance().log("Finished processing files in " + std::to_string(elapsed.count()) + " ms.");

    return 0;
}
