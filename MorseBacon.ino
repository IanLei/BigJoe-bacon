#include <stdio.h>
// V0.1 Clive beacon
// V1.1 some code functions moved and refactored
// V1.2 replace scanning for character match to direct ' ' = 0 offset index

#define N_MORSE  (sizeof(morseTable)/sizeof(morseTable[0]))
#define SPEED  (20)
#define DOTLEN  (1200/SPEED)
#define DASHLEN  (3 * (1200/SPEED))

struct t_mtab { char character, pattern; };
struct t_mtab morseTable[] = {
  {' ',  B00000001},            {'!',  B00101000}, /*     */    {'"',  B01010010}, /*     */
  {'#',  B00000001},            {'$',  B11001000}, /*     */    {'%',  B00000001},
  {'&',  B00100010}, /* AS  */  {'\'', B00000001}, /* '   */    {'(',  B00101101},
  {')',  B01101101},            {'*',  B00000001},              {'+',  B00101010}, /* AR  */
  {',',  B01110011}, /*     */  {'-',  B01100001},              {'.',  B01101010},
  {'/',  B00101001},            {'0',  B00111111},              {'1',  B00111110},
  {'2',  B00111100},            {'3',  B00111000},              {'4',  B00110000},
  {'5',  B00100000},            {'6',  B00100001},              {'7',  B00100011},
  {'8',  B00100111},            {'9',  B00101111},              {':',  B01000111},
  {';',  B01010101},            {'<',  B01101000}, /* VA  */    {'=',  B00110001}, /* BT  */
  {'>',  B00110101}, /* KA  */  {'?',  B00110100}, /* INT */    {'@',  B00101000}, /* VE  */
  {'A',  B00000110},            {'B',  B00010001},              {'C',  B00010101},
  {'D',  B00001001},            {'E',  B00000010},              {'F',  B00010100},
  {'G',  B00001011},            {'H',  B00010000},              {'I',  B00000100},
  {'J',  B00011110},            {'K',  B00001101},              {'L',  B00010010},
  {'M',  B00000111},            {'N',  B00000101},              {'O',  B00001111},
  {'P',  B00010110},            {'Q',  B00011011},              {'R',  B00001010},
  {'S',  B00001000},            {'T',  B00000011},              {'U',  B00001100},
  {'V',  B00011000},            {'W',  B00001110},              {'X',  B00011001},
  {'Y',  B00011101},            {'Z',  B00010011}
};

int TxPin = 12;
int tonePin =10;

int LEDpin = 13;

int SerialNo = 0;
char MyMsg[200];

int toneOn = 127;
int toneOff = 0;

void setup() {
  pinMode(LEDpin, OUTPUT);
  digitalWrite(LEDpin, LOW);
  
  pinMode(tonePin, OUTPUT);
  analogWrite(tonePin, toneOff);
  
  pinMode(TxPin, OUTPUT);
  digitalWrite(TxPin, LOW);
  
  Serial.begin(9600);
  Serial.println("Morse Beacon v0.1.2");
}

void loop() {
  SerialNo++;
  sprintf(MyMsg, "II BIGJOE II NR%d = YO HO BIG JOE WHAT DO YOU KNOW, FLYING HIGH FLYING LOW, SNIFF THE GAS AND OFF YOU GO = +\n", SerialNo);
  Serial.println(sizeof(MyMsg));
  int i = 0;
  while(MyMsg[i] != 0)    
  {
    FNsend(MyMsg[i]); // character at a time
    i++;
  };
  
  delay(1000);
}

void FNsend(char c) {
  if (c == ' ') { // handle word space character (exception)
    Serial.print(c);
    wordSpc();
    letterSpc();
    return;
  }
  int i = (c - ' ');
      unsigned char Msymbol = morseTable[i].pattern;
      Serial.print(morseTable[i].character); // debug feature

      while (Msymbol != 1) {  // pattern shifted to 1 is the terminating condition
          if (Msymbol & 1)
            dash();
          else
            dot();
          Msymbol >>= 1;  // right shift for next symbol
      }
      letterSpc();
      return;
  /*
    if the character is not a valid selection then print asterixs
    and kick the programmer before returning 
  */
}     //  Serial.print("*");
 

void dot() {
  analogWrite(tonePin, toneOn);
  digitalWrite(TxPin, HIGH);
  delay(DOTLEN);
  analogWrite(tonePin, toneOff);
  digitalWrite(TxPin, LOW);
  delay(DOTLEN);
}

void dash() {
  analogWrite(tonePin, toneOn);
  digitalWrite(TxPin, HIGH);
  delay(DASHLEN);
  analogWrite(tonePin, toneOff);
  digitalWrite(TxPin, LOW);
  delay(DOTLEN);
}

void letterSpc() {
  delay(DOTLEN);
  delay(DOTLEN);
}

void wordSpc() {
  delay(DASHLEN);
  delay(DOTLEN);
}

// YO HO, BIG JOE WHAT DO YOU KNOW, FLYING HIGH, FLYING LOW, SNIFF THE GAS AND OFF YOU GO.
// UP ABOVE THE CLOUDS SO HIGH, BIGJOE SAILS ABOVE THE SKY.
