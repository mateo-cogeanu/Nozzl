/**
 * @file WindowMenuItems.cpp
 * @author Radek Vana
 * @date 2020-11-09
 */

#include "WindowMenuItems.hpp"
#include "ScreenHandler.hpp"
#include "img_resources.hpp"
#include "display.hpp"

MI_RETURN::MI_RETURN()
    : IWindowMenuItem(_(label), &img::folder_up_16x16, is_enabled_t::yes, is_hidden_t::no) {
    has_return_behavior_ = true;
}

void MI_RETURN::click(IWindowMenu &window_menu) {
    window_menu.Validate(); /// don't redraw since we leave the menu
    Screens::Access()->Close();
}

MI_EXIT::MI_EXIT()
    : IWindowMenuItem(_(label), &img::folder_up_16x16, is_enabled_t::yes, is_hidden_t::no) {
}

void MI_EXIT::click(IWindowMenu &window_menu) {
    window_menu.Validate(); /// don't redraw since we leave the menu
    Screens::Access()->Close();
}

WI_ICON_SWITCH_OFF_ON_t::WI_ICON_SWITCH_OFF_ON_t(bool value, const string_view_utf8 &label, const img::Resource *id_icon, is_enabled_t enabled, is_hidden_t hidden)
    : IWindowMenuItem(label, 36, id_icon, enabled, hidden)
    , value_(value) //
{
    touch_extension_only_ = true;
}

void WI_ICON_SWITCH_OFF_ON_t::set_value(bool set) {
    if (value_ != set) {
        value_ = set;
        InValidateExtension();
    }
}

invalidate_t WI_ICON_SWITCH_OFF_ON_t::change(int) {
    value_ = !value_;
    return invalidate_t::yes;
}

void WI_ICON_SWITCH_OFF_ON_t::click(IWindowMenu &) {
    value_ = !value_;
    OnChange(!value_);
    InValidateExtension();
}

void WI_ICON_SWITCH_OFF_ON_t::printExtension(Rect16 extension_rect, [[maybe_unused]] Color color_text, Color color_back, ropfn raster_op) const {
    static_cast<void>(raster_op);
    constexpr uint16_t track_width = 36;
    constexpr uint16_t track_height = 18;
    constexpr uint16_t knob_size = 14;
    constexpr uint16_t knob_margin = 2;
    const Rect16 track(
        extension_rect.Left() + (extension_rect.Width() - track_width) / 2,
        extension_rect.Top() + (extension_rect.Height() - track_height) / 2,
        track_width,
        track_height);
    const Color track_color = !IsEnabled() ? Color::from_raw(0x242424)
        : value_                         ? COLOR_ORANGE
                                         : Color::from_raw(0x3A3A3A);
    const Color knob_color = IsEnabled() ? Color::from_raw(0xD8D8D8) : Color::from_raw(0x777777);
    const uint16_t knob_left = value_ ? track.Right() - knob_size - knob_margin : track.Left() + knob_margin;
    const Rect16 knob(knob_left, track.Top() + knob_margin, knob_size, knob_size);

    display::draw_rounded_rect(track, color_back, track_color, track_height / 2, MIC_ALL_CORNERS);
    display::draw_rounded_rect(knob, track_color, knob_color, knob_size / 2, MIC_ALL_CORNERS);
}
