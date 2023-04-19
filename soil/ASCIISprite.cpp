#include "ASCIISprite.h"

#include "utfcpp/source/utf8.h"

Soil::ASCIISprite::ASCIISprite(std::string text, Soil::ASCIIColor color) : color{ color } {
	this->text = utf8::utf8to32(text);
}

Soil::ASCIISprite::ASCIISprite(std::u32string text, Soil::ASCIIColor color) : color{ color }, text{ text } {
}
