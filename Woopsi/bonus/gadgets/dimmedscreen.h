#ifndef _DIMMED_SCREEN_H_
#define _DIMMED_SCREEN_H_

#include "screen.h"
#include "graphicsport.h"

using namespace std;

class DimmedScreen : public Screen {
public:
	/**
	 * Constructor.
	 * @param title The title of the screen; not displayed by default.
	 * @param font The font to use with the screen.
	 */
	DimmedScreen(char* title, u32 flags, FontBase* font = NULL) : Screen(title, flags, font) {

	};

	/**
	 * Override the Gadget::draw() method.
	 */
	virtual inline void draw() { Gadget::draw(); };

	/**
	 * Draw the area of this gadget that falls within the clipping region.
	 * Called by the draw() function to draw all visible regions.
	 * @param clipRect The clipping region to draw.
	 * @see draw()
	 */
	virtual void draw(Rect clipRect) {
		
		// Erase the gadget, thus redrawing everything underneath it
		_flags.erased = false;
		disableDrawing();
		woopsiApplication->eraseRect(clipRect);
		enableDrawing();

		GraphicsPort* port = newInternalGraphicsPort(clipRect);

		// Get pixel data directly from the framebuffer
		for (s16 y = clipRect.y; y < clipRect.y + clipRect.height; y++) {
			for (s16 x = clipRect.x; x < clipRect.x + clipRect.width; x++) {
				u16 colour = *(DrawBg[0] + (x + (y * SCREEN_WIDTH)));
				s16 r = colour & 31;
				s16 g = (colour >> 5) & 31;
				s16 b = (colour >> 10) & 31;
			
				if (r > 10) r -= 10; else r = 0;
				if (g > 10) g -= 10; else g = 0;
				if (b > 10) b -= 10; else b = 0;

				port->drawPixel(x, y, woopsiRGB(r, g, b));
			}
		}

		delete port;

		_flags.erased = false;
	};
};

#endif
