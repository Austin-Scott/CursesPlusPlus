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

const Cursor cursor::moveTo(int row, int col)
{
	Cursor result = Cursor();
	result.type = Cursor::Type::Move;
	result.row = row;
	result.col = col;
	return result;
}

const Cursor cursor::attributeOn(Attr attr)
{
	Cursor result = Cursor();
	result.type = Cursor::Type::AttributesOn;
	result.attrMask = attrTable.at(attr);
	return result;
}

const Cursor cursor::attributesOn(std::vector<Attr> attrs)
{
	Cursor result = Cursor();
	result.type = Cursor::Type::AttributesOn;
	for (Attr attr : attrs) {
		result.attrMask |= attrTable.at(attr);
	}
	return result;
}

const Cursor cursor::attributeOff(Attr attr)
{
	Cursor result = Cursor();
	result.type = Cursor::Type::AttributesOff;
	result.attrMask = attrTable.at(attr);
	return result;
}

const Cursor cursor::attributesOff(std::vector<Attr> attrs)
{
	Cursor result = Cursor();
	result.type = Cursor::Type::AttributesOff;
	for (Attr attr : attrs) {
		result.attrMask |= attrTable.at(attr);
	}
	return result;
}

const Cursor cursor::setColor(Color foreground, Color background)
{
	Cursor result = Cursor();
	result.type = Cursor::Type::SetColor;
	result.foreground = foreground;
	result.background = background;
	return result;
}