/**
 * @file ScreenPrintingModel.cpp
 */
#include "ScreenPrintingModel.hpp"
#include "i18n.h"
#include "ScreenHandler.hpp"
#include "screen_printing_layout.hpp"
#include "display.hpp"

namespace {
constexpr uint16_t btn_size = GuiDefaults::ButtonIconSize;
#if HAS_LARGE_DISPLAY()
constexpr uint16_t card_width = 132;
constexpr uint16_t card_height = 108;
constexpr uint16_t card_left = 24;
constexpr uint16_t card_top = 177;
constexpr uint16_t card_spacing = 18;
constexpr uint16_t card_border_width = 3;
constexpr uint8_t card_corner_radius = 12;
constexpr Color card_surface_color = Color::from_raw(0x151515);
constexpr Color card_disabled_surface_color = Color::from_raw(0x101010);
constexpr Color card_selected_border_color = COLOR_ORANGE;
constexpr Color card_disabled_border_color = Color::from_raw(0x242424);
constexpr Color card_label_color = Color::from_raw(0xD8D8D8);
constexpr Color card_disabled_label_color = Color::from_raw(0x777777);
#endif
#if HAS_LARGE_DISPLAY()
constexpr uint16_t btn_y_offset = 181;
constexpr uint16_t btn_text_spacing = 2;
#else
constexpr uint16_t btn_y_offset = 185;
constexpr uint16_t btn_text_spacing = 5;
#endif
constexpr uint16_t text_height = 17;

constexpr const char *label_resources[] = {
    N_("Tune"),
    N_("Pause"),
    N_("Pausing..."),
    N_("Stop"),
    N_("Resume"),
    N_("Resuming..."),
    N_("Heating..."),
    N_("Reprint"),
    N_("Home"),
    N_("Skip"),
    N_("Disconnect"),
    N_("Set Ready"),
};
} // namespace

#if HAS_LARGE_DISPLAY()
bool PrintActionCard::set_colors(Color border, Color surface) {
    if (border_color == border && surface_color == surface) {
        return false;
    }
    border_color = border;
    surface_color = surface;
    Invalidate();
    return true;
}

void PrintActionCard::unconditionalDraw() {
    const Rect16 rect = GetRect();
    const Color parent_color = GetParent() ? GetParent()->GetBackColor() : surface_color;

    if (border_color == surface_color) {
        display::draw_rounded_rect(rect, parent_color, surface_color, card_corner_radius, MIC_ALL_CORNERS);
        return;
    }

    display::draw_rounded_rect(rect, parent_color, border_color, card_corner_radius, MIC_ALL_CORNERS);
    const Rect16 inner_rect(
        rect.Left() + card_border_width,
        rect.Top() + card_border_width,
        rect.Width() - 2 * card_border_width,
        rect.Height() - 2 * card_border_width);
    display::draw_rounded_rect(inner_rect, border_color, surface_color, card_corner_radius - card_border_width, MIC_ALL_CORNERS);
}
#endif

ScreenPrintingModel::ScreenPrintingModel(const string_view_utf8 &caption)
    : IScreenPrinting(caption)
#if HAS_LARGE_DISPLAY()
    , button_cards {
        { this, Rect16(card_left, card_top, card_width, card_height) },
        { this, Rect16(card_left + card_width + card_spacing, card_top, card_width, card_height) },
        { this, Rect16(card_left + 2 * (card_width + card_spacing), card_top, card_width, card_height) },
    }
#endif
    , buttons {
        { this, GetButtonRect(0), &icon_resources[std::to_underlying(BtnRes::Settings)], [this](window_t &) { TuneAction(); } },
        { this, GetButtonRect(1), &icon_resources[std::to_underlying(BtnRes::Pause)], [this](window_t &) { PauseAction(); } },
        { this, GetButtonRect(2), &icon_resources[std::to_underlying(BtnRes::Stop)], [this](window_t &) { StopAction(); } },
    }
    , labels {
        { this, GetButtonLabelRect(0), is_multiline::no, is_closed_on_click_t::no, _(label_resources[std::to_underlying(LabelRes::Settings)]) },
        { this, GetButtonLabelRect(1), is_multiline::no, is_closed_on_click_t::no, _(label_resources[std::to_underlying(LabelRes::Pause)]) },
        { this, GetButtonLabelRect(2), is_multiline::no, is_closed_on_click_t::no, _(label_resources[std::to_underlying(LabelRes::Stop)]) },
    } {
    for (uint8_t i = 0; i < socket_count; i++) {
        labels[i].set_font(Font::small);
        labels[i].SetPadding({ 0, 0, 0, 0 });
        labels[i].SetAlignment(Align_t::Center());
    }

    refreshButtonStyle();

    static_assert(std::size(label_resources) == std::to_underlying(LabelRes::_count), "Size mismatch");
    static_assert(std::size(icon_resources) == std::to_underlying(BtnRes::_count), "Size mismatch");
}

