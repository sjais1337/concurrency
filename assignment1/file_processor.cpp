#include "file_processor.h"

#include <string>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <cctype>

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


void execute_search(const string& filename, const Config& config)
{
    ifstream file(filename);

    if(!file.is_open())
    {
        cerr << "Warning: Could not open file: " << filename << endl;
        return;
    }

    string line;
    int line_num = 0;
    bool print_filename = config.files.size() > 1;

    string pattern_to_search = config.ignore_case ? to_lower(config.pattern) : config.pattern;


    while(getline(file, line))
    {
        line_num++;
        string line_to_search = config.ignore_case ? to_lower(line): line;
        
        bool found = (line_to_search.find(pattern_to_search) != string::npos);

        if(found != config.invert_match)
        {
            if(print_filename)
            {
                cout << filename << ": ";
            }
            if(config.line_number)
            {
                cout << line_num << ": ";
            }
           // cout << line << endl;
        }
    }
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
