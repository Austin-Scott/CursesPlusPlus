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

private:
	const Cursor* nextInstruction = nullptr;
	Type type;
	std::string text;
	int row, col;
	int attrMask;
	Color foreground;
	Color background;

public:
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

	static const Cursor moveTo(int row, int col);
	static const Cursor endl;
	static const Cursor clearToEndOfLine;
	static const Cursor clearToEndOfWindow;
	static const Cursor attributeOn(Attr attr);
	static const Cursor attributesOn(std::vector<Attr> attrs);
	static const Cursor attributeOff(Attr attr);
	static const Cursor attributesOff(std::vector<Attr> attrs);
	static const Cursor setColor(Color foreground, Color background);
};