Rect16 ScreenPrintingModel::GetButtonRect(uint8_t idx) {
#if HAS_LARGE_DISPLAY()
    return Rect16(card_left + (card_width + card_spacing) * idx + (card_width - btn_size) / 2, btn_y_offset, btn_size, btn_size);
#else
    return Rect16(btn_padding + (btn_size + btn_spacing) * idx, btn_y_offset, btn_size, btn_size);
#endif
}

Rect16 ScreenPrintingModel::GetButtonLabelRect(uint8_t idx) {
#if HAS_LARGE_DISPLAY()
    return Rect16(card_left + (card_width + card_spacing) * idx + card_border_width,
        btn_y_offset + btn_size + btn_text_spacing, card_width - 2 * card_border_width, text_height);
#else
    return Rect16(btn_padding - btn_spacing / 2 + (btn_size + btn_spacing) * idx, btn_y_offset + btn_size + btn_text_spacing, btn_size + btn_spacing, text_height);
#endif
}

void ScreenPrintingModel::refreshButtonStyle([[maybe_unused]] bool redraw) {
#if HAS_LARGE_DISPLAY()
    for (size_t i = 0; i < socket_count; ++i) {
        const bool disabled = !buttons[i].IsEnabled() || buttons[i].IsShadowed();
        const bool selected = buttons[i].IsFocused() && !disabled;
        const Color surface = disabled ? card_disabled_surface_color : card_surface_color;
        const Color border = disabled ? card_disabled_border_color : selected ? card_selected_border_color
                                                                         : card_surface_color;

        const bool card_changed = button_cards[i].set_colors(border, surface);
        buttons[i].SetBackColor(surface);
        labels[i].SetBackColor(surface);
        labels[i].SetTextColor(disabled ? card_disabled_label_color : selected ? COLOR_WHITE
                                                                               : card_label_color);

        if (card_changed) {
            buttons[i].Invalidate();
            labels[i].Invalidate();
        }
        if (redraw && card_changed) {
            button_cards[i].Draw();
            buttons[i].Draw();
            labels[i].Draw();
        }
    }
#endif
}

void ScreenPrintingModel::windowEvent(window_t *sender, GUI_event_t event, void *param) {
    IScreenPrinting::windowEvent(sender, event, param);
    refreshButtonStyle(true);
}

void ScreenPrintingModel::SetButtonIconAndLabel(BtnSocket idx, BtnRes ico_res, LabelRes txt_res) {
    SetButtonIcon(idx, ico_res);
    SetButtonLabel(idx, txt_res);
}

void ScreenPrintingModel::SetButtonIcon(BtnSocket idx, BtnRes ico_res) {
    if (idx > BtnSocket::_last || ico_res > BtnRes::_last) {
        return;
    }
    buttons[std::to_underlying(idx)].SetRes(&icon_resources[std::to_underlying(ico_res)]);
}

void ScreenPrintingModel::SetButtonLabel(BtnSocket idx, LabelRes txt_res) {
    if (idx > BtnSocket::_last || txt_res > LabelRes::_last) {
        return;
    }
    labels[std::to_underlying(idx)].SetText(_(label_resources[std::to_underlying(txt_res)]));
}

void ScreenPrintingModel::DisableButton(BtnSocket idx) {
    const size_t btn_idx = std::to_underlying(idx);
    if (idx > BtnSocket::_last || buttons[btn_idx].IsShadowed()) {
        return;
    }

    // Move focus to the right when disabled - if any button is enabled
    if (buttons[btn_idx].IsFocused()) {
        for (size_t i = 1; i < socket_count; i++) {
            if (!buttons[(btn_idx + i) % socket_count].IsShadowed()) {
                buttons[(btn_idx + i) % socket_count].SetFocus();
                break;
            }
        }
    }

    buttons[btn_idx].Shadow();
    buttons[btn_idx].Disable();
    buttons[btn_idx].Invalidate();
    refreshButtonStyle(true);
}

void ScreenPrintingModel::EnableButton(BtnSocket idx) {
    const size_t btn_idx = std::to_underlying(idx);
    if (idx > BtnSocket::_last || !buttons[btn_idx].IsShadowed()) {
        return;
    }

    buttons[btn_idx].Unshadow();
    buttons[btn_idx].Enable();
    buttons[btn_idx].Invalidate();
    refreshButtonStyle(true);
}
