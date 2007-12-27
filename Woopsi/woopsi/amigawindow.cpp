#include "amigawindow.h"	
#include "screen.h"

AmigaWindow::AmigaWindow(s16 x, s16 y, u16 width, u16 height, char* title, u32 flags, FontBase* font) : Window(x, y, width, height, title, flags, font) {
	_title = title;

	_windowBorderTop = NULL;
	_windowBorderLeft = NULL;
	_windowBorderRight = NULL;
	_windowBorderBottom = NULL;
	_windowCloseButton = NULL;

	// Add border to gadget list
	if (!(flags & GADGET_BORDERLESS)) {
		createBorder();
	}
}

AmigaWindow::~AmigaWindow() {
}

u8 AmigaWindow::getBorderSize() {
	if (!_flags.borderless) {
		return WINDOW_BORDER_SIZE;
	}

	return 0;
}

u8 AmigaWindow::getTitleHeight() {
	if (!_flags.borderless) {
		return WINDOW_TITLE_HEIGHT;
	}

	return 0;
}

void AmigaWindow::setClickedGadget(Gadget* gadget) {
	if (_clickedGadget != gadget) {
		_clickedGadget = gadget;

		// Only remember we clicked a gadget if we didn't click
		// a border gadget
		if ((_clickedGadget == _windowBorderBottom) ||
			(_clickedGadget == _windowBorderLeft) ||
			(_clickedGadget == _windowBorderRight) ||
			(_clickedGadget == _windowBorderTop)) {

			// Forget the clicked gadget
			_clickedGadget = NULL;
		}

		// Notify parent
		if (_parent != NULL) {
			_parent->setClickedGadget(this);
		}
	}
}

void AmigaWindow::setBorderless(bool isBorderless) {
	if (isBorderless != _flags.borderless) {
		if (isBorderless) {
			// Remove borders

			if (_flags.closeable) {
				_windowCloseButton->close();
			}

			_windowDepthButton->close();
			_windowBorderLeft->close();
			_windowBorderRight->close();
			_windowBorderBottom->close();
			_windowBorderTop->close();

			_windowCloseButton = NULL;
			_windowDepthButton = NULL;
			_windowBorderBottom = NULL;
			_windowBorderLeft = NULL;
			_windowBorderRight = NULL;
			_windowBorderTop = NULL;

			_flags.borderless = true;

			// Move all children to compensate
			for (u8 i = 0; i < _gadgets.size(); i++) {
				_gadgets[i]->moveTo(_gadgets[i]->getX() - getX() - WINDOW_BORDER_SIZE, _gadgets[i]->getY() - getY() - WINDOW_TITLE_HEIGHT);
			}
		} else {
			// Add borders

			// Move all children to compensate
			for (u8 i = 0; i < _gadgets.size(); i++) {
				_gadgets[i]->moveTo(_gadgets[i]->getX() - getX() + WINDOW_BORDER_SIZE, _gadgets[i]->getY() - getY() + WINDOW_TITLE_HEIGHT);
			}

			// Create borders
			createBorder();

			_flags.borderless = false;
		}

		invalidateVisibleRectCache();

		draw();
	}
}

void AmigaWindow::createBorder() {
	// Add gadgets to the start in reverse order

	// Add close button
	if (_flags.closeable) {
		_windowCloseButton = new WindowCloseButton(0, 0, WINDOW_CLOSE_BUTTON_WIDTH, WINDOW_TITLE_HEIGHT, _font);
		insertGadget(_windowCloseButton);
	}

	// Add depth button
	_windowDepthButton = new WindowDepthButton(_width - WINDOW_DEPTH_BUTTON_WIDTH, 0, WINDOW_DEPTH_BUTTON_WIDTH, WINDOW_TITLE_HEIGHT, _font);
	insertGadget(_windowDepthButton);

	// Add top border
	if (_flags.closeable) {
		_windowBorderTop = new WindowBorderTop(WINDOW_CLOSE_BUTTON_WIDTH, _width - WINDOW_CLOSE_BUTTON_WIDTH - WINDOW_DEPTH_BUTTON_WIDTH, WINDOW_TITLE_HEIGHT, _title, _font);
	} else {
		_windowBorderTop = new WindowBorderTop(0, _width - WINDOW_DEPTH_BUTTON_WIDTH, WINDOW_TITLE_HEIGHT, _title, _font);
	}

	_windowBorderLeft = new WindowBorderSide(0, WINDOW_TITLE_HEIGHT, WINDOW_BORDER_SIZE, _height - WINDOW_BORDER_SIZE - WINDOW_TITLE_HEIGHT);
	_windowBorderRight = new WindowBorderSide(_width - WINDOW_BORDER_SIZE, WINDOW_TITLE_HEIGHT, WINDOW_BORDER_SIZE, _height - WINDOW_BORDER_SIZE - WINDOW_TITLE_HEIGHT);
	_windowBorderBottom = new WindowBorderBottom(0, _height - WINDOW_BORDER_SIZE, _width, WINDOW_BORDER_SIZE, WINDOW_BORDER_SIZE);
	insertGadget(_windowBorderBottom);
	insertGadget(_windowBorderRight);
	insertGadget(_windowBorderLeft);
	insertGadget(_windowBorderTop);
}

