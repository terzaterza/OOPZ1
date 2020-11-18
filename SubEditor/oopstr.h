#pragma once
#include <string>

namespace oop {

	void str_rep_all(const std::string& pattern, const std::string& val, std::string& str);
	std::string get_token(const std::string& delimiter, std::string& str);
	int count_split(const std::string& delimiter, const std::string& str);

}