#include "subeditor.h"

namespace oop {

	pair<SubBlock*, int> read_block(std::string&);

	int ceil(float t)
	{
		return (int)((int)t == t ? t : t + 1);
	}

	SubtitleEditor::~SubtitleEditor()
	{
		blocks.clear();
		mem_states.clear();
	}

	void SubtitleEditor::rearrange()
	{
		_for(i, 0, blocks.size())
			blocks[i]->index = i + 1;
	}

	int SubtitleEditor::loadSubtitles(std::string subtitles)
	{
		save_state();
		pair<SubBlock*, int> p(nullptr, -1);
		blocks.clear();
		int count = 1;
		while ((p = read_block(subtitles)).b > -1) {
			count++;
			blocks.add(p.a);
			if (p.b == 0) break;
		}
		if (p.b == -1) return count;
		return -1;
	}

	std::string SubtitleEditor::saveSubtitles() const
	{
		std::string r = "";
		_for(i, 0, blocks.size())
			r += blocks[i]->as_str();
		return r;
	}

	void SubtitleEditor::addToSelection(int start, int end)
	{
		start -= 1;
		end -= 1;
		if (start < 0) start = 0;
		if (end >= blocks.size()) end = blocks.size() - 1;
		_fore(i, start, end)
			if (selection.contains(blocks[i]) == -1)
				selection.add(blocks[i]);
	}

	void SubtitleEditor::deselect()
	{
		selection.clear();
	}

	void apply_dec(int start, int end, const std::string& sym, SubBlock& sb)
	{
		if (start < 0) start = 0;
		if (end >= sb.get_text().length()-1) end = sb.get_text().length() - 2;

		std::string dec = std::string("<") + sym + ">";
		sb.get_text().insert(start, dec);

		dec.insert(1, "/");
		sb.get_text().insert(end + dec.length(), dec);
	}

	void SubtitleEditor::boldText(int start, int end)
	{
		save_state();
		_for(i, 0, selection.size())
			apply_dec(start, end, "b", *selection[i]);
	}

	void SubtitleEditor::italicText(int start, int end)
	{
		save_state();
		_for(i, 0, selection.size())
			apply_dec(start, end, "i", *selection[i]);
	}

	void SubtitleEditor::underlineText(int start, int end)
	{
		save_state();
		_for(i, 0, selection.size())
			apply_dec(start, end, "u", *selection[i]);
	}

	void SubtitleEditor::removeStyle()
	{
		save_state();
		_for(i, 0, selection.size())
		{
			std::string& t = selection[i]->get_text();
			_for(l, 0, t.length())
			{
				if (t[l] == '<' && t[l + 2] == '>' &&
					(t[l+1]=='u'||t[l+1]=='b'||t[l+1]=='i')) {
					std::string block = t.substr(l, 3);
					block.insert(1, "/");

					t.erase(l, 3);
					t.erase(t.find(block), 4);
					break;
				}
			}
		}
	}

	void shift_list(vector<SubBlock*>& list, int ms)
	{
		_for(i, 0, list.size())
			list[i]->shift(ms);
	}

	void SubtitleEditor::shiftForward(int ms)
	{
		save_state();
		if (selection.size() == 0)
			shift_list(blocks, ms);
		else
			shift_list(selection, ms);
	}

	void SubtitleEditor::shiftBackward(int ms)
	{
		save_state();
		if (selection.size() == 0)
			shift_list(blocks, -ms);
		else
			shift_list(selection, -ms);
	}

	void SubtitleEditor::insertSubtitle(int start, int length, const std::string& text)
	{
		save_state();
		Time ts(start), te(start + length);
		SubBlock* sb = new SubBlock(-1, ts, te, text);
		bool inserted = false;
		_for(i, 0, blocks.size())
		{
			// can replace this with rearrange
			if (inserted)
				blocks[i]->index++;
			else {
				if (ts.as_ms() < blocks[i]->start.as_ms()) {
					sb->index = i + 1;
					blocks.insert(i, sb);
					inserted = true;
				}
			}
		}
		if (!inserted) {
			sb->index = blocks.size() + 1;
			blocks.add(sb);
		}
	}

