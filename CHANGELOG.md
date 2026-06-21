# Changelog

This file records the local MK4S interface customization and its development environment. It intentionally describes the work in more detail than a release-facing changelog so future edits can be reviewed against the original design and safety constraints.

## Unreleased — MK4S dashboard polish

### Summary

The MK4/MK4S large-display home and active-print screens now use a cleaner card-based interface inspired by contemporary printer interfaces: dark neutral surfaces, a Prusa-orange outline only on the selected option, larger transparent-background action icons, and no extra instructional hero copy. Bottom dialog actions now follow the same selected-outline rule instead of filling the selected button orange. The change preserves the existing Prusa navigation model, action order, actions, and rotary/touch interaction behavior.

This is a presentation-only change. No printer control behavior, heating behavior, temperature values, temperature limits, thermal safeguards, G-code commands, action callbacks, or navigation destinations were changed.

### Home-screen visual redesign

#### Added

- Six large dark action tiles arranged in a three-column by two-row grid.
- Neutral charcoal rounded outlines around inactive tiles, with the Prusa-orange outline reserved for the currently selected option.
- New home-only 96 × 96 icon resources with the old gray backplate removed.
- Larger action icons inside each tile, replacing the previous 80 × 80 resources on the MK4/MK4S home screen.
- Single-piece tile background drawing, avoiding nested rounded windows that created inverse inner-corner artifacts and tiny black corner gaps.
- Redraw-safe selected highlighting: the neutral tile surface is drawn first, then only the orange border segments are drawn, so focus movement cannot briefly flood the tile center orange.
- A consistent dark surface behind each icon and label, replacing the looser icon-only presentation.
- More deliberate horizontal margins and tighter vertical rhythm.
- A tighter relationship between each action icon and its label.
- Background-color continuity beneath transparent icon pixels and label windows, preventing black rectangular seams inside the cards.

#### Layout geometry

The large-display dashboard uses the following geometry:

- Display profile: `W480H320`, selected by the MK4/MK4S `XBUDDY` target.
- Grid: three columns and two rows.
- Card size: 142 × 121 pixels.
- Left grid margin: 19 pixels.
- Horizontal card gap: 8 pixels.
- First-row top position: 42 pixels.
- Vertical card gap: 4 pixels.
- Icon size: new 96 × 96-pixel home icon resources.
- First-row icon top position: 47 pixels.
- Horizontal icon gap: 54 pixels between adjacent 96-pixel icons.
- Label width: 132 pixels.
- Label height: 20 pixels.
- First-row label top position: 142 pixels.

The final column positions are symmetrical: cards begin at x=19, x=169, and x=319. The 96-pixel icons are centered within each card, and labels sit in the lower part of the same card surface. The design therefore reads as a coherent tile launcher instead of six independent controls floating in the screen body.

#### Preserved behavior

The following actions and their order are unchanged:

1. Print/file browser
2. Preheat
3. Filament or MMU filament workflow
4. Control
5. Settings
6. Info

The following dynamic behavior is also unchanged:

- Print remains disabled and changes to “No USB” when removable media is unavailable.
- Inserting USB media can focus the Print action as before.
- MMU state continues to select the appropriate filament/MMU icon and callback.
- Disabled and focused icon resources are still selected by the existing `WindowMultiIconButton` implementation.
- Screen state restoration still records and restores the focused action index.
- Touch and rotary-knob activation still use the original button callbacks.
- Header version/model text and footer status content are unchanged.
- The Mini 240×320 layout is unchanged; the card treatment is compiled only for `HAS_LARGE_DISPLAY()` targets.

### Active-print screen polish

#### Added

- Three 132 × 108-pixel action cards for Tune, Pause/Resume, and Stop/Home on the MK4/MK4S active-print screen.
- A 12-pixel native rounded-corner radius matching the home-screen tiles.
- A 3-pixel Prusa-orange outline around only the focused print action.
- Neutral `0x151515` card surfaces that remain unchanged when selected.
- Muted labels on inactive cards, white labels on the focused card, and subdued styling for unavailable actions.
- New print-action-only 80 × 80 PNG resources with the legacy gray square backplates removed.
- Identical normal and focused icon artwork so focus is communicated by the surrounding card rather than by replacing the icon with an orange or white tile.
- Explicit card/icon/label redraw ordering to keep transparent icons intact during focus changes and periodic active-print screen updates.

