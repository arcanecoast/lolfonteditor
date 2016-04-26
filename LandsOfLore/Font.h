#pragma once

#include <vector>

#include "FontItem.h"

using std::vector;

namespace LandsOfLore {
    class Font
    {
    private:
        vector<FontItem> m_items;
        uint8 m_fontConfig[3];

    public:
        bool LoadFromFile(const char *path);
        void SaveToFile(const char *path);

        const FontItem& GetItem(uint8 i) const;
        FontItem& GetItem(uint8 i);

        uint8 GetFontConfigItem(uint8 i) const;
        void SetFontConfigItem(uint8 i, uint8 value);

        uint16 Size() const;
    };
}
