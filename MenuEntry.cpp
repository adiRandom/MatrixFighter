#include "MenuEntry.hpp"
#include <string.h>
#include <Arduino.h>
#include "MemoryFree.h"

MenuEntry::MenuEntry()
  : _name{ nullptr }, _selectorPos{
      Point{ 0, 0 }
    } {
}

MenuEntry::MenuEntry(char const name[], Point selectorPos)
  : _selectorPos{ selectorPos } {
  setName(name);
}

MenuEntry::MenuEntry(MenuEntry const& menuEntry)
  : _selectorPos{ menuEntry._selectorPos } {
  setName(menuEntry._name);
}

MenuEntry& MenuEntry::operator=(MenuEntry const& menuEntry) {
  if (&menuEntry == this) {
    return *this;
  }

  _selectorPos = menuEntry._selectorPos;
  setName(menuEntry._name);
  return *this;
}

MenuEntry::~MenuEntry() {
  if (_name != nullptr) {
    delete[] _name;
    _name = nullptr;
  }
}

char* MenuEntry::getName() const {
  return _name;
}
Point MenuEntry::getSelectorPos() const {
  return _selectorPos;
}

// void MenuEntry::setName(char const name[]) {
//   if (_name != nullptr) {
//     delete[] _name;
//   }

//   _name = new char[strlen(name) + 1];
//   strcpy(_name, name);
// }

void MenuEntry::setName(char* name) {
  Serial.println("Set");
  Serial.println(name);
  if (_name != nullptr) {
    Serial.println("deleting");

    delete[] _name;
    _name = nullptr;
  }

  Serial.println("Prepare");
  Serial.println(freeMemory());
  _name = new char[strlen(name) + 1];
  Serial.println("Aloc");
  Serial.println(freeMemory());

  strcpy(_name, name);
  Serial.println("Done");
  Serial.println(_name);
}