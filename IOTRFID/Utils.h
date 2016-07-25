
void blinkLED(int timesToBlink, int delayBetweenBlinks)
{

  int i;
  for (i = 0; i < timesToBlink; i++)
  {
    digitalWrite(blinkPin, 0);
    delay(delayBetweenBlinks / 2);
    digitalWrite(blinkPin, 1);
    delay(delayBetweenBlinks / 2);
  }

}


void clearBufferArray()              // function to clear buffer array
{
  for (int i=0; i<64;i++)
    { RFIDbuffer[i]='\0';}                  // clear all index of array with command NULL
}
