/*#include <iostream>
#include "subeditor.h"

int main() {
	std::string input = "1\n00:26 : 44, 019 --> 00:26 : 48, 230\nAll right, to a night\nwe'll never remember...\n\n2\n00 : 26 : 50, 066 --> 00:26 : 52, 943\n...but the four of us\nwill never forget.\n\n";
	std::string inp2 = "1\n00:00:47,255 --> 00:00:49,882\nDOUG [ON RECORDING]: <i>You've reached\nDoug. Sorry I missed your call.</i>\n\n2\n00:00:50,050 --> 00:00:52,217\n<i>Please leave a name and number\nand I'll get back to you.</i>\n\n";
	oop::SubtitleEditor se;
	se.loadSubtitles(input);
	se.insertSubtitle(1606000, 1500, "Gas");
	se.addToSelection(1, 2);
	se.deleteSubtitles();
	se.merge(inp2);
	se.addToSelection(1, 1);
	se.deleteSubtitles();
	se.fixLongLines(20);
	se.fixLongTime(1000);
	std::cout << se.saveSubtitles();
	return 0;
}*/