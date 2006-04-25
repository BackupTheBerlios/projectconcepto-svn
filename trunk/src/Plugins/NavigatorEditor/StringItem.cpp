#include "StringItem.h"
#include <stdio.h>
#include <interface/InterfaceDefs.h>


StringItem::StringItem(char *newLabel, char *newString, uint32 level = 0, bool expanded = true):BaseListItem(B_RECT_TYPE,level,expanded)
{
	textControl		= new BTextControl(BRect(0,0,100,10),"StringItem",NULL,newString,NULL);
	label			= newLabel;
	string			= newString;
	background		= ui_color(B_CONTROL_BACKGROUND_COLOR);
	backgroundHi	= ui_color(B_CONTROL_HIGHLIGHT_COLOR);
	foreground		= ui_color(B_CONTROL_TEXT_COLOR);
	separated		= 100;
}



void StringItem::Update(BView *newOwner, const BFont *font)
{
	BListItem::Update(newOwner,font);
	float	widht,height;
	textControl->GetPreferredSize(&widht,&height);
	SetHeight(height+3);
	font_height	fontHeight;
	font->GetHeight(&fontHeight);
	textLine=((height+3)-fontHeight.ascent)/2;
}

void StringItem::DrawItem(BView *owner, BRect bounds, bool complete = false)
{
	BRect	newBounds=bounds;
	newBounds.InsetBy(1,1);
	owner->SetDrawingMode(B_OP_OVER);
	owner->MovePenTo(bounds.left+4, bounds.bottom-2);
	rgb_color color;
	if (IsSelected())
		color = backgroundHi;
	else
		color = background;
	owner->SetHighColor(color);
	textControl->SetViewColor(color);
	owner->FillRoundRect(bounds,3,3);
	if (IsEnabled())
		owner->SetHighColor(foreground);	
	else
		owner->SetHighColor(tint_color(foreground,B_DISABLED_LABEL_TINT));
	owner->MovePenTo(newBounds.left+4, newBounds.bottom-textLine); 
	owner->DrawString(label);
	if (IsSelected())
	{
		if (textControl->Parent() == NULL)
			owner->AddChild(textControl);
	    textControl->MoveTo(newBounds.right-separated+1,newBounds.top+2);
	    textControl->ResizeTo(newBounds.right-separated-3,newBounds.Height()-3);
	}
	else
	{
		if (textControl->Parent() != NULL)
			owner->RemoveChild(textControl);
		owner->MovePenTo(newBounds.right-separated+3, newBounds.bottom-textLine);
		owner->DrawString(string); 
	}
	owner->SetHighColor(205,205,205,255);
	owner->StrokeRoundRect(newBounds,3,3);
	owner->StrokeLine(BPoint(newBounds.right-separated,newBounds.top),BPoint(newBounds.right-separated,newBounds.bottom));
	owner->SetHighColor(foreground);	
}

void StringItem::Select(void)
{
}
void StringItem::Deselect(void)
{
}

void StringItem::SetExpanded(bool expande)
{
	printf("SetExpanded\n");
};
