/**
 * @file footer_doubleline.cpp
 * @author Radek Vana
 * @date 2021-04-14
 */

#include "ifooter.hpp"
#include "ScreenHandler.hpp"
#include "footer_eeprom.hpp"
#include "display.hpp"
#include <config_store/store_instance.hpp>

namespace {
constexpr Color footer_surface_color = Color::from_raw(0x151515);
constexpr Color footer_divider_color = Color::from_raw(0x2C2C2C);
}

IFooter::IFooter(window_t *parent)
    : window_frame_t(parent, GuiDefaults::RectFooter, positioning::absolute) {
    SetBackColor(footer_surface_color);
    Disable();
}

void IFooter::unconditionalDraw() {
    const Rect16 rect = GetRect();
    display::fill_rect(rect, GetBackColor());
    display::fill_rect(Rect16(rect.Left(), rect.Top(), rect.Width(), 1), footer_divider_color);
}

bool IFooter::SetSlot(FooterLine &line, size_t slot_id, footer::Item item) {
    return SetSlotInit(slot_id, item) && line.Create(item, slot_id);
}

bool IFooter::SetSlotInit(size_t slot_id, footer::Item item) {
    if (slot_id >= FOOTER_ITEMS_PER_LINE__) {
        return false;
    }
    if (config_store().get_footer_setting(slot_id) != item) {
        config_store().set_footer_setting(slot_id, item);
        // send event to all windows - there can be multiple footers, ScreenEvent is the best way
        Screens::Access()->ScreenEvent(nullptr, GUI_event_t::REINIT_FOOTER, footer::encode_item_for_event(item));
    }
    return true;
}

footer::Item IFooter::GetSlotInit(size_t slot_id) {
    if (slot_id >= FOOTER_ITEMS_PER_LINE__) {
        return footer::Item::none;
    }
    return config_store().get_footer_setting(slot_id);
}
