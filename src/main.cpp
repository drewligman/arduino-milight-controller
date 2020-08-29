#include <Arduino.h>
#include <RF24.h>
#include <printf.h>

#include "NRF24MiLightRadio.h"
#include "MiLightRadioConfig.h"
#include "V2RFEncoding.h"
#include "MiLightClient.h"

#define CE_PIN 49
#define CSN_PIN 53

RF24 rf24(CE_PIN, CSN_PIN);

NRF24MiLightRadio radio(
	rf24,
	MiLightRadioConfig::ALL_CONFIGS[2],
	RF24ChannelHelpers::allValues(),
	RF24Channel::RF24_LOW
);

MiLightClient milight(radio);

uint8_t packet[V2_PACKET_LEN];
uint8_t v = 0;

void printPacket(uint8_t* packet) {
	for(int i = 0; i < 9; i++) {
		printf("%02X ", packet[i]);
	}
	printf("\n");
}

void setup() {
	Serial.begin(115200);
	printf_begin();
	printf("Setup\n");
	milight.begin();
	milight.setIdentity(0x8B0D);
	milight.setGroup(0);
}

void loop() {
	milight.updateColor(v += 10);
	milight.updateSaturation(100);
	milight.updateBrightness(100);
	milight.updateBrightness(0);
}
