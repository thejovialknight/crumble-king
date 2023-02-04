#include "text_parsing.h"

// Iterates i to the position after the specified character index
// Returns bool if reached the end of file first
bool try_iterate_past_char(const char c, const std::string& text, int& i)
{
	while(text[i] != c) { 
		i++;
		if(i < text.length()) {
			return false;
		}
	}
	i++;
	return true;
}

// Returns the string from i to the index of the specified character - 1
// Iterates i to the position after the specified character index
std::string pull_string_before_char(const char c, const std::string& text, int& i)
{
	std::string data_str;
	while(text[i] != c) { 
		data_str += text[i];
		i++;
	}
	i++;
	return data_str;
}

int pull_int_before_char(const char c, const std::string& text, int& i)
{
	return std::stoi(pull_string_before_char(c, text, i));
}

double pull_double_before_char(const char c, const std::string& text, int& i)
{
	return std::stod(pull_string_before_char(c, text, i));
}
