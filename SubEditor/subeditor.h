#pragma once
#include <string>
#include "ooptmpl.h"
#include "oopstr.h"

#define _for(x, s, e) for (int x = s; x < e; x++)
#define _fore(x, s, e) for (int x = s; x <= e; x++)

namespace oop {

	std::string itos_f(const int& i, int len);

	class Time
	{
		int hh, mm, ss, ms;
	public:
		Time(int h, int m, int s, int ms);
		Time(int ms);
		Time(std::string std);
		std::string as_str() const;
		void add_time(int ms);
		int as_ms() const;
	};

	class SubBlock
	{
		friend class SubtitleEditor;
	private:
		int index;
		Time start, end;
		std::string text;
	public:
		SubBlock(int index, const Time& start, const Time& end, const std::string& text);
		std::string as_str() const;
		std::string& get_text();
		void shift(int ms);
	};

	class SubtitleEditor
	{
		vector<SubBlock*> blocks;
		vector<SubBlock*> selection;
		vector<vector<SubBlock*>*> mem_states;

		void rearrange();
		void save_state();
	public:
		~SubtitleEditor();

		int loadSubtitles(std::string subtitles);
		std::string saveSubtitles() const;

		void addToSelection(int start, int end);
		void deselect();

		void boldText(int start, int end);
		void italicText(int start, int end);
		void underlineText(int start, int end);
		void removeStyle();

		void shiftForward(int ms);
		void shiftBackward(int ms);

		void insertSubtitle(int start, int length, const std::string& text);
		void deleteSubtitles();

		void merge(const std::string& subtitles);

		void fixLongLines(int max);
		void fixLongTime(int ms);

		void undo();
	};
}