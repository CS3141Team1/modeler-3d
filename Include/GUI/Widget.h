#pragma once

#include <algorithm>
#include <cstdlib>
#include <vector>

#include "GuiRenderer.h"
#include "Types.h"

namespace Gui
{

/**
 * Object holding all of the logic for a widget.
 *
 * @author Michael Conard, Nicholas Hamilton
 */
class Widget
{
public:
    static const float64 LeftAlign;
    static const float64 RightAlign;
    static const float64 TopAlign;
    static const float64 BottomAlign;

	virtual ~Widget() {}

	Widget(float32 x = 0, float32 y = 0, float32 width = 0, float32 height = 0, float64 hAlign = LeftAlign, float64 vAlign = TopAlign)
	: mX(x),
	  mY(y),
	  mWidth(width),
	  mHeight(height),
	  mHAlign(hAlign),
	  mVAlign(vAlign),
	  mParent(nullptr),
	  mChildren() {}

	/**
	 * Called when the mouse is clicked. Checks if a child consumed the click and then if it gets the click.
	 *
	 * @param x an int representing the x position of the click
	 * @param y an int representing the y position of the click
	 * @param button an in representing which button was clicked, 1 = left, 2 = middle, 3 = right
	 * @param down a bool representing if the button was down (why do we have this?)
	 *
	 * @return bool if the click was consumed
	 */
	bool MouseButton(float32 x, float32 y, int32 button, bool down)
	{
	    float32 px, py;
		for(uint32 i = 0; i < mChildren.size(); i++)
		{
		    GetChild(i)->ComputePosition(px, py);
			if(GetChild(i)->MouseButton(x-px,y-py,button,down))
				return true;
		}
		bool bounds = InBounds(x,y);
		if (bounds) OnMouseButton(x, y, button, down);
		return bounds;
	}

	/**
	 * Virtual method for actual widgets to implement, to be called when the widget is clicked.
	 *
	 * @param x an int representing the x position of the click
	 * @param y an int representing the y position of the click
	 * @param button an in representing which button was clicked, 1 = left, 2 = middle, 3 = right
	 * @param down a bool representing if the button was down (why do we have this?)
	 */
	virtual void OnMouseButton(float32 x, float32 y, int32 button, bool down) {}

	/**
	 * Propagates the update event down the children.
	 *
	 * @param dt a double that represents change in time
	 */
	void Update(float64 dt)
	{
	    OnUpdate(dt);

	    for(uint32 i = 0; i < GetChildCount(); i++)
        {
            GetChild(i)->Update(dt);
        }
	}

	/**
	 * Calls the update logic for the widget.
	 */
	virtual void OnUpdate(float64 dt) {}

	/**
	 * Propagates the draw event down the children.
	 *
	 * @param graphics The Video::IgraphicsDevice*
	 * @param g The Video::GuiRenderer
	 */
	void Draw(Video::GuiRenderer* g)
	{
		//Actually draws this widget
		OnDraw(g);

		//Moves the origin because children have position relative to this parent

		float32 x, y;
		for (uint32 i = 0; i < GetChildCount(); i++)
		{
		    GetChild(GetChildCount() - i - 1)->ComputePosition(x, y);

		    g->Translate(x, y);
			//Draws the children
			GetChild(GetChildCount() - i - 1)->Draw(g);
			//Translates the origin back
			g->Translate(-x, -y);
		}
	}

	/**
	 * Contains the draw logic for the widget. To be implemented by real widgets.
	 *
	 * @param graphics The Video::IgraphicsDevice*
	 * @param g The Video::GuiRenderer
	 */
	virtual void OnDraw(Video::GuiRenderer* g) {}

	/**
	 * Focuses this widget to the front of the screen.
	 */
	void Focus() {
		Widget* w = this;

		while (w->GetParent() != nullptr)
		{
			w->GetParent()->AddChild(w, 0);
			w = w->GetParent();
		}
	}

	/**
	 * Adds a child to this widget at the end of it's child list.
	 *
	 * @param w a Widget* that is the new child
	 */
	void AddChild(Widget* w)
	{
		AddChild(w, GetChildCount());
	}

