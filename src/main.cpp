#include <Arduino.h>
#include <RF24.h>
#include <RF24PowerLevel.h>
#include <RF24Channel.h>
#include <NRF24MiLightRadio.h>
#include <MiLightRadioConfig.h>
#include <V2RFEncoding.h>
#include <printf.h>

#define cePin 9
#define csnPin 53
#define PACKET_LEN 9

MiLightRadioConfig config = MiLightRadioConfig::ALL_CONFIGS[2];

RF24 rf24(cePin, csnPin);
NRF24MiLightRadio radio(rf24, config, RF24ChannelHelpers::allValues(), RF24Channel::RF24_LOW);

uint8_t send_packet[9];
uint8_t read_frame[9];
size_t read_frame_length;

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
	radio.begin();
	rf24.setPALevel(RF24_PA_MAX);

	send_packet[0] = 0x00; // obfs key
	send_packet[1] = 0x25; // protocolId, FUT089 = 0x25
	send_packet[2] = 0x8B; // deviceId >> 8
	send_packet[3] = 0x0D; // deviceId & 0xFF
	send_packet[4] = 0x01; // cmd?
	send_packet[5] = 0x00; // arg?
	send_packet[6] = 0x00; // seq #
	send_packet[7] = 0x00; // groupId
	Serial.println("---");


}

void loop() {
	bool listening = true;
	if(listening) {
		if(radio.available()) {
			printf("packet on channel %u", rf24.getChannel());
			read_frame_length = PACKET_LEN;
			radio.read(read_frame, read_frame_length);
			printf(" of length %u: \n", read_frame_length);
			//printPacket(read_frame);
			V2RFEncoding::decodeV2Packet(read_frame);
			printPacket(read_frame);
			printf("*%u*", radio.dupesReceived());
		}
	} else {
		V2RFEncoding::encodeV2Packet(send_packet);
		radio.write(send_packet, PACKET_LEN);
		V2RFEncoding::decodeV2Packet(send_packet);
		//send_packet[5]++;
		//send_packet[6]++;
		delay(500);
	}
}