#### Layout geometry

- Card positions: x=24, x=174, and x=324.
- Card top: y=177.
- Card size: 132 × 108 pixels.
- Horizontal card gap: 18 pixels.
- Icon size: 80 × 80 pixels, horizontally centered in each card.
- Icon top: y=185.
- Labels remain beneath their corresponding icons and are centered within the card width.

#### Preserved behavior

- Tune still opens the existing tune menu.
- Pause, resume, skip, and transitional labels still follow the existing print state.
- Stop, reprint, home, disconnect, and set-ready actions retain their existing callbacks and state rules.
- Existing enabled, disabled, shadowed, and focus navigation behavior is unchanged.
- The active-print progress, filename, timing, status message, end-result view, header, and footer behavior are unchanged.
- The 240 × 320 Mini active-print layout keeps its existing geometry and icon resources.

#### Border containment correction

- Raised the large-display print-action icons by 4 pixels and reduced the icon-to-label gap from 5 pixels to 2 pixels.
- The label windows now end inside the card’s inner surface instead of extending across the lower border.
- The selected orange outline therefore remains continuous around all four sides, including directly beneath Tune, Pause/Resume, and Stop/Home labels.

### Print-preview action cards

- Replaced the legacy white focused Print icon tile and gray Back icon tile with two compact cards using the home-screen design language.
- Both preview cards use the same `0x151515` charcoal surface as the home and active-print cards.
- The selected preview action receives only a 3-pixel Prusa-orange outline; its center remains charcoal.
- Added transparent-background 80 × 80 Print, Back, and tool-mapping icon resources specifically for the preview screen.
- Normal and selected states use identical icon artwork so the surrounding card is the sole selection indicator.
- Preview card geometry is 110 × 108 pixels with a 12-pixel corner radius, centered in a 120-pixel action column.
- Labels are constrained to the card interior and cannot erase the lower outline.
- Existing Print, Continue/tool-mapping, and Back responses and touch/encoder behavior are unchanged.

### Dialog and workflow action-button polish

- All shared bottom radio-button actions, including prompts such as Abort, Print, Cancel, Done, and Purge More, use the same `0x151515` charcoal center as the main-menu cards.
- Selection is now shown with a 3-pixel orange rounded outline and white text.
- Unselected actions retain the same neutral fill and white text, avoiding a visually heavy full-orange selection block.
- Gaps between adjacent actions are repainted with the dialog parent background, preventing orange or black separator artifacts as focus moves.
- Text rendering is clipped to the inner rectangle of selected actions, preventing it from repainting over the top and bottom outline.
- Button response values, ordering, enabled states, touch targets, encoder behavior, and click dispatch are unchanged.

### Menu, switch, file-list, and scrollbar polish

#### Standard menu rows

- Replaced the legacy white focused-row fill with the same `0x151515` charcoal surface used by the home, preview, and active-print cards.
- Standard enabled menu rows now use charcoal card surfaces with softly rounded 10-pixel corners.
- The selected row receives a 3-pixel Prusa-orange outline while retaining its charcoal center and white text.
- Unselected labels use muted `0xD8D8D8`; selected labels use white.
- Disabled rows use a darker `0x101010` surface and retain the existing disabled-text treatment.
- Focused content is inset inside the border so labels, icons, arrows, values, and switches cannot overwrite the orange outline.
- Focused text-roll padding is reduced by exactly the border inset, preserving the original usable text height and preventing short rows from rendering blank.
- Custom menu-item color schemes remain supported and do not receive the standard orange-card override.
- Removed the legacy dark-gray horizontal delimiter drawn between adjacent menu and file-browser slots.
- Rounded cards are now separated only by the existing narrow black layout gap, eliminating the thin line visible beneath selected and unselected rows.

#### Toggle switches

