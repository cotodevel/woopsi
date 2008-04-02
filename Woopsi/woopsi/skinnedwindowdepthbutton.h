#ifndef _SKINNED_WINDOW_DEPTH_BUTTON_H_
#define _SKINNED_WINDOW_DEPTH_BUTTON_H_

#include <nds.h>
#include "decorationglyphbutton.h"
#include "skin.h"

using namespace std;

class SkinnedWindowDepthButton : public DecorationGlyphButton {

public:
	SkinnedWindowDepthButton(s16 x, s16 y, const WindowSkin* skin);

	virtual void draw(Rect clipRect);
	virtual void draw();

protected:
	const WindowSkin* _skin;

	/**
	 * Destructor.
	 */
	virtual inline ~SkinnedWindowDepthButton() { };
};

#endif
