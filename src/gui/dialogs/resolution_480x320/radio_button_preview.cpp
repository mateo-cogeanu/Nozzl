/**
 * @file radio_button_preview.cpp
 */

#include "radio_button_preview.hpp"
#include "window_icon.hpp"
#include "window_text.hpp"
#include "i18n.h"
#include "img_resources.hpp"
#include "display.hpp"
#include <client_response.hpp>
#include <tools_mapping.hpp>

const char *label_texts[] = {
    N_("Print"),
    N_("Back")
};

static constexpr const img::Resource *icons[] = {
    &img::preview_action_print_80x80,
    &img::preview_action_back_80x80,
};

static constexpr const uint8_t icon_label_delim = 5;
static constexpr const uint8_t label_height = 16;
static constexpr const uint8_t button_cnt = 2;
static constexpr const uint8_t card_border_width = 3;
static constexpr const uint8_t card_corner_radius = 12;
static constexpr const uint16_t card_width = 110;
static constexpr const uint16_t card_height = 108;
static constexpr Color card_surface_color = Color::from_raw(0x151515);

RadioButtonPreview::RadioButtonPreview(window_t *parent, Rect16 rect)
    : RadioButtonFSM(parent, rect, PhasesPrintPreview::main_dialog) {
}

Rect16 RadioButtonPreview::getVerticalIconRect(uint8_t idx) const {
    Rect16 rect = GetRect();
    return Rect16(
        rect.Left() + (rect.Width() - GuiDefaults::ButtonIconSize) / 2,
        rect.Top() + idx * (GuiDefaults::ButtonIconSize + GuiDefaults::ButtonIconVerticalSpacing),
        GuiDefaults::ButtonIconSize,
        GuiDefaults::ButtonIconSize);
}

Rect16 RadioButtonPreview::getVerticalLabelRect(uint8_t idx) const {
    const Rect16 card = getVerticalCardRect(idx);
    return Rect16(
        card.Left() + card_border_width,
        getVerticalIconRect(idx).Bottom() + icon_label_delim,
        card.Width() - 2 * card_border_width,
        label_height);
}

Rect16 RadioButtonPreview::getVerticalCardRect(uint8_t idx) const {
    const Rect16 rect = GetRect();
    return Rect16(
        rect.Left() + (rect.Width() - card_width) / 2,
        getVerticalIconRect(idx).Top() - 4,
        card_width,
        card_height);
}

void RadioButtonPreview::unconditionalDraw() {
    for (int i = 0; i < button_cnt; i++) {
        const bool selected = GetBtnIndex() == i;
        const Rect16 card = getVerticalCardRect(i);
        const Color parent_color = GetParent() ? GetParent()->GetBackColor() : COLOR_BLACK;

        if (selected) {
            display::draw_rounded_rect(card, parent_color, COLOR_ORANGE, card_corner_radius, MIC_ALL_CORNERS);
            const Rect16 inner_card(
                card.Left() + card_border_width,
                card.Top() + card_border_width,
                card.Width() - 2 * card_border_width,
                card.Height() - 2 * card_border_width);
            display::draw_rounded_rect(inner_card, COLOR_ORANGE, card_surface_color,
                card_corner_radius - card_border_width, MIC_ALL_CORNERS);
        } else {
            display::draw_rounded_rect(card, parent_color, card_surface_color, card_corner_radius, MIC_ALL_CORNERS);
        }

        window_icon_t icon(nullptr, getVerticalIconRect(i), icons[i]);
        window_text_t label(nullptr, getVerticalLabelRect(i), is_multiline::no, is_closed_on_click_t::no, _(label_texts[i]));
        icon.SetBackColor(card_surface_color);
        label.SetBackColor(card_surface_color);

        if (ClientResponses::get_available_response(PhasesPrintPreview::main_dialog, i) == Response::Continue) {
            if (tools_mapping::is_tool_mapping_possible()) {
                label.SetText(_("Continue")); // replace print with continue if tools mapping will show
                icon.SetRes(&img::preview_action_mapping_80x80); // replace icon with tools mapping one
            }
        }

        label.set_font(Font::small);
        label.SetAlignment(Align_t::Center());

        icon.Draw();
        label.Draw();
    }
}

void RadioButtonPreview::windowEvent(window_t *sender, GUI_event_t event, void *param) {
    switch (event) {

    case GUI_event_t::CLICK: {
        const Response response = Click();

        marlin_client::FSM_response(fsm_and_phase(), response);

        if (GetParent()) {
            GetParent()->WindowEvent(this, GUI_event_t::CHILD_CLICK, event_conversion_union { .response = response }.pvoid);
        }
    } break;

    case GUI_event_t::TOUCH_CLICK: {
        event_conversion_union un;
        un.pvoid = param;
        std::optional<size_t> new_index = std::nullopt;
        for (size_t i = 0; i < button_cnt; ++i) {
            if (getVerticalIconRect(i).Contain(un.point)) {
                new_index = i;
                break;
            }
        }

        if (new_index) {
            SetBtnIndex(*new_index);
            // Sound_Play(eSOUND_TYPE::ButtonEcho);
            // Generate click for itself
            WindowEvent(this, GUI_event_t::CLICK, param);
        }
    } break;

    default:
        RadioButtonFSM::windowEvent(sender, event, param);
        break;
    }
}
