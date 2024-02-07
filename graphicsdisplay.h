#ifndef __GRAPHICSDISPLAY_H__
#define __GRAPHICSDISPLAY_H__

#include "observer.h"
#include "window.h"

class Cell;

class GraphicsDisplay: public Observer {
  Xwindow window;
  // const int gridSize;

 public:
  GraphicsDisplay(Xwindow &window);

  void notify(Cell &c) override;

  ~GraphicsDisplay();

};

#endif
