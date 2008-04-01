#ifndef _SIMPLE_WINDOW_H_
#define _SIMPLE_WINDOW_H_

#include <nds.h>
#include "amigawindow.h"

using namespace std;

class Button;
class BitmapButton;
class Textbox;
class MultiLineTextBox;
class SuperBitmap;
class RadioButtonGroup;
class CheckBox;

class SimpleWindow : public AmigaWindow {

public:
	SimpleWindow(s16 x, s16 y, u16 width, u16 height, char* title, u32 flags, FontBase* font = NULL);
	virtual ~SimpleWindow();

	virtual Button* newButton(s16 x, s16 y, u16 width, u16 height, char* text);
	virtual BitmapButton* newBitmapButton(s16 x, s16 y, u16 width, u16 height, u16 bitmapX, u16 bitmapY, u16 bitmapWidth, u16 bitmapHeight, const u16* bitmapNormal, const u16* bitmapClicked);
	virtual Textbox* newTextbox(s16 x, s16 y, u16 width, u16 height, char* text);
	virtual MultiLineTextBox* newMultiLineTextBox(s16 x, s16 y, u16 width, u16 height, u8 maxRows, char* text);
	virtual SuperBitmap* newSuperBitmap(s16 x, s16 y, u16 width, u16 height, u32 bitmapWidth, u32 bitmapHeight, bool isDecoration);
	virtual RadioButtonGroup* newRadioButtonGroup(s16 x, s16 y);
	virtual CheckBox* newCheckBox(s16 x, s16 y, u16 width, u16 height);
};

#endif
