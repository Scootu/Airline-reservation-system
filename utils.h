#pragma once

#ifndef AIRLINESSYSTEM_UTILS_H_
#define AIRLINESSYSTEM_UTILS_H

#include <cassert>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <string>
#include <sstream>
#include <ctime>
#include <vector>
#include <unicode/unistr.h>
#include <unicode/ucnv.h> // for utf-8 conversion

using namespace std;

int ReadInt(int low, int high, bool cancel_choice_allowed = false)
{ // Added parameter. Respect backward compatibility
    if (!cancel_choice_allowed)
        cout << "\nEnter number in range " << low << " - " << high << ": ";
    else
        cout << "\nEnter -1 to cancel or number in range " << low << " - " << high << ": ";

    int value;

    cin >> value;

    if (cancel_choice_allowed && value == -1)
        return value;

    if (low <= value && value <= high)
        return value;

    cout << "ERROR: invalid number...Try again\n";
    return ReadInt(low, high);
}

int ShowReadMenu(const vector<string> &choices, string header = "Menu")
{ // Added parameter. Respect backward compatibility
    cout << "\n"
         << header << ":\n";
    for (int ch = 0; ch < (int)choices.size(); ++ch)
    {
        cout << "\t" << ch + 1 << ": " << choices[ch] << "\n";
    }
    return ReadInt(1, choices.size());
}

string GetCurrentTimeDate()
{ // src: https://stackoverflow.com/questions/17223096/outputting-date-and-time-in-c-using-stdchrono
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X");
    return ss.str();
}

int get_total_days(const std::string &date_time_from, const std::string &date_time_to)
{
    auto parse_date = [](const std::string &date_str) -> std::tm
    {
        std::tm tm_date = {};
        std::istringstream ss(date_str);
        std::string day, month, year;

        std::getline(ss, day, '-');
        std::getline(ss, month, '-');
        std::getline(ss, year);

        tm_date.tm_mday = std::stoi(day);
        tm_date.tm_mon = std::stoi(month) - 1;           // months are 0-11
        tm_date.tm_year = 2000 + std::stoi(year) - 1900; // year since 1900

        return tm_date;
    };

    std::tm tm_from = parse_date(date_time_from);
    std::tm tm_to = parse_date(date_time_to);

    std::time_t time_from = std::mktime(&tm_from);
    std::time_t time_to = std::mktime(&tm_to);

    double seconds = std::difftime(time_to, time_from);
    return static_cast<int>(seconds / (60 * 60 * 24));
}

std::string WCharToString(const SQLWCHAR *wstr)
{
    // Convert wchar_t* (SQLWCHAR*) to ICU UnicodeString
    icu::UnicodeString ustr(reinterpret_cast<const UChar *>(wstr));

    // Convert to UTF-8 std::string
    std::string result;
    ustr.toUTF8String(result);
    return result;
}
#endif