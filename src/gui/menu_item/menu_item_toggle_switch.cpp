#include "menu_item_toggle_switch.hpp"
#include "display.hpp"

#include <utils/enum_array.hpp>

MenuItemToggleSwitch::MenuItemToggleSwitch(Tristate value, const string_view_utf8 &label)
    : IWindowMenuItem(label, 36)
    , value_(value) //
{
    touch_extension_only_ = true;
}

void MenuItemToggleSwitch::set_value(Tristate set, bool emit_toggled) {
    if (value_ == set) {
        return;
    }

    const auto old_value = value_;
    value_ = set;
    if (emit_toggled) {
        toggled(old_value);
    }
    InValidateExtension();
}

void MenuItemToggleSwitch::click(IWindowMenu &) {
    set_value(!value(), true);
}

void MenuItemToggleSwitch::printExtension(Rect16 extension_rect, [[maybe_unused]] Color color_text, Color color_back, ropfn raster_op) const {
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
        : value_.value == Tristate::yes ? COLOR_ORANGE
        : value_.value == Tristate::other ? Color::from_raw(0x555555)
                                         : Color::from_raw(0x3A3A3A);
    const Color knob_color = IsEnabled() ? Color::from_raw(0xD8D8D8) : Color::from_raw(0x777777);
    const uint16_t knob_left = value_.value == Tristate::yes ? track.Right() - knob_size - knob_margin
        : value_.value == Tristate::other                    ? track.Left() + (track_width - knob_size) / 2
                                                            : track.Left() + knob_margin;
    const Rect16 knob(knob_left, track.Top() + knob_margin, knob_size, knob_size);

    display::draw_rounded_rect(track, color_back, track_color, track_height / 2, MIC_ALL_CORNERS);
    display::draw_rounded_rect(knob, track_color, knob_color, knob_size / 2, MIC_ALL_CORNERS);
}
