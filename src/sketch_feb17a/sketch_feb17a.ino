#include <string.h>
#include <Wire.h>
/*
 * RTC SETUP
 */
#include <RTClib.h>

DS3231 rtc;

void initializeRTC()
{
  Serial.println("Initialize RTC");
#ifdef AVR
  Wire.begin();
#else
  Wire1.begin(); // Shield I2C pins connect to alt I2C bus on Arduino Due
#endif
  if(!rtc.begin())
  {
    Serial.println("RTC not initialized");
  }

  if (!rtc.isrunning()) {
    Serial.print("RTC will adjust to ");
    Serial.println(__TIME__);
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(__DATE__, __TIME__));
  }
}


/*
 * LEDS SETUP
 */

#include <FastLED.h>

#define PIN_MOST_SIGNIFICANT_HOUR_LEDS 9
#define PIN_LESS_SIGNIFICANT_HOUR_LEDS 10
#define PIN_TWO_POINTS_DATA_LEDS 11
#define PIN_MOST_SIGNIFICANT_MINUTE_LEDS 5
#define PIN_LESS_SIGNIFICANT_MINUTE_LEDS 3
#define NUM_PER_DIGIT 8
#define NUM_PER_TWO_POINTS 4
#define LED_TYPE NEOPIXEL
#define RED_MAX_VALUE 255

CRGB mostSignificantHourLeds[NUM_PER_DIGIT];
CRGB lessSignificantHourLeds[NUM_PER_DIGIT];
CRGB twoPointsLeds[NUM_PER_TWO_POINTS];
CRGB mostSignificantMinuteLeds[NUM_PER_DIGIT];
CRGB lessSignificantMinuteLeds[NUM_PER_DIGIT];

void initializeLEDs()
{
  delay(3000); // 3 second delay for recovery
  Serial.println("NEOPIXEL");

  FastLED.addLeds<LED_TYPE, PIN_MOST_SIGNIFICANT_HOUR_LEDS>(mostSignificantHourLeds, NUM_PER_DIGIT);
  FastLED.addLeds<LED_TYPE, PIN_LESS_SIGNIFICANT_HOUR_LEDS>(lessSignificantHourLeds, NUM_PER_DIGIT);
  FastLED.addLeds<LED_TYPE, PIN_TWO_POINTS_DATA_LEDS>(twoPointsLeds, NUM_PER_TWO_POINTS);
  FastLED.addLeds<LED_TYPE, PIN_MOST_SIGNIFICANT_MINUTE_LEDS>(mostSignificantMinuteLeds, NUM_PER_DIGIT);
  FastLED.addLeds<LED_TYPE, PIN_LESS_SIGNIFICANT_MINUTE_LEDS>(lessSignificantMinuteLeds, NUM_PER_DIGIT);

  FastLED.setBrightness(2);
}

void setup() {
  Serial.begin(9600);
  Serial.println("Starting execution");

  initializeRTC();

  initializeLEDs();
}


/*
 * RTC LOOP
 */
#define MOST_SIGNIFICANT_DIGIT 11
#define LESS_SIGNIFICANT_DIGIT 12

int getHourDigit(char* time, int digit)
{
  Serial.println("Get Hour Digit");
  char hourDigitStr[2];
  hourDigitStr[0] = time[digit];
  hourDigitStr[1] = '\0';
  int hourDigit = atoi(hourDigitStr);
  return hourDigit;
}

/*
 * LEDS LOOP
 */

bool isG(int i)
{
  return (i==0);
}
bool isB(int i)
{
  return (i==1);
}
bool isA(int i)
{
  return (i==2);
}
bool isF(int i)
{
  return (i==3);
}
bool isE(int i)
{
  return (i==4) ;
}
bool isD(int i)
{
  return (i==5);
}
bool isC(int i)
{
  return (i==6);
}

int redValueForZero(int i)
{
  // OFF: G
  if(isG(i))
  {
    return 0;
  }
  else // ON: ABCDEF
  {
    return RED_MAX_VALUE;
  }
}

int redValueForOne(int i)
{
  // OFF: ADEFG
  if(isA(i) || isD(i) || isE(i) || isF(i) || isG(i))
  {
    return 0;
  }
  else // ON: BC
  {
    return RED_MAX_VALUE;
  }
}

int redValueForTwo(int i)
{
  // OFF: CF
  if(isC(i) || isF(i))
  {
    return 0;
  }
  else // ON: ABDEG
  {
    return RED_MAX_VALUE;
  }
}

