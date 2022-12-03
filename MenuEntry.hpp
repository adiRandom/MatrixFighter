#ifndef MENU_ENTRY_HPP
#define MENU_ENTRY_HPP
#include "Utils.h"

class MenuEntry {
private:

  char* _name = nullptr;
  Point _selectorPos;

  void setName(char* name);
public:

  MenuEntry();
  MenuEntry(char const name[], Point selectorPos);
  MenuEntry(MenuEntry const& menuEntry);
  MenuEntry& operator=(MenuEntry const& menuEntry);
  virtual ~MenuEntry();

  char* getName() const;
  Point getSelectorPos() const;
};

#endif