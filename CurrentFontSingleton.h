#pragma once

#include "LandsOfLore/Font.h"

using LandsOfLore::Font;

class CurrentFontSingleton
{
public:
    static Font& GetInstance()
    {
        static Font font;

        return font;
    }
};
