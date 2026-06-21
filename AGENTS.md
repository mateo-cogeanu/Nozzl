# Agent Safety Instructions

## Temperature and Heating Safety — Do Not Modify

Do not modify temperature or heating behavior, controls, values, limits, safety logic, configuration, constants, tests, calibration data, or documentation.

Purely visual UI and layout edits are permitted in files that also contain temperature or heating code, provided the edits do not alter, move, refactor, rename, reinterpret, or otherwise affect that code or its controls.

This prohibition includes, but is not limited to:

- Hotend, nozzle, heatbed, chamber, enclosure, and ambient temperature handling.
- Minimum, maximum, target, warning, shutdown, runaway, and fault thresholds.
- Heater control, thermal regulation, PID behavior, thermistor or sensor handling, and temperature calibration.
- Safety checks, interlocks, validation, clamping, overrides, error handling, and recovery behavior involving heat or temperature.
- Any change that could directly or indirectly alter permitted temperatures or heating behavior.

If a requested task would require or might affect any of the above, stop and tell the user that the repository instructions prohibit the change. Do not make the temperature- or heating-related portion of the change, even as a refactor or incidental cleanup. When making an allowed visual-only edit in the same file, leave all heating-related lines and behavior untouched and verify this explicitly in the final diff.
