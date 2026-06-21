/**
 * @file ScreenPrintingModel.hpp
 */
#pragma once

#include "IScreenPrinting.hpp"
#include "gui.hpp"
#include "window_header.hpp"
#include "status_footer.hpp"
#include <utility_extensions.hpp>

#if HAS_LARGE_DISPLAY()
class PrintActionCard final : public window_t {
    Color border_color {};
    Color surface_color {};

public:
    using window_t::window_t;
    bool set_colors(Color border, Color surface);

protected:
    virtual void unconditionalDraw() override;
};
#endif

class ScreenPrintingModel : public IScreenPrinting {
protected:
    enum class BtnSocket {
        Left = 0,
        Middle,
        Right,
        _count,
        _last = _count - 1,
    };
    static constexpr size_t socket_count = std::to_underlying(BtnSocket::_count);

    enum class BtnRes {
        Settings = 0,
        Pause,
        Stop,
        Resume,
        Home,
        Reprint,
        Disconnect,
        SetReady,
        _count,
        _last = _count - 1,
    };

    enum class LabelRes {
        Settings = 0,
        Pause,
        Pausing,
        Stop,
        Resume,
        Resuming,
        Reheating,
        Reprint,
        Home,
        Skip,
        Disconnect,
        SetReady,
        _count,
        _last = _count - 1,
    };

#if HAS_LARGE_DISPLAY()
    PrintActionCard button_cards[socket_count];
#endif
    WindowMultiIconButton buttons[socket_count];
    window_text_t labels[socket_count];

    Rect16 GetButtonRect(uint8_t idx);
    Rect16 GetButtonLabelRect(uint8_t idx);
    void SetButtonIconAndLabel(BtnSocket btn, BtnRes ico_res, LabelRes txt_res);
    void SetButtonIcon(BtnSocket btn, BtnRes res);
    void SetButtonLabel(BtnSocket btn, LabelRes res);
    void DisableButton(BtnSocket btn);
    void EnableButton(BtnSocket btn);
    void refreshButtonStyle(bool redraw = false);

public:
    ScreenPrintingModel(const string_view_utf8 &caption);

protected:
    virtual void windowEvent(window_t *sender, GUI_event_t event, void *param) override;
};
