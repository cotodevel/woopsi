#include "radiobuttongroup.h"
#include "radiobutton.h"

RadioButtonGroup::RadioButtonGroup(s16 x, s16 y, FontBase* font) : Gadget(x, y, 0, 0, GADGET_BORDERLESS, font) {
	_selectedGadget = NULL;
}

RadioButtonGroup::~RadioButtonGroup() {
}

RadioButton* RadioButtonGroup::newRadioButton(s16 x, s16 y, u16 width, u16 height) {
	
	RadioButton* newButton = new RadioButton(x, y, width, height, _font);
	addGadget(newButton);

	// Do we need to resize?
	u16 newWidth = _width;
	u16 newHeight = _height;

	if (newWidth < x + width) {
		newWidth = x + width;
	}

	if (newHeight < y + height) {
		newHeight = y + height;
	}

	resize(newWidth, newHeight);

	return newButton;
}

RadioButton* RadioButtonGroup::getSelectedGadget() {
	return (RadioButton*)_selectedGadget;
}

s16 RadioButtonGroup::getSelectedIndex() {
	for (u8 i = 0; i < _gadgets.size(); i++) {
		if (((RadioButton*)_gadgets[i]) == _selectedGadget) {
			return i;
		}
	}

	// Nothing selected
	return -1;
}

void RadioButtonGroup::setSelectedGadget(RadioButton* gadget) {
	if (_selectedGadget != gadget) {
		if (_selectedGadget != NULL) {
			_selectedGadget->setState(RadioButton::RADIO_BUTTON_STATE_OFF);
		}

		_selectedGadget = gadget;

		if (_selectedGadget != NULL) {
			_selectedGadget->setState(RadioButton::RADIO_BUTTON_STATE_ON);
		}

		raiseValueChangeEvent();
	}
}

void RadioButtonGroup::setSelectedIndex(u8 index) {
	if (index < _gadgets.size()) {
		setSelectedGadget((RadioButton*)_gadgets[index]);

		raiseValueChangeEvent();
	}
}

void RadioButtonGroup::draw(Rect clipRect) {
	clear(clipRect);
}

void RadioButtonGroup::draw() {
	Gadget::draw();
}

bool RadioButtonGroup::resize(u16 width, u16 height) {

	if ((_width != width) || (_height != height)) {
		_width = width;
		_height = height;

		raiseResizeEvent(width, height);

		return true;
	}

	return false;
}