	void SubtitleEditor::deleteSubtitles()
	{
		save_state();
		_for(i, 0, selection.size()) {
			_for(j, 0, blocks.size()) {
				if (selection[i] == blocks[j]) {
					delete blocks[j];
					blocks.remove(j);
					break;
				}
			}
		}
		selection.clear();
		rearrange();
	}

	void SubtitleEditor::merge(const std::string& subtitles)
	{
		save_state();
		SubtitleEditor temp;
		if (temp.loadSubtitles(subtitles) == -1) {
			_for (i, 0, temp.blocks.size()) {
				insertSubtitle(temp.blocks[i]->start.as_ms(),
					temp.blocks[i]->end.as_ms() - temp.blocks[i]->start.as_ms(), temp.blocks[i]->get_text());
			}
		}
		rearrange();
	}

	void SubtitleEditor::fixLongLines(int max)
	{
		save_state();
		vector<SubBlock*>& list = selection.size() > 0 ? selection : blocks;
		_for(i, 0, list.size()) {
			SubBlock& sb = *list[i];
			std::string& text = sb.get_text();

			int nb = 0, all = 1;
			_for(j, 0, text.length()) {
				if (text[j] != ' ') nb++;
				if (text[j] == '\n') nb = 0;
				if (nb > max) { all = 0; break; }
			}

			if (!all) {

				str_rep_all("\n", " ", text);
				int ll = 0, lb = -1;

				_for(j, 0, text.length()) {
					if (text[j] == ' ') { lb = j; ll--; };
					ll++;
					if (ll > max) {
						text[lb] = '\n';
						ll = j - lb;
					}
				}

				text[text.length() - 1] = '\n';
			}
		}
	}

	void SubtitleEditor::fixLongTime(int ms)
	{
		save_state();
		vector<SubBlock*>& list = selection.size() > 0 ? selection : blocks;
		_for(i, 0, list.size()) {
			SubBlock& sb = *list[i];
			int len = sb.end.as_ms() - sb.start.as_ms();
			if (len <= ms) continue;

			int new_block_amount = len / ms + 1;
			int time_per_block = len / new_block_amount;

			std::string tokenable = sb.text;
			str_rep_all("\n", " ", tokenable);
			int words = count_split(" ", tokenable);

			int words_per_block = ceil(words / (float)new_block_amount);

			_for(b, 0, new_block_amount) {
				std::string block_text = "";
				int word_amount = (b == new_block_amount - 1) ? (words - b * words_per_block) : words_per_block;
				_for(w, 0, word_amount)
					block_text += get_token(" ", tokenable) + " ";
				block_text[block_text.length() - 1] = '\n';
				insertSubtitle(sb.start.as_ms() + b * time_per_block, time_per_block, block_text);
				delete mem_states[mem_states.size() - 1];
				mem_states.remove(mem_states.size() - 1);
			}
			blocks.remove(sb.index - 1);
		}
		rearrange();
	}

	void SubtitleEditor::save_state()
	{
		vector<SubBlock*>* temp = new vector<SubBlock*>();
		_for(i, 0, blocks.size()) {
			SubBlock* new_mem_block = (SubBlock*)malloc(sizeof(SubBlock));
			memcpy(new_mem_block, blocks[i], sizeof(SubBlock));
			temp->add(new_mem_block);
		}
		mem_states.add(temp);
	}

	void SubtitleEditor::undo()
	{
		if (mem_states.size() == 0) return;
		blocks.clear();
		vector<SubBlock*>& mem_state = *mem_states[mem_states.size() - 1];
		_for(i, 0, mem_state.size()) {
			SubBlock* new_mem_block = (SubBlock*)malloc(sizeof(SubBlock));
			memcpy(new_mem_block, mem_state[i], sizeof(SubBlock));
			blocks.add(new_mem_block);
		}
		delete &mem_state;
		mem_states.remove(mem_states.size() - 1);
	}

}