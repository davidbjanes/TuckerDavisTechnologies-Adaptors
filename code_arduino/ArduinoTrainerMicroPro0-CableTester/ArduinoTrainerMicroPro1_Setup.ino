
  // **********************************************
  // Setup Functions 
  // **********************************************
  
  void setup() 
  {        
    // Initialize Random Number Generator
    randomSeed(1);
        
    // Initalize Serial Connection
    Serial.begin(115200);
    
    // Initialize SPI bus
    SPI.begin();
    SPI.setDataMode(SPI_MODE0);
    SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE0));
    SPI.endTransaction();
    
    // Initalize LED Array
    pinMode(pinDigital_LEDDriverSS, OUTPUT); 
    digitalWrite(pinDigital_LEDDriverSS, HIGH);
    setupLEDDriver();
    bootupLEDSequence();
    

    // MUX Setup
    pinMode(pinDigital_AMP_MUX_S0, OUTPUT); digitalWrite(pinDigital_AMP_MUX_S0, LOW);  
    pinMode(pinDigital_AMP_MUX_S1, OUTPUT); digitalWrite(pinDigital_AMP_MUX_S1, LOW);  
    pinMode(pinDigital_AMP_MUX_S2, OUTPUT); digitalWrite(pinDigital_AMP_MUX_S2, LOW);   
    pinMode(pinDigital_AMP_MUX_S3, OUTPUT); digitalWrite(pinDigital_AMP_MUX_S3, LOW);   
    
    pinMode(pinDigital_DF30a_MUX_S0, OUTPUT); digitalWrite(pinDigital_DF30a_MUX_S0, LOW);   
    pinMode(pinDigital_DF30a_MUX_S2, OUTPUT); digitalWrite(pinDigital_DF30a_MUX_S2, LOW);   
    pinMode(pinDigital_DF30a_MUX_S3, OUTPUT); digitalWrite(pinDigital_DF30a_MUX_S3, LOW);   
    pinMode(pinDigital_DF30a_MUX_S1, OUTPUT); digitalWrite(pinDigital_DF30a_MUX_S1, LOW);   
    
    pinMode(pinDigital_DF30b_MUX_S0, OUTPUT); digitalWrite(pinDigital_DF30b_MUX_S0, LOW);   
    pinMode(pinDigital_DF30b_MUX_S1, OUTPUT); digitalWrite(pinDigital_DF30b_MUX_S1, LOW);   
      

    // ADC Setup
    Serial.println("Getting single-ended readings from AIN0..3");
    Serial.println("ADC Range: +/- 6.144V (1 bit = 3mV/ADS1015)");
      
    // The ADC input range (or gain) can be changed via the following
    // functions, but be careful never to exceed VDD +0.3V max, or to
    // exceed the upper and lower limits if you adjust the input range!
    // Setting these values incorrectly may destroy your ADC!
    //                                                                ADS1015  ADS1115
    //                                                                -------  -------
    // ads.setGain(GAIN_TWOTHIRDS);  // 2/3x gain +/- 6.144V  1 bit = 3mV      0.1875mV (default)
    // ads.setGain(GAIN_ONE);        // 1x gain   +/- 4.096V  1 bit = 2mV      0.125mV
    // ads.setGain(GAIN_TWO);        // 2x gain   +/- 2.048V  1 bit = 1mV      0.0625mV
    // ads.setGain(GAIN_FOUR);       // 4x gain   +/- 1.024V  1 bit = 0.5mV    0.03125mV
    // ads.setGain(GAIN_EIGHT);      // 8x gain   +/- 0.512V  1 bit = 0.25mV   0.015625mV
    // ads.setGain(GAIN_SIXTEEN);    // 16x gain  +/- 0.256V  1 bit = 0.125mV  0.0078125mV
    
    ads.setGain(GAIN_TWOTHIRDS);
    ads.begin();
    

    // ADC Setup
    // For Adafruit MCP4725A1 the address is 0x62 (default) or 0x63 (ADDR pin tied to VCC)
    // For MCP4725A0 the address is 0x60 or 0x61
    // For MCP4725A2 the address is 0x64 or 0x65
    dac.begin(0x62);
        
    
    // DELAY for Everything to be Initialized
    delay(1000);    
  }
