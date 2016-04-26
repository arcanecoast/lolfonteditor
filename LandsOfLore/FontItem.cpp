#include <cassert>

#include "FontItem.h"

using namespace LandsOfLore;
using namespace std;

LandsOfLore::FontItem::FontItem() :
m_content(0), m_width(0), m_height(0)
{
}

LandsOfLore::FontItem::FontItem(uint8 width, uint8 height, uint8* content /*= 0*/) :
m_width(width), m_height(height), m_unknown(0)
{
    if (m_width > 0 && m_height > 0) {
        m_content = new uint8[m_width * m_height];

        // Decode lands of lore in-game font item

        int lineWidth = width / 2 + width % 2;

        for (int h = 0; h < m_height; h++) {
            for (int w = 0; w < lineWidth; ++w) {
                if (m_width - w * 2 >= 2) {
                    uint8 twoPackedPixels = content[h * lineWidth + w];

                    uint8 firstPixel  = twoPackedPixels & 15;
                    uint8 secondPixel = twoPackedPixels >> 4;

                    assert(firstPixel <= 15 && secondPixel <= 15);

                    m_content[h * m_width + (w * 2)] = firstPixel;
                    m_content[h * m_width + (w * 2 + 1)] = secondPixel;
                } else {
                    uint8 pixel = content[h * lineWidth + w];

                    assert(pixel <= 15);

                    m_content[h * m_width + (w * 2)] = pixel;
                }
            }
        }

    } else {
        m_content = 0;
    }
}

LandsOfLore::FontItem::FontItem(const FontItem& ref) : 
m_width(ref.m_width), m_height(ref.m_height), m_unknown(ref.m_unknown)
{
    if (ref.m_content) {
        m_content = new uint8[m_width * m_height];
        memcpy(m_content, ref.m_content, m_width * m_height);
    } else {
        m_content = 0;
    }
}

LandsOfLore::FontItem::~FontItem()
{
    delete[] m_content;
}

void LandsOfLore::FontItem::GetSize(uint8& width, uint8& height) const
{
    width  = m_width;
    height = m_height;
}

void LandsOfLore::FontItem::SetSize(uint8 width, uint8 height)
{
    if (width != m_width || height != m_height) {
        m_width  = width;
        m_height = height;

        delete[] m_content;

        if (m_width > 0 && m_height > 0) {
            m_content = new uint8[m_width * m_height];
            memset(m_content, 0, m_width * m_height);
        } else {
            m_content = 0;
        }
    }
}

bool LandsOfLore::FontItem::GetPixel(uint8 x, uint8 y, uint8& color) const
{
    if (x < m_width && y < m_height) {
        color = m_content[y * m_width + x];

        return true;
    } else {
        return false;
    }
}

bool LandsOfLore::FontItem::SetPixel(uint8 x, uint8 y, uint8 color) const
{
    if (x < m_width && y < m_height) {
        m_content[y * m_width + x] = color & 15;

        return true;
    } else {
        return false;
    }
}

uint8* LandsOfLore::FontItem::GetContentInLandsOfLoreFormat(uint16& size) const
{
    uint8* result = 0;

    if (m_content) {
        int lineWidth = m_width / 2 + m_width % 2;

        result = new uint8[lineWidth * m_height];
        size   = lineWidth * m_height;

        for (int h = 0; h < m_height; ++h) {
            for (int w = 0; w < m_width; w += 2) {
                if (m_width - w >= 2) {
                    uint8 firstPixel  = m_content[h * m_width + w];
                    uint8 secondPixel = m_content[h * m_width + w + 1];

                    uint8 twoPackedPixels = (secondPixel << 4) + firstPixel;

                    result[lineWidth * h + w / 2] = twoPackedPixels;
                } else { /* width is odd */
                    uint8 pixel = m_content[h * m_width + w];

                    result[lineWidth * h + w / 2] = pixel;
                }
            }
        }
    } else {
        size = 0;
    }

    return result;
}

FontItem& LandsOfLore::FontItem::operator=(const FontItem& ref)
{
    if (&ref == this) {
        return *this;
    }

    m_width   = ref.m_width;
    m_height  = ref.m_height;
    m_unknown = ref.m_unknown;

    delete[] m_content;

    if (ref.m_content) {
        m_content = new uint8[m_width * m_height];
        memcpy(m_content, ref.m_content, m_width * m_height);
    } else {
        m_content = 0;
    }

    return *this;
}

uint8 LandsOfLore::FontItem::GetUnknown() const
{
    return m_unknown;
}

void LandsOfLore::FontItem::SetUnknown(uint8 value)
{
    m_unknown = value;
}
