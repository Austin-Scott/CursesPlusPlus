#pragma once
#include <string>
#include <vector>

enum Attr {
	Normal,
	Standout,
	Underline,
	Reverse,
	Blink,
	Dim,
	Bold,
	Protect,
	Invisible,
	AltCharacterSet
};

enum Color {
	Black,
	Red,
	Green,
	Yellow,
	Blue,
	Magenta,
	Cyan,
	White
};

class Cursor {
public:
	enum Type {
		Empty,
		Text,
		AttributesOn,
		AttributesOff,
		SetColor,
		Move,
		EndLine,
		ClearToEndOfLine,
		ClearToEndOfWindow
	};

	Type type;
	std::string text;
	int row, col;
	int attrMask;
	Color foreground;
	Color background;

	friend class Window;

	Cursor() { type = Type::Empty; }
	template<class T> Cursor(T value) {
		type = Type::Text;
		text = std::to_string(value);
	}
	Cursor(const char* value) {
		type = Type::Text;
		text = std::string(value);
	}
};

namespace cursor {
	const Cursor moveTo(int row, int col);
	const Cursor attributeOn(Attr attr);
	const Cursor attributesOn(std::vector<Attr> attrs);
	const Cursor attributeOff(Attr attr);
	const Cursor attributesOff(std::vector<Attr> attrs);
	const Cursor setColor(Color foreground, Color background);

	const Cursor endl = []() {
		Cursor result = Cursor();
		result.type = Cursor::Type::EndLine;
		return result;
	}();

	const Cursor clearToEndOfLine = []() {
		Cursor result = Cursor();
		result.type = Cursor::Type::ClearToEndOfLine;
		return result;
	}();

	const Cursor clearToEndOfWindow = []() {
		Cursor result = Cursor();
		result.type = Cursor::Type::ClearToEndOfWindow;
		return result;
	}();
}