#pragma once

enum Mode {
    COUNT,
    WEEK
};

namespace CONTROL_CONSTANTS {
    inline constexpr int ENTER_KEY = '\n';
} // namespace CONTROL_CONSTANTS


const int CHARACTER_LIMIT = 30;
const int MAX_DISPLAY_ROWS = 30;
const int MAX_ROWS = 15;
const int MAX_SETTINGS = 2;