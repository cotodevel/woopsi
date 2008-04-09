#ifndef _AMIGA_WINDOW_H_
#define _AMIGA_WINDOW_H_

#include <nds.h>
#include "window.h"
#include "eventhandler.h"

using namespace std;

class WindowBorderTop;
class WindowBorderSide;
class WindowBorderBottom;
class WindowBorderButton;

/**
 * Class providing a window that mimics the appearance of the windows from
 * Amiga Workbench 3.x.  It has a title bar and borders around the edge of
 * the window, along with a depth button and (optional) close button.
 */
class AmigaWindow : public Window, public EventHandler {

public:

	/**
	 * Constructor.
	 * @param x The x co-ordinate of the window, relative to its parent.
	 * @param y The y co-ordinate of the window, relative to its parent.
	 * @param width The width of the window.
	 * @param height The height of the window.
	 * @param title The text that should appear in the title bar.
	 * @param flags Standard flags.  Setting GADGET_BORDERLESS hides the
	 * Amiga borders.  Setting GADGET_CLOSEABLE shows the close button.
	 * @param font The font to use for the window.
	 */
	AmigaWindow(s16 x, s16 y, u16 width, u16 height, char* title, u32 flags, FontBase* font = NULL);

	/**
	 * Show or hide the Amiga borders.
	 * @param isBorderless True to enable borders, false to disable them.
	 */
	virtual void setBorderless(bool isBorderless);

	/**
	 * Gets the size of the left, right and bottom borders.
	 * @return The size of the borders.
	 */
	virtual const u8 getBorderSize() const;

	/**
	 * Gets the height of the title bar.
	 * @return The height of the title bar.
	 */
	virtual const u8 getTitleHeight() const;

	/**
	 * Insert the properties of the space within this gadget that is available
	 * for children into the rect passed in as a parameter.
	 * All co-ordinates are relative to this gadget.
	 * @param rect Reference to a rect to populate with data.
	 */
	virtual void getClientRect(Rect& rect) const;

	/**
	 * Sets the supplied gadget as the clicked child.  The gadget must
	 * be a child of this gadget.
	 * @param gadget A pointer to the child gadget.
	 * @see getClickedGadget()
	 */
	virtual void setClickedGadget(Gadget* gadget);

	/**
	 * Click this gadget at the supplied co-ordinates.
	 * @param x X co-ordinate of the click.
	 * @param y Y co-ordinate of the click.
	 * @return True if the click was successful.
	 */
	virtual bool click(s16 x, s16 y);

	/**
	 * Give the gadget focus.
	 * @return True if the gadget received focus correctly.
	 */
	virtual bool focus();

	/**
	 * Remove focus from the gadget.
	 * @return True if the gadget lost focus correctly.
	 */
	virtual bool blur();

	/**
	 * Resize the gadget to the new dimensions.
	 * @param width The new width.
	 * @param height The new height.
	 * @return True if the resize was successful.
	 */
	virtual bool resize(u16 width, u16 height);

	/**
	 * Handle events fired by decoration gadgets.
	 * @param e Event arguments to process.
	 * @return True if the event was processed.
	 */
	virtual bool handleEvent(const EventArgs& e);

protected:
	char* _title;									/**< The window's title */

	WindowBorderTop* _windowBorderTop;				/**< Pointer to the top border */
	WindowBorderSide* _windowBorderLeft;			/**< Pointer to the left border */
	WindowBorderSide* _windowBorderRight;			/**< Pointer to the right border */
	WindowBorderBottom* _windowBorderBottom;		/**< Pointer to the bottom border */
	WindowBorderButton* _closeButton;				/**< Pointer to the close button */
	WindowBorderButton* _depthButton;				/**< Pointer to the depth button */

	/**
	 * Create the window's border.
	 */
	virtual void createBorder();

	/**
	 * Destructor.
	 */
	virtual inline ~AmigaWindow() { }
};

#endif