int redValueForThree(int i)
{
  // OFF: EF
  if(isE(i) || isF(i))
  {
    return 0;
  }
  else // ON: ABCDG
  {
    return RED_MAX_VALUE;
  }
}

int redValueForFour(int i)
{
  // OFF: ADE
  if(isA(i) || isD(i) || isE(i))
  {
    return 0;
  }
  else // ON: BCFG
  {
    return RED_MAX_VALUE;
  }
}

int redValueForFive(int i)
{
  // OFF: BE
  if(isB(i) || isE(i))
  {
    return 0;
  }
  else // ON: ACDFG
  {
    return RED_MAX_VALUE;
  }
}

int redValueForSix(int i)
{
  // OFF: B
  if(isB(i))
  {
    return 0;
  }
  else // ON: ACDEFG
  {
    return RED_MAX_VALUE;
  }
}

int redValueForSeven(int i)
{
  // OFF: DEFG
  if(isD(i) || isE(i) || isF(i) || isG(i))
  {
    return 0;
  }
  else // ON: ABC
  {
    return RED_MAX_VALUE;
  }
}

int redValueForEight(int i)
{
  // ON: ALL! ABCDEFG
  return RED_MAX_VALUE;
}

int redValueForNine(int i)
{
  // OFF: E
  if(isE(i))
  {
    return 0;
  }
  else // ON: ABCDFG
  {
    return RED_MAX_VALUE;
  }
}

int redValue(int i, int number)
{
  switch(number)
  {
    case 0:
    {
      return redValueForZero(i);
    }
    break;
    case 1:
    {
      return redValueForOne(i);
    }
    break;
    case 2:
    {
      return redValueForTwo(i);
    }
    break;
    case 3:
    {
      return redValueForThree(i);
    }
    break;
    case 4:
    {
      return redValueForFour(i);
    }
    break;
    case 5:
    {
      return redValueForFive(i);
    }
    break;
    case 6:
    {
      return redValueForSix(i);
    }
    break;
    case 7:
    {
      return redValueForSeven(i);
    }
    break;
    case 8:
    {
      return redValueForEight(i);
    }
    break;
    case 9:
    {
      return redValueForNine(i);
    }
    break;
    default:
      return 0;
  }
}

void sendTimeToLed(int mostSignificant, int lessSignificant)
{
  FastLED.clear();
  // MOST SIGNIFICANT
  for(int i=0; i<NUM_PER_DIGIT; i++)
  {
    int red = redValue(i, mostSignificant);
    //mostSignificantLeds[i] = CRGB(0, 200, 0);
    mostSignificantHourLeds[i] = CRGB(red, 0, 0);
    mostSignificantMinuteLeds[i] = CRGB(red, 0, 0);
    // Serial.print("mostSignificantLeds[");
    // Serial.print(i);
    // Serial.print("] = ");
    // Serial.println(red);
  }



  // LESS SIGNIFICANT
  for(int i=0; i<NUM_PER_DIGIT; i++)
  {
    int red = redValue(i, lessSignificant);
    //lessSignificantLeds[i] = CRGB(0, 200, 0);
    lessSignificantHourLeds[i] = CRGB(red, 0, 0);
    lessSignificantMinuteLeds[i] = CRGB(red, 0, 0);
    // Serial.print("lessSignificantLeds[");
    // Serial.print(i);
    // Serial.print("] = ");
    // Serial.println(red);
  }


  // TWO POINTS
  for(int i=0; i<NUM_PER_TWO_POINTS; i++)
  {
    //int red = lessSignificant%2 == 0 ? 200 : 0;
    //twoPointsLeds[i] = CRGB(0, red, 0);
    twoPointsLeds[i] = CRGB(RED_MAX_VALUE, 0, 0);
    // Serial.print("twoPointsLeds[");
    // Serial.print(i);
    // Serial.print("] = ");
    // Serial.println(red);
  }
  
  FastLED.show(100);
}


void loop() 
{ 

  Serial.println("\n=============RTC===============\n");
  Serial.println(__DATE__);
  Serial.println(__TIME__);
  DateTime now = rtc.now();
  char time[30];
  Serial.println(now.tostr(time));
  
  int mostSignificantDigit = getHourDigit(time, MOST_SIGNIFICANT_DIGIT);
  Serial.println(mostSignificantDigit);
  int lessSignificantDigit = getHourDigit(time, LESS_SIGNIFICANT_DIGIT);
  Serial.println(lessSignificantDigit);

  Serial.println("\n=============LED===============\n");
  sendTimeToLed(mostSignificantDigit,lessSignificantDigit);
  delay(100);
}
