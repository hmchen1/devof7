/*
 This project is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 Deviation is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with Deviation.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "common.h"
#include "../pages.h"
#include "gui/gui.h"
#include "config/model.h"
#include "standard.h"
#if HAS_STANDARD_GUI
#include "../../common/standard/_traveladj_page.c"

static struct stdtravel_obj * const gui = &gui_objs.u.stdtravel;

static unsigned _action_cb(u32 button, unsigned flags, void *data);

static u16 current_selected = 0;

static guiObject_t *getobj_cb(int relrow, int col, void *data)
{
    (void)data;
    if(col == 0)
        return (guiObject_t *)&gui->dn[relrow];
    else
        return (guiObject_t *)&gui->up[relrow];
}
static int row_cb(int absrow, int relrow, int y, void *data)
{
    (void)data;
    u8 w = 35;
    u8 x = 50;
    MIXER_GetLimit(absrow, &mp->limit);
    GUI_CreateLabelBox(&gui->chan[relrow], 0, y,
            0, LINE_HEIGHT, &DEFAULT_FONT, STDMIX_channelname_cb, NULL, (void *)(long)absrow);
    GUI_CreateTextSelectPlate(&gui->dn[relrow], x, y,
            w, LINE_HEIGHT, &DEFAULT_FONT, NULL, traveldown_cb, (void *)(long)absrow);
    GUI_CreateTextSelectPlate(&gui->up[relrow], x + w + 3, y,
            w, LINE_HEIGHT, &DEFAULT_FONT, NULL, travelup_cb, (void *)(long)absrow);
    return 2;
}

void PAGE_TravelAdjInit(int page)
{
    (void)page;
    //if (page < 0 && current_selected > 0) // enter this page from childen page , so we need to get its previous mp->current_selected item
    //    page = current_selected;
    PAGE_SetActionCB(_action_cb);
    PAGE_SetModal(0);
    PAGE_RemoveAllObjects();

    PAGE_ShowHeader(("")); // draw a underline only
    u8 w = 35;
    u8 x = 50;
    GUI_CreateLabelBox(&gui->dnlbl, x+2, 0,  w, LINE_HEIGHT, &DEFAULT_FONT, NULL, NULL, _tr("Down"));
    GUI_CreateLabelBox(&gui->uplbl, x + w +5, 0,  w, LINE_HEIGHT, &DEFAULT_FONT, NULL, NULL, _tr("Up"));

    GUI_CreateScrollable(&gui->scrollable, 0, HEADER_HEIGHT, LCD_WIDTH, LCD_HEIGHT - HEADER_HEIGHT,
                         LINE_SPACE, Model.num_channels, row_cb, getobj_cb, NULL, NULL);

    GUI_SetSelected(GUI_ShowScrollableRowOffset(&gui->scrollable, current_selected));
}

static unsigned _action_cb(u32 button, unsigned flags, void *data)
{
    (void)data;
    if ((flags & BUTTON_PRESS) || (flags & BUTTON_LONGPRESS)) {
        if (CHAN_ButtonIsPressed(button, BUT_EXIT)) {
            PAGE_ChangeByID(PAGEID_MENU, PREVIOUS_ITEM);
        }
        else {
            // only one callback can handle a button press, so we don't handle BUT_ENTER here, let it handled by press cb
            return 0;
        }
    }
    return 1;
}
void PAGE_TravelAdjExit()
{
    current_selected = GUI_ScrollableGetObjRowOffset(&gui->scrollable, GUI_GetSelected());
}
#endif //HAS_STANDARD_GUI
