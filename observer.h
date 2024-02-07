#ifndef _OBSERVER_H_
#define _OBSERVER_H_
class Cell;

class Observer {
  public:

  // Method that notifies Cell
  virtual void notify(Cell &c) = 0;
  virtual ~Observer() = default;
};
#endif



