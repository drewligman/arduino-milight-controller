#include <Arduino.h>
#include "MiLightRadio.h"

#define V2_PACKET_LEN 9

#define V2_PROTOCOL_ID_INDEX 1
#define V2_COMMAND_INDEX 4
#define V2_ARGUMENT_INDEX 5

#define V2_NUM_GROUPS 8

enum MiLightFUT089Command {
  FUT089_ON_OFF = 0x01,
  FUT089_COLOR = 0x02,
  FUT089_BRIGHTNESS = 0x05,
  FUT089_MODE = 0x06,
  FUT089_KELVIN = 0x07,     // Controls Kelvin when in White mode
  FUT089_SATURATION = 0x07,  // Controls Saturation when in Color mode
  FUT089_NIGHT_MODE = 0x81
};

enum MiLightFUT089Arguments {
  FUT089_MODE_SPEED_UP   = 0x12,
  FUT089_MODE_SPEED_DOWN = 0x13,
  FUT089_WHITE_MODE = 0x14,
  FUT089_OFF_OFFSET = V2_NUM_GROUPS + 1
};

class MiLightClient {
protected:
    MiLightRadio _radio;
    size_t _resendCount;

    uint16_t _deviceId;
    uint8_t _groupId;
    uint8_t _seqN;

public:
    MiLightClient(
        MiLightRadio& radio,
        size_t resendCount = 20,
        uint8_t seqN = 0
    );

    ~MiLightClient() { }

    void setGroup(const uint8_t groupId, const uint16_t deviceId);
    void setResendCount(const size_t resendCount);

    void begin() { _radio.begin(); }
    bool available() { return _radio.available(); }
    size_t read(uint8_t packet[]);
    void write(uint8_t packet[]);
    void sendCommand(const uint8_t command, const uint8_t arg);

    // Common methods
    void updateStatus(const bool status);
    void updateGlobalStatus(const bool status);
    void pair();
    void unpair();

    void updateMode(const uint8_t mode);
    void modeSpeedDown();
    void modeSpeedUp();

    // RGBW methods
    //void updateHue(const uint16_t hue);
    void updateColor(const uint8_t color);
    void updateBrightness(const uint8_t brightness);
    void updateColorWhite();
    void enableNightMode();

    // CCT methods
    void updateTemperature(const uint8_t colorTemperature);
    void updateSaturation(const uint8_t saturation);
};
