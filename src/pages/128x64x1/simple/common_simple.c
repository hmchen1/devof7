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
#include "gui/gui.h"
#include "config/model.h"
#include "../pages.h"

#include "simple.h"
#include "../../common/simple/_common_simple.c"

#define gui (&gui_objs.u.stdchan)
static u8 _action_cb(u32 button, u8 flags, void *data);

static guiObject_t *getobj_cb(int relrow, int col, void *data)
{
    (void)col;
    (void)data;
    return (guiObject_t *)&gui->value[relrow];
}
static int row_cb(int absrow, int relrow, int y, void *data)
{
    struct page_defs *page_defs = (struct page_defs *)data;
    (void)data;
    u8 w = 59;
    u8 x = 58;

    GUI_CreateLabelBox(&gui->name[relrow], 0, y,
            0, ITEM_HEIGHT, &DEFAULT_FONT, SIMPLEMIX_channelname_cb, NULL, (void *)(long)absrow);
    GUI_CreateTextSelectPlate(&gui->value[relrow], x, y,
            w, ITEM_HEIGHT, &DEFAULT_FONT, page_defs->tgl, page_defs->value, (void *)(long)absrow);
    return 1;
}

void SIMPLE_Init(const struct page_defs *page_defs)
{
    PAGE_SetActionCB(_action_cb);
    PAGE_SetModal(0);
    PAGE_RemoveAllObjects();
    PAGE_ShowHeader(_tr(page_defs->title));
    GUI_CreateScrollable(&gui->scrollable, 0, ITEM_HEIGHT + 1, LCD_WIDTH, LCD_HEIGHT - ITEM_HEIGHT -1,
                     ITEM_SPACE, Model.num_channels, row_cb, getobj_cb, NULL, (void *)page_defs);
    GUI_SetSelected(GUI_ShowScrollableRowOffset(&gui->scrollable, 0));
}

static u8 _action_cb(u32 button, u8 flags, void *data)
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
