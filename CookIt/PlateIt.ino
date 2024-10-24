/*
  Test Code of Plate It Feature
*/
int resPin0 = A0;
int resPin1 = A1;
int resPin2 = A2;
int resPin3 = A3;
int resPin4 = A4;
int resPin5 = A5;

int Patty = 0;
int Pickles = 0;
int Onions = 0;
int Cheese = 0;
int Tomato = 0;
int Lettuce = 0;
int BottomBun = 0;
int TopBun = 0;

int pattyCount = 0;
int pickleCount = 0;
int onionCount = 0;
int cheeseCount = 0;
int tomatoCount = 0;
int lettuceCount = 0;
int topBunCount = 0;

int order[] = {0, 3, 5, 2, 6};
// Patty, Cheese, Lettuce, Onion, TopBun
int playerOrder[10] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
int ing = 0;
bool winFlag = 1;
int len;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pins: 9 as INPUT and pins 6-8 as outputs.
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  Serial.begin(9600);
  Serial.println("Burger Start");
  len = arraySize(order, sizeof(order));
}

// the loop function runs over and over again forever
void loop()
{
  bool roundStarts = true;
  while (roundStarts)
  {
    // Collect Input ftom pin 9 connected as button and pulldown resistor to ground
    BottomBun = digitalRead(2);
    
    // If Bottom Bun is Placed
    if (BottomBun == HIGH)
    {
      Serial.println("Bottom Bun Down");
      // End Once Bell is Pressed
      while(digitalRead(4) == LOW)
      {
        // Patty
        Patty = analogRead(resPin0);
        if (Patty >=500 && Patty <525 && pattyCount == 0)
        {
          playerOrder[ing] = 0;
          ing++;
          pattyCount++;
          Serial.println("Patty");
          printArray(playerOrder, ing);
        }
        else if(Patty == 1023 && pattyCount != 0)
        {
          ing--;
          pattyCount--;
          Serial.println("Patty Removed");
          playerOrder[ing] = -1;
          printArray(playerOrder, ing);
        }
  
        // Pickles
        Pickles = analogRead(resPin1);
        if (Pickles >=500 && Pickles <525 && pickleCount == 0)
        {
          playerOrder[ing] = 1;
          ing++;
          pickleCount++;
          Serial.println("Pickles");
          printArray(playerOrder, ing);
        }
        else if(Pickles == 1023 && pickleCount != 0)
        {
          ing--;
          pickleCount--;
          Serial.println("Pickles Removed");
          playerOrder[ing] = -1;
          printArray(playerOrder, ing);
        }
  
        // Onions
        Onions = analogRead(resPin2);
        if (Onions >=500 && Onions <525 && onionCount == 0)
        {
          playerOrder[ing] = 2;
          ing++;
          onionCount++;
          Serial.println("Onions");
          printArray(playerOrder, ing);
        }
        else if(Onions == 1023 && onionCount != 0)
        {
          ing--;
          onionCount--;
          Serial.println("Onions Removed");
          playerOrder[ing] = -1;
          printArray(playerOrder, ing);
        }
  
        // Cheese
        Cheese = analogRead(resPin3);
        if (Cheese >=500 && Cheese <525 && cheeseCount == 0)
        {
          playerOrder[ing] = 3;
          ing++;
          cheeseCount++;
          Serial.println("Cheese");
          printArray(playerOrder, ing);
        }
        else if(Cheese == 1023 && cheeseCount != 0)
        {
          ing--;
          cheeseCount--;
          Serial.println("Cheese Removed");
          playerOrder[ing] = -1;
          printArray(playerOrder, ing);
        }
  
        // Tomato
        Tomato = analogRead(resPin4);
        if (Tomato >=0 && Tomato <10 && tomatoCount == 0)
        {
          playerOrder[ing] = 4;
          ing++;
          tomatoCount++;
          Serial.println("Tomato");
          printArray(playerOrder, ing);
        }
         else if(Tomato == 1023 && tomatoCount != 0)
        {
          ing--;
          tomatoCount--;
          Serial.println("Tomato Removed");
          playerOrder[ing] = -1;
          printArray(playerOrder, ing);
        }
  
        // Lettuce
        Lettuce = analogRead(resPin5);
        if (Lettuce >=500 && Lettuce <525 && lettuceCount == 0)
        {
          playerOrder[ing] = 5;
          ing++;
          lettuceCount++;
          Serial.println("Lettuce");
          printArray(playerOrder, ing);
        }
        else if(Lettuce == 1023 && lettuceCount != 0)
        {
          ing--;
          lettuceCount--;
          Serial.println("Lettuce Removed");
          playerOrder[ing] = -1;
          printArray(playerOrder, ing);
        }
  
        // TopBun
        TopBun = digitalRead(3);
        if (TopBun == HIGH && topBunCount == 0)
        {
          playerOrder[ing] = 6;
          ing++;
          topBunCount++;
          Serial.println("TopBun");
          printArray(playerOrder, ing);
        }
        else if(TopBun == LOW && topBunCount != 0)
        {
          ing--;
          topBunCount--;
          Serial.println("TopBun Removed");
          playerOrder[ing] = -1;
          printArray(playerOrder, ing);
        }
        delay(100);
      }
      Serial.println("Bell Rung!");
      delay(1000);
      // Set Endpoint to Length of Array
      printArray(order, len);
      printArray(playerOrder, len);
      delay(500);
      for (int i=0; i<len; i++)
      {
        if (playerOrder[i] == order[i])
          winFlag = 1;
        else
        {
          winFlag = 0;
          break;
        }
      }
  
      if (winFlag == 1)
        Serial.println("Good Job!");
      else
        Serial.println("Get Out Of The Kitchen!");
      BottomBun = LOW;
      delay(1000);
    }
    break;
  }
  // Reset Parameters
  pattyCount = 0;
  pickleCount = 0;
  onionCount = 0;
  cheeseCount = 0;
  tomatoCount = 0;
  lettuceCount = 0;
  topBunCount = 0;
  ing = 0;
  winFlag = 1;
}

void printArray(int arr[], int len)
{
  Serial.print("[");
  for (int i=0; i<len; i++)
  {
    Serial.print(arr[i]);
    if (i!= (len-1))
    Serial.print(",");
  }
  Serial.println("]");
}

int arraySize(int arr[], int arrSize)
{
  int len = arrSize / sizeof(arr[0]);
  Serial.println(len);
  return len;
}