- Replaced bitmap-backed toggle presentation with native rounded pill rendering.
- Enabled/on toggles use a Prusa-orange track and a light charcoal-white knob.
- Enabled/off toggles use a neutral `0x3A3A3A` track.
- Tri-state/middle toggles use a centered knob and `0x555555` track.
- Disabled toggles use subdued `0x242424` and `0x777777` colors.
- Toggle values, callbacks, touch-only extension behavior, and state transitions are unchanged.

#### G-code and directory rows

- File-browser entries inherit the standard charcoal card and selected orange-outline treatment through the shared menu-item renderer.
- Added a transparent 16 × 16 G-code file PNG containing a bold white “G” edged in Prusa orange.
- Added a transparent 16 × 16 folder PNG using a clean Prusa-orange folder contour.
- File-list entry setup selects those packaged image resources by entry type; neither icon is procedurally drawn in C or C++.
- G-code files and folders retain the same icon artwork in focused and unfocused rows, while selection remains communicated by the surrounding card outline.
- G-code filenames remain visible in selected rows, including through the existing focused text-roll path.
- Directory and Return rows retain their existing icons and navigation behavior.
- File sorting, path handling, loading, selection, and print dispatch are unchanged.

#### Scrollbars

- Both the general `ScrollBar` and menu-specific `MenuScrollbar` render their visible thumb in Prusa orange instead of silver.
- Scrollbar dimensions, proportional thumb sizing, offsets, visibility rules, and scrolling behavior are unchanged.
- Emulator verification used temporary duplicate G-code files to force overflow; those temporary files were removed after confirming the orange scrollbar.

### Shared status-footer polish

- Replaced the visually disconnected black footer region with a continuous `0x151515` charcoal status surface.
- Added a restrained one-pixel `0x2C2C2C` divider along the footer’s upper edge.
- Propagated the footer surface color through its line, item, icon, and text windows so transparent icon pixels and text backgrounds do not create black rectangular seams.
- The same shared footer implementation is used on the home and active-print screens, ensuring identical geometry and styling.
- Existing footer slots, icons, values, formatting, update intervals, configuration, and data readers are unchanged.

### Softer button confirmation sound

- Shortened only the `ButtonEcho` UI confirmation from 100 ms to 28 ms.
- Lowered its pitch from 900 Hz to 700 Hz and set its sound-profile volume to `0.18` for a quieter, less piercing tactile tick.
- Applied the same short confirmation in Once, Loud, and Assist modes; Silent mode remains silent.
- Encoder movement, blocked-navigation feedback, prompts, warnings, critical alerts, waiting signals, startup sound, and forced safety sounds are unchanged.

### Safety policy

#### Repository instructions

Added a repository-level `AGENTS.md` safety policy. It prohibits modifications to:

- Temperature and heating behavior.
- Heating controls and control semantics.
- Temperature values, limits, thresholds, and calibration data.
- Thermal safety checks, interlocks, shutdowns, and recovery logic.
- Related tests, configuration, and documentation that could alter permitted behavior.

The policy permits visual-only edits in a file that also contains heating code, but only when those edits leave every heating-related line and behavior untouched. This exception was required because the home-screen source file also owns the existing Preheat action.

#### Safety review performed

The final firmware diff was searched for added or removed lines containing heating-related commands and terms, including the existing Preheat action and its `M1700` invocation. No heating-related line was changed.

The visual implementation adds card/surface windows, home-only icon resources, layout constants, and background, text, and rounding properties. It does not alter the action array or any callback.

### MK4 emulator setup

#### Added launcher

Added `utils/run_mk4_emulator.sh` as the local entry point for UI testing.

Run it from the repository root:

```sh
./utils/run_mk4_emulator.sh
```

The launcher:

- Uses the MK4 release/no-bootloader build at `build/mk4_release_noboot`.
- Builds that firmware automatically when the ELF is missing.
- Uses the repository Python virtual environment.
- Selects the locally installed native Mini404 build when available.
- Creates and reuses persistent emulator state under `build/mk4-emulator`.
- Creates a simulated USB flash drive under `build/mk4-emulator/usbdir`.
- Adds `UI_DEMO.gcode` to the simulated USB drive for print-menu UI testing.
- Creates and attaches the 8 MiB external-flash image required by the MK4 resource system.
- Creates and attaches both 8 KiB EEPROM banks expected by the emulated xBuddy board.
- Seeds EEPROM with development defaults that bypass mandatory first-run calibration workflows.
- Copies the current firmware bundle to simulated USB so the firmware can install matching UI resources into external flash.
- Disables Mini404's unresponsive native Cocoa display frontend on this macOS version.
- Runs Mini404 headlessly and exposes a purpose-built browser control panel on loopback only.
- Streams actual 480×320 framebuffer screenshots into the browser and provides clickable encoder controls.
- Prints all selected paths before launching so stale-build or wrong-binary problems are visible.
- Detects an emulator already using the configured web port and cleanly restarts the owned session instead of launching a conflicting second QEMU process.

The build and state locations can be overridden without editing the script:

```sh
MK4_BUILD_DIR=/path/to/build \
MK4_EMULATOR_STATE_DIR=/path/to/state \
MK4_EMULATOR_WEB_PORT=8042 \
./utils/run_mk4_emulator.sh
```

To expose a G-code file to the emulated printer, copy it into:

```text
build/mk4-emulator/usbdir
```

Restart the emulator if the emulated USB device does not immediately refresh its directory contents.

#### Apple Silicon compatibility

The firmware bootstrap pins Mini404 0.9.10. Its downloaded macOS executable is Intel `x86_64` and links against Intel Homebrew libraries under `/usr/local/opt`. This development machine is Apple Silicon and uses ARM Homebrew under `/opt/homebrew`, so the pinned executable exits before opening its display with a missing `libfdt.1.dylib` error.

To avoid installing a second Intel Homebrew tree or mixing incompatible library architectures, Mini404 0.9.10 was rebuilt natively from its upstream source tag. The resulting ARM64 executable is installed locally at:

```text
.dependencies/mini404-native-0.9.10/qemu-system-buddy
```

The native build uses the same Mini404 release as the repository bootstrap, links against the existing ARM Homebrew dependencies, and reports an ARM64 Mach-O architecture. The `.dependencies` directory remains build-machine state and is not intended to be committed.

The native simulator was configured from tag `v0.9.10` with:

```sh
git submodule update --init capstone dtc meson slirp ui/keycodemapdb
./configure --target-list=buddy-softmmu
gmake -j3
```

Its `qemu-system-buddy` executable and `pc-bios/keymaps` data were then placed in the native dependency directory above. The launcher detects this binary automatically.

On an Intel Mac or a platform where the repository-provided Mini404 executable is usable, the launcher can fall back to `.dependencies/mini404-0.9.10/qemu-system-buddy`. On Apple Silicon it deliberately refuses that fallback and prints a diagnostic rather than presenting a low-level dynamic-loader failure.

#### Emulator controls and persistence

- The simulator exposes the emulated MK4 LCD and controls at `http://127.0.0.1:8042` and opens it in the default browser.
- Press `W` and `S` to rotate the virtual encoder and `Enter` to press it.
- The three on-page buttons provide the same encoder controls without requiring keyboard focus.
- The simulator runs the successful MK4 release ELF directly from `build/mk4_release_noboot`.
- UI state and emulated storage remain in `build/mk4-emulator` between runs.
- The simulated USB directory is writable from Finder or the terminal while developing.
- Close the Mini404 window or press Ctrl-C in the launching terminal to stop the simulator.
- Emulator behavior approximates the printer hardware but is not a substitute for final validation on a physical MK4S.

### Validation

#### Firmware build

A complete MK4 release build without the bootloader completed successfully:

```sh
python3 utils/build.py \
  --preset mk4 \
  --build-type release \
  --bootloader no \
  --no-store-output
```

Build result:

- Configuration: `mk4_release_noboot`
- Firmware version: `6.4.0+11974.LOCAL`
- Resolution: `W480H320`
- Final status: one successful build, zero failures
- Flash usage: 1,805,164 bytes of 2 MiB, approximately 86.08%
- Main RAM usage: 108,732 bytes of 192 KiB, approximately 55.30%
- CCM RAM usage: 61,180 bytes of 64 KiB, approximately 93.35%
- Generated binary: `build/mk4_release_noboot/firmware.bin`
- Generated bundle: `build/mk4_release_noboot/firmware.bbf`

