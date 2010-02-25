#include "scrollingpanel.h"
#include "graphicsport.h"
#include "woopsifuncs.h"

using namespace WoopsiUI;

ScrollingPanel::ScrollingPanel(s16 x, s16 y, u16 width, u16 height, u32 flags, GadgetStyle* style) : Gadget(x, y, width, height, flags, style) {
	
	Rect rect;
	getClientRect(rect);
	
	_canvasWidth = rect.width;
	_canvasHeight = rect.height;
	_canvasX = 0;
	_canvasY = 0;
	
	setAllowsVerticalScroll(true);
	setAllowsHorizontalScroll(true);

	_flags.permeable = true;
}

void ScrollingPanel::drawContents(GraphicsPort* port) {
	port->drawFilledRect(0, 0, _width, _height, getBackColour());
}

void ScrollingPanel::drawBorder(GraphicsPort* port) {

	// Stop drawing if the gadget indicates it should not have an outline
	if (isBorderless()) return;

	port->drawBevelledRect(0, 0, _width, _height, getShadowColour(), getShineColour());
}

void ScrollingPanel::jump(s32 x, s32 y) {
	// Calculate difference between jump value and current value and scroll
	scroll(x - _canvasX, y - _canvasY);
}

void ScrollingPanel::scroll(s32 dx, s32 dy) {

	Rect rect;
	getClientRect(rect);

	// Prevent scrolling outside boundaries
	if (_canvasX + dx < -(_canvasWidth - rect.width)) {
		dx = -(_canvasWidth - rect.width) - _canvasX;
	} else if (_canvasX + dx > 0) {
		dx = -_canvasX;
	}

	if (_canvasY + dy < -(_canvasHeight - rect.height)) {
		dy = -(_canvasHeight - rect.height) - _canvasY;
	} else if (_canvasY + dy > 0) {
		dy = -_canvasY;
	}

	// Prevent scrolling in disallowed planes
	if (!allowsVerticalScroll()) dy = 0;
	if (!allowsHorizontalScroll()) dx = 0;

	// Perform scroll if necessary
	if ((dx != 0) || (dy != 0)) {

		// Perform scroll
		WoopsiArray<Rect> revealedRects;
		GraphicsPort* port = newGraphicsPort(true);
		port->scroll(0, 0, dx, dy, rect.width, rect.height, &revealedRects);
		delete port;

		// Adjust the scroll values
		_canvasY += dy;
		_canvasX += dx;

		if (revealedRects.size() > 0) {

			// Create internal and standard graphics ports
			GraphicsPort* internalPort = newInternalGraphicsPort(revealedRects.at(0));
			GraphicsPort* port = newGraphicsPort(revealedRects.at(0));

			// Draw revealed sections
			for (s32 i = 0; i < revealedRects.size(); ++i) {

				internalPort->setClipRect(revealedRects.at(i));
				port->setClipRect(revealedRects.at(i));

				drawBorder(internalPort);
				drawContents(port);
			}

			delete internalPort;
			delete port;
		}

		// Scroll all child gadgets
		scrollChildren(dx, dy);

		// Notify event handlers
		_gadgetEventHandlers->raiseScrollEvent(dx, dy);
	}
}

// Reposition child gadgets without redrawing
void ScrollingPanel::scrollChildren(s32 dx, s32 dy) {
	s16 gadgetX = 0;
	s16 gadgetY = 0;
	for (s32 i = 0; i < _gadgets.size(); i++) {
		gadgetX = (_gadgets[i]->getX() - getX()) + dx;
		gadgetY = (_gadgets[i]->getY() - getY()) + dy;

		_gadgets[i]->moveTo(gadgetX, gadgetY);
	}
}

void ScrollingPanel::onClick(s16 x, s16 y) {
	startDragging(x, y);
}

void ScrollingPanel::onDrag(s16 x, s16 y, s16 vX, s16 vY) {
	scroll(vX, vY);
}
