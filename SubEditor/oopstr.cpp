#include "oopstr.h"

namespace oop {

	void str_rep_all(const std::string& pattern, const std::string& val, std::string& str)
	{
		for (int i = 0; i <= str.length() - pattern.length(); i++) {
			if (str.substr(i, pattern.length()) != pattern) continue;
			str.erase(i, pattern.length());
			str.insert(i, val);
			i += val.length() - 1;
		}
	}

	std::string get_token(const std::string& delimiter, std::string& str)
	{
		size_t pos = str.find(delimiter);
		if (pos == std::string::npos)
			return str;
		std::string r = str.substr(0, pos);
		int pp = pos + delimiter.length();
		str = str.substr(pp, str.length() - pp);
		return r;
	}

	int count_split(const std::string& delimiter, const std::string& str)
	{
		int len = 1;
		size_t pos = 0;
		while ((pos = str.find(delimiter, pos) + 1) != std::string::npos && pos < str.length()) len++;
		return len;
	}
}