The build emitted an `xcrun` architecture warning from an ancillary macOS tool, but compilation, linking, firmware processing, and artifact generation all completed successfully.

#### Emulator smoke test

The native ARM64 Mini404 executable was validated with:

- Machine type: `prusa-mk4-027c`
- Kernel: the newly built MK4 release firmware ELF
- Simulated USB mass storage enabled
- Two persistent EEPROM banks attached as parallel flash devices
- Persistent 8 MiB resource flash attached as an MTD device
- Persistent state directory: `build/mk4-emulator`
- USB SOF interrupt emulation disabled, matching the repository simulator wrapper
- Instruction timing: `-icount 2`, matching the repository simulator wrapper

An automated script-console screenshot confirmed that the firmware reached and rendered the customized 480×320 home dashboard. The test verifies actual framebuffer content rather than merely checking that a Mini404 process remains alive.

#### Black-screen diagnosis and correction

The first launcher revision incorrectly treated a running Mini404 process as a successful smoke test. In practice its window remained black and macOS displayed a spinning wait cursor. Two independent causes were identified:

1. The repository's basic `utils/simulator` CLI does not attach MK4 EEPROM or external resource-flash images. Without external flash, the firmware has nowhere to install the image and font resources carried in `firmware.bbf`, leaving the framebuffer black.
2. The debug firmware eventually reached resource installation but stopped at a precise-stepping timing assertion under emulation. The release build does not contain this debug-only assertion and is the appropriate target for interactive UI evaluation.

After correcting the firmware and storage configuration, Mini404's Cocoa frontend still presented as an unresponsive application on this macOS version even though headless framebuffer screenshots proved that the firmware was running. A VNC/TigerVNC fallback connected at the protocol level but did not reliably present a usable macOS window. The final launcher therefore runs QEMU headlessly and serves a small local web interface built directly on Mini404's script console. This avoids every native display frontend while preserving live framebuffer output and encoder interaction.

The web panel requests a fresh screenshot approximately every 150 ms. Screenshot and encoder commands share an asynchronous lock because Mini404's script console processes one command at a time. The HTTP server binds only to `127.0.0.1`; it is not exposed to the local network.

Repeated launcher invocations are safe. Port availability is checked with a local bind operation. The active launcher records its process ID in the emulator state directory. A repeated invocation interrupts that owned process, waits for it to release port 8042, and launches a clean emulator/browser pair. If the occupied port is not owned by this launcher, use `MK4_EMULATOR_WEB_PORT` to select another one.

The launcher now uses a dedicated Python wrapper that provisions and attaches every required storage device. A clean release-state boot was observed progressing from a black framebuffer to the orange “Installing” resource-bootstrap screen and then to the fully rendered home dashboard in approximately 30 seconds.

#### Static checks

- `git diff --check` completed without whitespace errors.
- The MK4-target compilation includes both modified home-screen translation units.
- A focused safety scan confirmed no added or removed heating-related lines.

### Files changed

