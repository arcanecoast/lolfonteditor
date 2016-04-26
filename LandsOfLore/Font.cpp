#include <cstdio>
#include <memory>

#include "Font.h"

using namespace LandsOfLore;
using namespace std;

bool LandsOfLore::Font::LoadFromFile(const char *path)
{
    FILE *inFile = fopen(path, "rb");

    if (!inFile) {
        return false;
    }

    m_items.clear();

    uint16 filesize;
    uint16 shifts[6];

    fread(&filesize, sizeof(uint16), 1, inFile);
    fread(shifts, sizeof(uint16), 6, inFile);
    fread(m_fontConfig, sizeof(uint8), 3, inFile);

    uint16 itemsCount = 0;
    fread(&itemsCount, sizeof(uint8), 1, inFile);
    ++itemsCount;

    uint8 maxHeight, maxWidth;

    fread(&maxHeight, sizeof(uint8), 1, inFile);
    fread(&maxWidth, sizeof(uint8), 1, inFile);

    // Read items' shift to content

    uint16* shiftsToContent(new uint16[itemsCount]);
    fread(shiftsToContent, sizeof(uint16), itemsCount, inFile);

    // Read items' width

    uint8* widths(new uint8[itemsCount]);
    fseek(inFile, shifts[3], SEEK_SET);
    fread(widths, sizeof(uint8), itemsCount, inFile);

    // Read items' height

    uint8* heights(new uint8[itemsCount]);
    uint8* unknowns(new uint8[itemsCount]);
    fseek(inFile, shifts[5], SEEK_SET);

    for (uint16 i = 0; i < itemsCount; i++)
    {
        fread(&unknowns[i], sizeof(uint8), 1, inFile);
        fread(&heights[i], sizeof(uint8), 1, inFile);
    }

    m_items.reserve(itemsCount);

    for (uint16 i = 0; i < itemsCount; i++)
    {
        uint contentSize = heights[i] * (widths[i] / 2 + widths[i] % 2);

        if (contentSize > 0) {
            uint8 *content = new uint8[contentSize];

            fseek(inFile, shiftsToContent[i], SEEK_SET);
            fread(content, sizeof(uint8), contentSize, inFile);

            FontItem newItem(widths[i], heights[i], content);
            newItem.SetUnknown(unknowns[i]);
            delete[] content;

            m_items.push_back(newItem);
        } else {
            FontItem newItem(widths[i], heights[i]);
            newItem.SetUnknown(unknowns[i]);

            m_items.push_back(newItem);
        }
    }

    delete[] unknowns;
    delete[] heights;
    delete[] widths;
    delete[] shiftsToContent;

    fclose(inFile);

    return true;
}

void LandsOfLore::Font::SaveToFile(const char *path)
{
    FILE *outFile = fopen(path, "wb");

    if (!outFile) {
        return;
    }

    uint16 itemsCount = m_items.size();

    uint8 maxHeight = 0, maxWidth = 0;
    uint8** pointersToContents = new uint8*[itemsCount];
    uint16* sizeOfContents = new uint16[itemsCount];

    uint16 contentSize = 0;

    for (uint16 i = 0; i != itemsCount; ++i) {
        FontItem& item = m_items[i];

        uint8 width, height;
        item.GetSize(width, height);

        if (width > maxWidth) {
            maxWidth = width;
        }

        if (height > maxHeight) {
            maxHeight = height;
        }
        
        pointersToContents[i] = item.GetContentInLandsOfLoreFormat(sizeOfContents[i]);
        contentSize += sizeOfContents[i];
    }

    uint16 shifts[6] = {
        1280,
        14,
        20,
        20 + itemsCount * 2,
        (20 + itemsCount * 2) + itemsCount,
        (20 + itemsCount * 2) + itemsCount + contentSize
    };

    uint16 filesize = shifts[5] + itemsCount * 2;

    fwrite(&filesize, sizeof(uint16), 1, outFile);
    fwrite(shifts, sizeof(uint16), 6, outFile);
    fwrite(m_fontConfig, sizeof(uint8), 3, outFile);

    uint8 itemsCountMinusOne = itemsCount - 1;
    fwrite(&itemsCountMinusOne, sizeof(uint8), 1, outFile);

    fwrite(&maxHeight, sizeof(uint8), 1, outFile);
    fwrite(&maxWidth, sizeof(uint8), 1, outFile);

    // Write items' shift to content

    uint32 offsetToContent = shifts[4];
    for (int i = 0; i != itemsCount; ++i) {
        fwrite(&offsetToContent, sizeof(uint16), 1, outFile);

        offsetToContent += sizeOfContents[i];
    }

    // Write items' width

    for (int i = 0; i != itemsCount; ++i) {
        FontItem& item = m_items[i];

        uint8 width, height;
        item.GetSize(width, height);

        fwrite(&width, sizeof(uint8), 1, outFile);
    }

    // Write items' content

    for (uint16 i = 0; i < itemsCount; i++) {
        if (sizeOfContents[i] > 0) {
            uint8 *content = pointersToContents[i];
            fwrite(content, sizeof(uint8), sizeOfContents[i], outFile);
            delete[] content;
        }
    }

    // Write items' height

    for (int i = 0; i != itemsCount; ++i) {
        FontItem& item = m_items[i];

        uint8 width, height, unknown;
        item.GetSize(width, height);
        unknown = item.GetUnknown();

        fwrite(&unknown, sizeof(uint8), 1, outFile);
        fwrite(&height, sizeof(uint8), 1, outFile);
    }

    delete[] sizeOfContents;
    delete[] pointersToContents;

    fclose(outFile);
}

const FontItem& LandsOfLore::Font::GetItem(uint8 i) const
{
    if (i < Size()) {
        return m_items[i];
    }

    throw "Index is out of bounds";
}

FontItem& LandsOfLore::Font::GetItem(uint8 i)
{
    if (i < Size()) {
        return m_items[i];
    }

    throw "Index is out of bounds";
}

uint16 LandsOfLore::Font::Size() const
{
    return m_items.size();
}

uint8 LandsOfLore::Font::GetFontConfigItem(uint8 i) const
{
    if (i >= 3) {
        throw "Out of bounds  of font config";
    }

    return m_fontConfig[i];
}

void LandsOfLore::Font::SetFontConfigItem(uint8 i, uint8 value)
{
    if (i >= 3) {
        throw "Out of bounds  of font config";
    }

    m_fontConfig[i] = value;
}
