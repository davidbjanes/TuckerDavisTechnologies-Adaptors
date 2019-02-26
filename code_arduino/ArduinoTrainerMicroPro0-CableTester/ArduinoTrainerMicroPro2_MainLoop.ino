
// **********************************************
// Loop Function
// **********************************************

const uint8_t ON   = 0x05;
const uint8_t OFF  = 0x01;
  
void loop()
{

  turnOffAll_LED();

  int good_channels[18];

  int write_ch;
  for (write_ch = 0; write_ch < 18; write_ch++)
  {
    Serial.print(" WRITE CH: ");   Serial.print(write_ch+1);

    // SET WRITE CHANNEL 1-16;
    //                              1  2  3  4  5  6  7  8   9  10  11  12  13  14  15  16
    const int CH_DF30_MAPPING[] = { 0, 4, 1, 5, 2, 6, 3, 7, 12,  8, 13,  9, 14, 10, 15, 11, 0, 0};
    digitalWrite(pinDigital_DF30a_MUX_S0, bitRead(CH_DF30_MAPPING[write_ch], 0) );
    digitalWrite(pinDigital_DF30a_MUX_S1, bitRead(CH_DF30_MAPPING[write_ch], 1) );
    digitalWrite(pinDigital_DF30a_MUX_S2, bitRead(CH_DF30_MAPPING[write_ch], 2) );
    digitalWrite(pinDigital_DF30a_MUX_S3, bitRead(CH_DF30_MAPPING[write_ch], 3) );

    int sum_correct_array[18];
    int read_ch = write_ch;
    //for (read_ch = 0; read_ch < 18; read_ch++)
    //{
    //  Serial.print("  - READING CH: ");   Serial.print(read_ch);

    // SET READ CHANNEL 1-16;
    digitalWrite(pinDigital_AMP_MUX_S0, bitRead(read_ch, 0) );
    digitalWrite(pinDigital_AMP_MUX_S1, bitRead(read_ch, 1) );
    digitalWrite(pinDigital_AMP_MUX_S2, bitRead(read_ch, 2) );
    digitalWrite(pinDigital_AMP_MUX_S3, bitRead(read_ch, 3) );


    if (read_ch < 16)
      WRITE_MODE_SEL = CH;
    else if (read_ch == 16)
      WRITE_MODE_SEL = REF;
    else if (read_ch == 17)
      WRITE_MODE_SEL = GND;


    switch (WRITE_MODE_SEL)
    {
      case CH:
        digitalWrite(pinDigital_DF30b_MUX_S0, LOW);
        digitalWrite(pinDigital_DF30b_MUX_S1, LOW);
        break;
      case REF:
        digitalWrite(pinDigital_DF30b_MUX_S0, LOW);
        digitalWrite(pinDigital_DF30b_MUX_S1, HIGH);
        break;
      case GND:
        digitalWrite(pinDigital_DF30b_MUX_S0, HIGH);
        digitalWrite(pinDigital_DF30b_MUX_S1, LOW);
        break;
      
    }


    // Write to DAC
    int16_t write_index;
    int16_t prev_val_dac = 0;
    int16_t prev_val_adc = 0;

    int16_t sum_correct = 0;

    for (write_index = 0; write_index < 256; write_index++)
    {
      // Flash LED to indicate progress
      if (write_index % 32 == 0)
        turnOn_LED(write_ch, ON);
      if (write_index % 64 == 0)
        turnOn_LED(write_ch, OFF);
      
      
      int16_t output = pgm_read_word(&(DACLookup_FullSine_8Bit[write_index])) / 2;
      dac.setVoltage(output, false);

      int16_t adc_val;
      switch (WRITE_MODE_SEL)
      {
        case CH:
          adc_val = ads.readADC_SingleEnded(3);
          break;

        case GND:
          adc_val = ads.readADC_SingleEnded(2);
          break;

        case REF:
          adc_val = ads.readADC_SingleEnded(1);
          break;
      }

      int16_t dac_val = output;

      if ( (prev_val_dac - dac_val > 0 && prev_val_adc - adc_val > 0) ||
           (prev_val_dac - dac_val < 0 && prev_val_adc - adc_val < 0) )
      {
        sum_correct += 1;
      }

      prev_val_dac = dac_val;
      prev_val_adc = adc_val; 

    }

    Serial.print(", SUM: "); Serial.println(sum_correct);
    sum_correct_array[read_ch] = sum_correct;
    //}

    
    if (sum_correct_array[write_ch] > 200)
    {
      good_channels[write_ch] = 1;
      turnOn_LED(write_ch, ON);
    }
    else
    {
      good_channels[write_ch] = 0;
      turnOn_LED(write_ch, OFF);
    }
  }

  while (1);

}


