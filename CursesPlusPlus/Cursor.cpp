#include "Cursor.h"
#include <map>
#include <curses.h>

const std::map<Attr, chtype> attrTable = {
	{Attr::Normal, A_NORMAL},
	{Attr::Standout, A_STANDOUT},
	{Attr::Underline, A_UNDERLINE},
	{Attr::Reverse, A_REVERSE},
	{Attr::Blink, A_BLINK},
	{Attr::Dim, A_DIM},
	{Attr::Bold, A_BOLD},
	{Attr::Protect, A_PROTECT},
	{Attr::Invisible, A_INVIS},
	{Attr::AltCharacterSet, A_ALTCHARSET}
};

const Cursor Cursor::moveTo(int row, int col)
{
	Cursor result = Cursor();
	result.type = Type::Move;
	result.row = row;
	result.col = col;
	return result;
}

const Cursor Cursor::attributeOn(Attr attr)
{
	Cursor result = Cursor();
	result.type = Type::AttributesOn;
	result.attrMask = attrTable.at(attr);
	return result;
}

const Cursor Cursor::attributesOn(std::vector<Attr> attrs)
{
	Cursor result = Cursor();
	result.type = Type::AttributesOn;
	for (Attr attr : attrs) {
		result.attrMask |= attrTable.at(attr);
	}
	return result;
}

const Cursor Cursor::attributeOff(Attr attr)
{
	Cursor result = Cursor();
	result.type = Type::AttributesOff;
	result.attrMask = attrTable.at(attr);
	return result;
}

const Cursor Cursor::attributesOff(std::vector<Attr> attrs)
{
	Cursor result = Cursor();
	result.type = Type::AttributesOff;
	for (Attr attr : attrs) {
		result.attrMask |= attrTable.at(attr);
	}
	return result;
}

const Cursor Cursor::setColor(Color foreground, Color background)
{
	Cursor result = Cursor();
	result.type = Type::SetColor;
	result.foreground = foreground;
	result.background = background;
	return result;
}

const Cursor Cursor::endl = []() {
	Cursor result = Cursor();
	result.type = Type::EndLine;
	return result;
}();

const Cursor Cursor::clearToEndOfLine = []() {
	Cursor result = Cursor();
	result.type = Type::ClearToEndOfLine;
	return result;
}();

const Cursor Cursor::clearToEndOfWindow = []() {
	Cursor result = Cursor();
	result.type = Type::ClearToEndOfWindow;
	return result;
}();
