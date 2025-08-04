#include "file_processor.h"
#include "logger.h"
#include <chrono>
#include <string>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <cctype>
#include <mutex>
#include <thread> 
using namespace std;

static string to_lower(const string& str) {
    string lower_str = str;
    transform(lower_str.begin(), lower_str.end(), lower_str.begin(), [](unsigned char c) { return tolower(c); } );
    return lower_str;
}

static string replace_all(string source, const string& from, const string& to)
{
    string new_string;
    new_string.reserve(source.length());
    string::size_type last_pos = 0;
    string::size_type find_pos;

    while((find_pos = source.find(from, last_pos)) != string::npos)
    {
        new_string.append(source, last_pos, find_pos - last_pos);
        new_string += to;
        last_pos = find_pos + from.length();
    }
    
    new_string += source.substr(last_pos);
    return new_string;
}

void execute_search(const string& filename, const Config& config, Shared& data) {
    auto start = chrono::high_resolution_clock::now();
    ifstream file(filename);

    if(!file.is_open()) {
        Logger::getInstance().logError("Warning: Could not open file " + filename);
        return;
    }

    string line;
    size_t line_number = 0;
    bool print_filename = config.files.size() > 1;
    size_t count = 0;

    string pattern = config.ignore_case ? to_lower(config.pattern) : config.pattern;

    while(getline(file, line)) {
        line_number++;
        string modified_line = config.ignore_case ? to_lower(line) : line;
        string::size_type last_pos = 0, find_pos; 
        while((find_pos = modified_line.find(pattern, last_pos)) != string::npos) {
            count++;
            last_pos = find_pos + pattern.size();
        }
    }

    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start).count();

    std::unique_lock<std::shared_mutex> data_lock(data.data_mtx);
    data.total_occ += count;
    data_lock.unlock();
    
    Logger::getInstance().log("Found " + to_string(count) + " occurrences in " + filename);
    Logger::getInstance().log("Processed " + filename + " in " + to_string(duration) + " ms");
}


void execute_replace(const string& filename, const Config& config)
{
    ifstream infile(filename);
    if(!infile.is_open())
    {
        Logger::getInstance().logError("Warning: Could not open file for reading: " + filename);
        return;
    }

    string temp_filename = filename + ".tmp";
    ofstream outfile(temp_filename);

    if(!outfile.is_open())
    {
        Logger::getInstance().logError("Error: Could not create temporary file for writing.");
        return;
    }

    string line;
    bool changed = false;

    while(getline(infile, line))
    {
        if(line.find(config.pattern) != string::npos)
        {
            string modified = replace_all(line, config.pattern, config.replacement);
            outfile << modified << endl;
            changed = true;
        } else {
            outfile << line << endl;
        }
    }

    infile.close();
    outfile.close();

    if(changed)
    {
        if(remove(filename.c_str()) != 0)
        {
            Logger::getInstance().logError("Error: Could not remove original file.");
            return;
        }
        
        if(rename(temp_filename.c_str(), filename.c_str()) != 0)
        {
            Logger::getInstance().logError("Error: Could not rename temporary file.");
        }
        else
        {
            Logger::getInstance().log("Replaced matches in: " + filename);
        }
    } else {
        remove(temp_filename.c_str());
        Logger::getInstance().log("No matches found in: " + filename);
    }
}