bool AmigaWindow::click(s16 x, s16 y) {

	if (_flags.enabled) {
		if (checkCollision(x, y)) {
			bool gotGadget = false;
			_clickedGadget = NULL;

			// Work out which gadget was clicked
			for (s16 i = _gadgets.size() - 1; i > -1; i--) {
				if (_gadgets[i]->click(x, y)) {

					// Only remember we clicked a gadget if we didn't click
					// a border gadget
					if (_clickedGadget != NULL) {
						if ((_clickedGadget != _windowCloseButton) &&
							(_clickedGadget != _windowDepthButton)) {
							gotGadget = true;
						}
					}

					break;
				}
			}

			// Did we click a gadget?
			if (!gotGadget) {
				// Handle click on window
				Gadget::click(x, y);
			}

			// Do we need to draw the XOR rect?
			if (_flags.dragging) {
				// Get a graphics port from the parent screen
				GraphicsPort* port = _parent->newGraphicsPort();

				// Draw rect
				port->drawXORRect(_newX, _newY, _width, _height);

				delete port;
			}

			return true;
		}
	}

	return false;
}

bool AmigaWindow::focus() {

	if (_flags.enabled) {
		if (!_flags.active) {

			// Handle focus gained on window
			Gadget::focus();

			// Run focus on borders
			if (_windowBorderTop != NULL) {
				_windowBorderBottom->draw();
				_windowBorderTop->draw();
				_windowBorderLeft->draw();
				_windowBorderRight->draw();
				_windowDepthButton->draw();
			}

			// Run focus on close button
			if (_windowCloseButton != NULL) {
				_windowCloseButton->draw();
			}

			return true;
		}
	}
	return false;
}

bool AmigaWindow::blur() {

	if (_flags.active) {

		// Handle focus lost on window
		Gadget::blur();

		// Run blur on borders
		if (_windowBorderTop != NULL) {
			_windowBorderBottom->draw();
			_windowBorderTop->draw();
			_windowBorderLeft->draw();
			_windowBorderRight->draw();
			_windowDepthButton->draw();
		}

		// Run blur on close button
		if (_windowCloseButton != NULL) {
			_windowCloseButton->draw();
		}

		// Take focus away from child gadgets
		if (_activeGadget != NULL) {
			_activeGadget->blur();
			_activeGadget = NULL;
		}

		return true;
	}

	return false;
}

bool AmigaWindow::resize(u16 width, u16 height) {

	// Enforce gadget to stay within parent confines if necessary
	if (_parent != NULL) {
		if (!_parent->isPermeable()) {

			Rect parentRect;
			_parent->getClientRect(parentRect);

			// Check width
			if (_x + width > parentRect.width) {
				width = parentRect.width - _x;
			}

			// Check height
			if (_y + height > parentRect.height) {
				height = parentRect.height - _y;
			}
		}
	}

	if ((_width != width) || (_height != height)) {
		erase();

		_width = width;
		_height = height;

		// Top border
		if (_flags.closeable) {
			_windowBorderTop->resize(_width - WINDOW_CLOSE_BUTTON_WIDTH - WINDOW_DEPTH_BUTTON_WIDTH, WINDOW_TITLE_HEIGHT);
		} else {
			_windowBorderTop->resize(_width - WINDOW_DEPTH_BUTTON_WIDTH, WINDOW_TITLE_HEIGHT);
		}

		_windowBorderLeft->resize(WINDOW_BORDER_SIZE, height - WINDOW_BORDER_SIZE - WINDOW_TITLE_HEIGHT);
		_windowBorderRight->moveTo(_width - WINDOW_BORDER_SIZE, WINDOW_TITLE_HEIGHT);
		_windowBorderRight->resize(WINDOW_BORDER_SIZE, height - WINDOW_BORDER_SIZE - WINDOW_TITLE_HEIGHT);
		_windowBorderBottom->resize(_width, WINDOW_BORDER_SIZE);
		_windowBorderBottom->moveTo(0, _height - WINDOW_BORDER_SIZE);

		// Depth button
		_windowDepthButton->moveTo(_width - WINDOW_DEPTH_BUTTON_WIDTH, 0);

		draw();

		raiseResizeEvent(width, height);

		return true;
	}

	return false;
}

// Insert the available space for child gadgets into the rect
void AmigaWindow::getClientRect(Rect& rect) {
	if (!_flags.borderless) {
		rect.x = _windowBorderLeft->getWidth();
		rect.y = _windowBorderTop->getHeight();
		rect.width = _width - _windowBorderLeft->getWidth() - _windowBorderRight->getWidth();
		rect.height = _height - _windowBorderBottom->getHeight() - _windowBorderTop->getHeight();
	} else {
		rect.x = 0;
		rect.y = 0;
		rect.width = _width;
		rect.height = _height;
	}
}
