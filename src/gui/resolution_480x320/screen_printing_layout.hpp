/**
 * @file screen_printing_layout.hpp
 */
#pragma once
#include "img_resources.hpp"
#include "window_icon.hpp"

inline constexpr uint16_t btn_padding = 90;
inline constexpr uint16_t btn_spacing = 30;
inline constexpr WindowMultiIconButton::Pngs icon_resources[] = {
    { img::print_action_settings_80x80, img::print_action_settings_80x80_focused, img::print_action_settings_80x80_disabled },
    { img::print_action_pause_80x80, img::print_action_pause_80x80_focused, img::print_action_pause_80x80_disabled },
    { img::print_action_stop_80x80, img::print_action_stop_80x80_focused, img::print_action_stop_80x80_disabled },
    { img::print_action_resume_80x80, img::print_action_resume_80x80_focused, img::print_action_resume_80x80_disabled },
    { img::print_action_home_80x80, img::print_action_home_80x80_focused, img::print_action_home_80x80_disabled },
    { img::print_action_reprint_80x80, img::print_action_reprint_80x80_focused, img::print_action_reprint_80x80_disabled },
    { img::print_action_disconnect_80x80, img::print_action_disconnect_80x80_focused, img::print_action_disconnect_80x80_disabled },
    { img::print_action_set_ready_80x80, img::print_action_set_ready_80x80_focused, img::print_action_set_ready_80x80_disabled },
};
