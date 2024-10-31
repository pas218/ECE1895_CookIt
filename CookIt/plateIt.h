// plateIt Class

class PlateIt
{
private:

  int resPin0;
  int resPin1;
  int resPin2;
  int resPin3;

  int Patty;
  int Cheese;
  int Tomato;
  int Onions;
  int Lettuce;
  int BottomBun;
  int TopBun;

  int pattyCount;
  int cheeseCount;
  int tomatoCount;
  int onionCount;
  int lettuceCount;
  int bottomBunCount;
  int topBunCount;

  int order[] = {5, 0, 1, 4, 3, 2, 6};
  // BottomBun, Patty, Cheese, Lettuce, Onion, TopBun
  int playerOrder[10] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
  int ing = 0;
  bool winFlag = 1;
  int len;

public:
    PlateIt(int RX, int TX);
    ~PlateIt();
};

PlateIt::PlateIt()
{
  resPin0 = A0;
  resPin1 = A1;
  resPin2 = A2;
  resPin3 = A3;

  Patty = 0;
  Cheese = 0;
  Tomato = 0;
  Onions = 0;
  Lettuce = 0;
  BottomBun = 0;
  TopBun = 0;

  pattyCount = 0;
  cheeseCount = 0;
  tomatoCount = 0;
  onionCount = 0;
  lettuceCount = 0;
  bottomBunCount = 0;
  topBunCount = 0;
}
