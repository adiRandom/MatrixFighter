#ifndef MENU_ENTRY_HPP
#define MENU_ENTRY_HPP
#include "Utils.h"

class MenuEntry {
private:

  uint8_t _id;
  Point _selectorPos;
  uint8_t _menuId;
public:

  MenuEntry();
  MenuEntry(uint8_t id, Point selectorPos, uint8_t menuId);
  MenuEntry(MenuEntry const& menuEntry);
  MenuEntry& operator=(MenuEntry const& menuEntry);

  uint8_t getId() const;
  Point getSelectorPos() const;
  uint8_t getMenuId() const;

  bool operator==(MenuEntry const& other);
};

#endif