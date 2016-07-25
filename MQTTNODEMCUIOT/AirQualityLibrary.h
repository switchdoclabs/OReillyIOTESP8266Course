int getAirQuality(void)
{

// Values in this routine are for 16 bit ADC

  int16_t sensor_value = ads1115.readADC_SingleEnded(0);
  Serial.print("Sensor_Value=");
  Serial.print(sensor_value);
  Serial.print("--->");
  if (sensor_value > 11200)
  {
    Serial.println("Very High Pollution Detected");
    return 0;
  }
  else if (sensor_value > 6400)
  {
    Serial.println("High Pollution");
    return 1;
  }
   else if (sensor_value > 4800)
  {
    Serial.println("Medium Pollution");
    return 2;
  }
  else if (sensor_value > 3200)
  {
    Serial.println("Low Pollution");
    return 3;
  }
  else
  {
    Serial.println("Fresh Air");
    return 4;

  }

  return -1;
}

String getAirQualityName(int sensor_value)
{

// Values in this routine are for 16 bit ADC


  if (sensor_value > 11200)
  {
    return "Very High Pollution Detected";

  }
  else if (sensor_value > 6400)
  {
    return("High Pollution");

  }
   else if (sensor_value > 4800)
  {
    return("Medium Pollution");

  }
  else if (sensor_value > 3200)
  {
    return("Low Pollution");

  }
  else
  {
    return("Fresh Air");


  }

  return "unknown";
}

int getAQSSensorValue()
{
  return ads1115.readADC_SingleEnded(0);
}