	/**
	 * Adds a child to this widget at a position in it's child list.
	 *
	 * @param w a Widget* that is the new child
	 * @param index a uint32 that is the position to add the child
	 */
	void AddChild(Widget* w, uint32 index) {
		Widget* parent = w->GetParent();

		if (parent != nullptr)
		{
			parent->RemoveChild(w);
		}

		mChildren.insert(mChildren.begin() + index, w);
		w->SetParent(this);
	}

	/**
	 * Searches for and removes a child from this widget.
	 *
	 * @param w the Widget* to search for and remove
	 */
	void RemoveChild(Widget* w)
	{
	    if (w->mParent != this) return;
	    w->mParent = nullptr;
		mChildren.erase(std::remove(mChildren.begin(), mChildren.end(), w), mChildren.end());
	}

	/**
	 * Sets with width and height of the widget.
	 *
	 * @param w a float that is the new width
	 * @param h a float that is the new height
	 */
	void SetSize(float32 w, float32 h)
	{
		mWidth = w;
		mHeight = h;
	}

	void SetAlignment(float32 h, float32 v)
	{
	    mHAlign = h;
	    mVAlign = v;
	}

	/**
	 * Sets with relative x and y position of the widget.
	 *
	 * @param s a float that is the new x position
	 * @param y a float that is the new y position
	 */
	void SetPosition(float32 x, float32 y)
	{
		mX = x;
		mY = y;
	}

	/**
	 * Sets the position and size of the widget
	 *
	 * @param s a float that is the new x position
	 * @param y a float that is the new y position
	 * @param w a float that is the new width
	 * @param h a float that is the new height
	 */
	void SetBounds(float32 x, float32 y, float32 w, float32 h)
	{
		SetPosition(x, y);
		SetSize(w, h);
	}

	/**
	 * Checks if (x,y) is within the bounds of this widget.
	 *
	 * @param x the x position of the coordinate being checked
	 * @param y the y position of the coordinate being checked
	 *
	 * @return bool true if (x,y) is within the widget
	 */
	bool InBounds(float32 x, float32 y)
	{
		x = x - GetX();
		y = y - GetY();

		//std::cout << "X: " << x << ", Y: " << y << std::endl;

		if (x < 0 || x >= GetWidth()) return false;
		if (y < 0 || y >= GetHeight()) return false;

		return true;
	}

	float32 GetHAlign() { return mHAlign; }
	float32 GetVAlign() { return mVAlign; }
	float32 GetX() { return mX; }
	float32 GetY() { return mY; }
	float32 GetWidth() { return mWidth; }
	float32 GetHeight() { return mHeight; }
	Widget* GetParent() { return mParent; }
	Widget* GetChild(uint32 i) { return mChildren[i]; }
	uint32 GetChildCount() { return mChildren.size(); }

	/**
	 * Recursively gets the children count to build total descendant count of this widget.
	 *
	 * @return uint32 the number of descendants from this widget
	 */
	uint32 GetDescendantCount()
	{
		uint32 count = GetChildCount();
		for(uint32 i = 0; i < GetChildCount(); i++)
		{
			count += GetChild(i)->GetDescendantCount();
		}
		return count;
	}

private:
    /**
     * Changes the parent of this widget
     *
     * @param parent the Widget* that is the new parent
     */
    void SetParent(Widget* parent)
    {
        mParent = parent;
    }

	void ComputePosition(float32& x, float32& y)
	{
	    // TODO might be wrong
	    if (!mParent) return;

	    float32 x1 = mParent->mX + mHAlign * (mParent->mWidth - mParent->mX);
	    float32 y1 = mParent->mY + mVAlign * (mParent->mHeight - mParent->mY);

	    int amtX = mHAlign <= 0.5 ? 0 : 1;
	    int amtY = mVAlign <= 0.5 ? 0 : 1;

	    float32 x2 = amtX * (mWidth + mX * 2);
	    float32 y2 = amtY * (mHeight + mY * 2);

	    x = x1 - x2;
	    y = y1 - y2;
	}

    float32 mX, mY, mWidth, mHeight;
    float32 mHAlign, mVAlign;
	Widget* mParent;
	std::vector<Widget*> mChildren;
};


}
