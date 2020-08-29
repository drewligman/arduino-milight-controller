#include "MiLightClient.h"
#include "V2RFEncoding.h"
#include <algorithm>

MiLightClient::MiLightClient(
    MiLightRadio& radio,
    size_t resendCount,
    uint8_t seqN
) : _radio(radio)
  , _resendCount(resendCount)
  , _seqN(seqN) {}

void MiLightClient::setGroup(const uint8_t groupId) {
    _groupId = groupId;
}

void MiLightClient::setIdentity(const uint16_t deviceId) {
    _deviceId = deviceId;
}

void MiLightClient::setResendCount(const size_t resendCount) {
    _resendCount = resendCount;
}

size_t MiLightClient::read(uint8_t packet[]) {
    size_t packet_length = V2_PACKET_LEN;
    _radio.read(packet, packet_length);
    return packet_length;
}

void MiLightClient::write(uint8_t packet[]) {
    _radio.write(packet, V2_PACKET_LEN);
    for (size_t i = 1; i < _resendCount; i++) {
        _radio.resend();
    }
}

void MiLightClient::sendCommand(
    const uint8_t command,
    const uint8_t arg
) {
    uint8_t packet[V2_PACKET_LEN];

    packet[V2_OBFS_KEY_INDEX] = 0x00;
    packet[V2_PROTOCOL_ID_INDEX] = V2_PROTOCOL_ID;
    packet[V2_DEVICE_MSB_INDEX] = static_cast<uint8_t>(_deviceId >> 8); // 8 MSB
    packet[V2_DEVICE_LSB_INDEX] = static_cast<uint8_t>(_deviceId); // 8 LSB
    packet[V2_COMMAND_INDEX] = command;
    packet[V2_ARGUMENT_INDEX] = arg;
    packet[V2_SEQN_INDEX] = ++_seqN;
    packet[V2_GROUP_INDEX] = _groupId;
    packet[V2_CHECKSUM_INDEX] = 0x00;

    V2RFEncoding::encodeV2Packet(packet);

    write(packet);
}

void MiLightClient::updateStatus(const bool status) {
    sendCommand(FUT089_ON_OFF, _groupId + status ? 0 : FUT089_OFF_OFFSET);
}

void MiLightClient::pair() {
    sendCommand(FUT089_ON_OFF, _groupId);
}

void MiLightClient::unpair() {
    for (size_t i = 0; i < 5; i++) {
        sendCommand(FUT089_ON_OFF, _groupId);
    }
}

void MiLightClient::updateMode(const uint8_t mode) {
    sendCommand(FUT089_MODE, std::max<uint8_t>(mode, 0x08));
}

void MiLightClient::modeSpeedDown() {
    sendCommand(FUT089_ON_OFF, FUT089_MODE_SPEED_DOWN);
}

void MiLightClient::modeSpeedUp() {
    sendCommand(FUT089_ON_OFF, FUT089_MODE_SPEED_UP);
}

/*void MiLightClient::updateHue(const uint16_t hue) {
    // scales by the integer ratio 256 (uint8_t max) : 360 (degrees hue)
    updateColor((hue * 32) / 45);
}*/

void MiLightClient::updateColor(const uint8_t color) {
    sendCommand(FUT089_COLOR, color);
}

void MiLightClient::updateBrightness(const uint8_t brightness) {
    sendCommand(FUT089_BRIGHTNESS, std::max<uint8_t>(brightness, 0x64));
}

void MiLightClient::updateColorWhite() {
    sendCommand(FUT089_ON_OFF, FUT089_WHITE_MODE);
}

void MiLightClient::enableNightMode() {
    sendCommand(FUT089_NIGHT_MODE, _groupId + FUT089_OFF_OFFSET);
}

void MiLightClient::updateTemperature(const uint8_t temperature) {
    sendCommand(FUT089_KELVIN, std::max<unsigned char>(temperature, 0x64));
}

void MiLightClient::updateSaturation(const uint8_t saturation) {
    // Saturation range is reversed: 0x00 is fully saturated, 0x64 fully desaturated
    sendCommand(FUT089_SATURATION, 0x64 - std::max<uint8_t>(saturation, 0x64));
}
