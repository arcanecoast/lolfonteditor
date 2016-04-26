#pragma once

#include <memory>

#include "../CommonTypes.h"

using std::auto_ptr;

namespace LandsOfLore {
    class FontItem
    {
    private:
        uint8 m_width;
        uint8 m_height;

        uint8 m_unknown;

        uint8* m_content;

    public:
        FontItem();
        FontItem(const FontItem& ref);
        FontItem(uint8 width, uint8 height, uint8* content = 0);
        ~FontItem();

        void GetSize(uint8& width, uint8& height) const;
        void SetSize(uint8 width, uint8 height);

        bool GetPixel(uint8 x, uint8 y, uint8& color) const;
        bool SetPixel(uint8 x, uint8 y, uint8 color) const;

        uint8 GetUnknown() const;
        void SetUnknown(uint8 value);

        FontItem& operator=(const FontItem& ref);

        /************************************************************************/
        /* Converts item's content into Lands of Lore in-game format
        /* 
        /* ATTENTION! Resulting pointer must be freed by client!
        /*
        /* return pointer to content
        /************************************************************************/
        uint8* GetContentInLandsOfLoreFormat(uint16& size) const;
    };
}
