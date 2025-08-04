#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <chrono> 
#include "config.h"
#include <thread>
#include "file_processor.h"

using namespace std;

void print_usage(const string& program_name)
{
    cerr << "A grep-like tool with replacing capabilities. \n \n";
    cerr << "USAGE:\n";
    cerr << "  " << program_name << " [OPTIONS] <pattern> <file1> [file2]...\n";
    cerr << "  " << program_name << " [OPTIONS] -r <replacement> <pattern> <file1> [file2]...\n";
    cerr << "OPTIONS:\n";
    cerr << "   -r, --replace <TEXT>    Enable find-and-replace mode.\n";
    cerr << "   -i, --ignore-case       Perform case-insensitive matching.\n";
    cerr << "  -n, --line-number      Prefix each line of output with its line number.\n";
    cerr << "  -v, --invert-match     Select non-matching lines.\n";
    cerr << "  -h, --help             Display this help message.\n";
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
        cerr << "Argument Error: " << e.what() << endl;
        print_usage(argv[0]);
        return 1;
    }
    
    auto start_pool = chrono::high_resolution_clock::now();
    vector<thread> threads;

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
              threads.emplace_back(execute_search, file, ref(config));
        //      auto end_time = chrono::high_resolution_clock::now();
        //      chrono::duration<double, milli> elapsed = end_time - start_time;
        //      cout << "\n--- Processed " << file << " in " << elapsed.count() << " ms. ---" << endl;
            }
        }
        catch (const exception& e)
        {
            cerr << "Error processing file " << file << ": " << e.what() << endl;
        }
    }

    for(auto& t: threads) {
      t.join();
    }
    auto end_pool = chrono::high_resolution_clock::now();
    
    chrono::duration<double, milli> elapsed = end_pool - start_pool;
    
    cout << "Finished processing files in " << elapsed.count() << " ms." << endl; 

    return 0;
}
