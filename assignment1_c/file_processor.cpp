#include "file_processor.h"
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
        lock_guard<mutex> lock(data.out_mtx);
        cerr << "Warning: Could not open file " << filename << endl;
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

    lock_guard<mutex> data_lock(data.data_mtx);
    data.total_occ += count;
    lock_guard<mutex> out_lock(data.out_mtx);
    cout << "--- Found " << count << " occurrences ---" << endl; 
    cout << "--- Processed " << filename << " in " << duration << " ms ---" << endl;
}


void execute_replace(const string& filename, const Config& config)
{
    ifstream infile(filename);
    if(!infile.is_open())
    {
        cerr << "Warning: Could not open file for reading: " << filename << endl;
        return;
    }

    string temp_filename = filename + ".tmp";
    ofstream outfile(temp_filename);

    if(!outfile.is_open())
    {
        cerr << "Error: Could not create temporary file for writing." << endl;
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
            cerr << "Error: Could not remove original file." << endl;
            return;
        }
        
        if(rename(temp_filename.c_str(), filename.c_str()) != 0)
        {
            cerr << "Error: Could not rename temporary file." << endl;
        }
        else
        {
            cout << "Replaced matches in: " << filename << std::endl;
        }
    } else {
        remove(temp_filename.c_str());
        cout << "No matches found in: " << filename << endl;
    }
}
