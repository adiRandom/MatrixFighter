#include "MenuEntry.hpp"
#include <string.h>
#include <Arduino.h>
#include "MemoryFree.h"

MenuEntry::MenuEntry()
  : _id{ -1 }, _selectorPos{
      Point{ -1, -1 },
    },
    _menuId{ -1 } {
}

MenuEntry::MenuEntry(uint8_t id, Point selectorPos, uint8_t menuId)
  : _selectorPos{ selectorPos }, _id{ id }, _menuId{ menuId } {
}

MenuEntry::MenuEntry(MenuEntry const& menuEntry)
  : _selectorPos{ menuEntry._selectorPos }, _id{ menuEntry._id }, _menuId{ menuEntry._menuId } {
}

MenuEntry& MenuEntry::operator=(MenuEntry const& menuEntry) {
  if (&menuEntry == this) {
    return *this;
  }

  _selectorPos = menuEntry._selectorPos;
  _id = menuEntry._id;
  _menuId = menuEntry._menuId;
  return *this;
}

uint8_t MenuEntry::getId() const {
  return _id;
}
Point MenuEntry::getSelectorPos() const {
  return _selectorPos;
}

uint8_t MenuEntry::getMenuId() const {
  return _menuId;
}

bool MenuEntry::operator==(MenuEntry const& other) {
  return _menuId == other._menuId && _id == other._id;
}
