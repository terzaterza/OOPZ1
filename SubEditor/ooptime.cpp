#include "subeditor.h"
#include "oopstr.h"

#define MS_IN_HOUR 3600000
#define MS_IN_MINUTE 60000
#define MS_IN_SECOND 1000

namespace oop {

	Time::Time(int h, int m, int s, int ms)
		: hh(h), mm(m), ss(s), ms(ms) {}

	Time::Time(int ms)
		: hh(0), mm(0), ss(0), ms(0)
	{
		add_time(ms);
	}

	Time::Time(std::string str)
	{
		hh = std::stoi(get_token(":", str));
		mm = std::stoi(get_token(":", str));
		ss = std::stoi(get_token(",", str));
		ms = std::stoi(str);
	}

	std::string itos_f(const int& i, int len);
	std::string Time::as_str() const
	{
		return itos_f(hh, 2) + ":" + itos_f(mm, 2) + ":"
			+ itos_f(ss, 2) + "," + itos_f(ms, 3);
	}

	int Time::as_ms() const
	{
		int m = hh * 60 + mm;
		int s = m * 60 + ss;
		return s * 1000 + ms;
	}

	void Time::add_time(int ms_add)
	{
		int nms = as_ms() + ms_add;
		if (nms < 0) nms = 0;
		hh = nms / MS_IN_HOUR;
		nms %= MS_IN_HOUR;
		mm = nms / MS_IN_MINUTE;
		nms %= MS_IN_MINUTE;
		ss = nms / MS_IN_SECOND;
		ms = nms % MS_IN_SECOND;
	}

	std::string itos_f(const int& i, int len)
	{
		std::string res, is = std::to_string(i);
		_for (i, 0, (int)(len - is.length())) res += "0";
		return res + is;
	}
}