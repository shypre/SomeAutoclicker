#pragma once

#include <vector>

#include "VirtualKeys.h"

#define KEYPRESS_DOWN 0
#define KEYPRESS_UP 1

struct ClickDataRow
{
    //int position;    // should not be needed
    int keycode;
    int wait_time;
    bool key_down_or_up;

    // constructor
    ClickDataRow(int keycode, int wait_time, bool key_down_or_up)
        : keycode(keycode)
        , wait_time(wait_time)
        , key_down_or_up(key_down_or_up)
    {}
};

struct ClickDataStruct
{
    std::vector<ClickDataRow> data_rows;
};