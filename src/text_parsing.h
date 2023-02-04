#pragma once
#include <string>

bool try_iterate_past_char(const char c, const std::string& text, int& i);
std::string pull_string_before_char(const char c, const std::string& text, int& i);
int pull_int_before_char(const char c, const std::string& text, int& i);
double pull_double_before_char(const char c, const std::string& text, int& i);
