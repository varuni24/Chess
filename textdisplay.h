#ifndef __TEXTDISPLAY_H__
#define __TEXTDISPLAY_H__
#include <iostream>
#include <vector>
#include "observer.h"
class Cell;

class TextDisplay: public Observer {
  std::vector<std::vector<char>> theDisplay;

 public:
  TextDisplay();

  void notify(Cell &c) override;

  ~TextDisplay();

  friend std::ostream &operator<<(std::ostream &out, const TextDisplay &td);
};

#endif