- `AGENTS.md` — repository safety policy for temperature and heating code.
- `src/gui/screen_home.hpp` — adds large-display-only card and inner surface windows.
- `src/gui/screen_home.cpp` — defines tile geometry and applies visual styling.
- `src/gui/res/png/home_*_96x96*.png` — home-only transparent-background 96-pixel icon variants.
- `src/gui/ScreenPrintingModel.hpp` — adds the large-display print-action card widget and styling hook.
- `src/gui/ScreenPrintingModel.cpp` — defines active-print card geometry, rounded drawing, focus styling, and redraw ordering.
- `src/gui/screen_printing.cpp` — hides and restores the new cards with the existing end-result view.
- `src/gui/resolution_480x320/screen_printing_layout.hpp` — selects transparent print-action icon resources for MK4/MK4S.
- `src/gui/res/png/print_action_*_80x80*.png` — transparent normal, focused, and disabled print-action icon variants.
- `src/gui/dialogs/i_radio_button.cpp` — changes shared workflow actions from selected fill to selected outline.
- `src/gui/dialogs/resolution_480x320/radio_button_preview.hpp` — expands the preview action column and adds card geometry support.
- `src/gui/dialogs/resolution_480x320/radio_button_preview.cpp` — draws unified preview cards, focus outlines, transparent icons, and contained labels.
- `src/gui/res/png/preview_action_*_80x80.png` — transparent Print, Back, and mapping icons for the large-display preview.
- `src/guiapi/src/i_window_menu_item.cpp` — applies the shared charcoal-card and orange-outline treatment to standard menus and file rows.
- `src/guiapi/src/i_window_menu.cpp` — removes legacy horizontal separators between menu cards.
- `src/gui/menu_item/menu_item_toggle_switch.cpp` — draws tri-state toggles with the unified native pill style.
- `src/guiapi/src/WindowMenuItems.cpp` — draws legacy boolean menu toggles with the same native pill style.
- `src/guiapi/src/scroll_bar.cpp` — changes the general scrollbar thumb to orange.
- `src/guiapi/src/window_menu_bar.cpp` — changes menu scrollbar thumbs to orange.
- `src/gui/window_file_list.cpp` — assigns dedicated PNG icons to folder and G-code entries.
- `src/gui/res/png/folder_orange_outline_16x16.png` — transparent orange-contour folder icon.
- `src/gui/res/png/gcode_g_orange_outline_16x16.png` — transparent white G-code glyph with orange edging.
- `src/gui/footer/ifooter.hpp` — adds the shared footer background renderer hook.
- `src/gui/footer/ifooter.cpp` — draws the continuous charcoal footer and subtle upper divider.
- `src/gui/footer/footer_line.cpp` — inherits the shared footer surface for line content.
- `src/gui/footer/ifooter_item.cpp` — propagates the shared surface through footer items, icons, and text.
- `src/guiapi/src/display_ex.cpp` — adds a subtle renderer-level text weight pass so existing fonts read slightly thicker without changing metrics or layout.
- `src/gui/screen_splash.cpp` — changes the MK4/MK4S setup splash greeting from Original Prusa branding to UNORIGINAL NOZZL.
- `src/gui/screen_splash.cpp` — adds a procedural Nozzl boot identity: orange rounded N mark, bold UNORIGINAL NOZZL title, and custom firmware subtitle.
- `cmake/ProjectVersion.cmake` — changes local build version branding from `.LOCAL` to `.NOZZL`, including the short version shown in the header.
- `src/gui/screen_home.cpp` — uses the bold normal font for large-display home menu labels for better readability.
- `src/gui/dialogs/i_radio_button.cpp` — makes shared confirmation buttons redraw as charcoal cards with subtle unselected outlines and selected-only orange outlines.
- `src/guiapi/src/window_msgbox.cpp` — darkens standard/error dialog surfaces and keeps orange as an accent instead of filling the whole dialog body.
- `src/common/sound.cpp` — makes only the button-confirmation tone shorter, lower, and quieter.
- `utils/run_mk4_emulator.sh` — one-command MK4 emulator launcher.
- `utils/run_mk4_emulator.py` — provisions MK4 storage and launches Mini404 with the required devices attached.
- `CHANGELOG.md` — detailed implementation, safety, build, and testing record.

### Known limitations and future polish

- The dashboard now uses home-only icon artwork and focus-only orange tile highlighting.
- The redesign covers the home screen, print-preview actions, active-print actions, shared bottom dialog actions, standard menu rows, toggles, file browsing, and scrollbars.
- The locally built ARM64 Mini404 executable is machine-local build output and is not committed. A clean Apple Silicon checkout must either reproduce the documented native build or gain an upstream universal/ARM release.
- The emulator cannot validate physical display color, viewing angle, encoder feel, actual touch calibration, printer timing, or hardware-only safety behavior.
- The layout uses existing translation strings. Extremely wide translated labels should still be checked visually in translation-enabled builds.
- No release package or flashing operation was performed as part of this change.
