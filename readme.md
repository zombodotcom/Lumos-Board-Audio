LED PIN DATA_PIN   33

MSGEQ7_STROBE_PIN 26 
MSGEQ7_RESET_PIN  25
//MSGEQ7 AUDIO PIN IS PIN 27
adc2_config_channel_atten( ADC2_CHANNEL_7, ADC_ATTEN_0db );

esp_err_t r = adc2_get_raw( ADC2_CHANNEL_7, ADC_WIDTH_12Bit, &read_raw);//AUDIO PIN IS PIN 27

[esp32 pinout](https://1.bp.blogspot.com/-RL8tkZprmjc/WMxXKaIgocI/AAAAAAAAALw/xLM5Qt3jgSALQ_hHRaurJswgGK5G4wVxwCLcB/s1600/myESP32%2BDevKitC%2Bpinout.png)
