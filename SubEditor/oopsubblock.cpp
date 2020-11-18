#include "subeditor.h"

#define NL "\n"
#define DNL "\n\n"
#define ARROW " --> "

namespace oop {

	SubBlock::SubBlock(int index, const Time& start, const Time& end, const std::string& text)
		: index(index), start(start), end(end), text(text) {}

	std::string SubBlock::as_str() const
	{
		return std::to_string(index) + NL +	start.as_str()
			+ ARROW + end.as_str() + NL + text + NL;
	}

	std::string& SubBlock::get_text()
	{
		return text;
	}

	void SubBlock::shift(int ms)
	{
		start.add_time(ms);
		end.add_time(ms);
	}

	pair<SubBlock*, int> read_block(std::string& subtitles)
	{
		// returns -1 for error, 0 for end of file, else number of lines
		if (subtitles.length() == 0)
			return pair<SubBlock*, int>(nullptr, 0);
		try {
			int i = std::stoi(get_token(NL, subtitles));
			Time s_time(get_token(ARROW, subtitles));
			Time e_time(get_token(NL, subtitles));
			std::string text = get_token(DNL, subtitles) + NL;

			if (text.find(DNL) != std::string::npos) throw 1;

			SubBlock* sb = new SubBlock(i, s_time, e_time, text);
			return pair<SubBlock*, int>(sb, subtitles.length());
		}
		catch (...) {
			return pair<SubBlock*, int>(nullptr, -1);
		}
	}

}