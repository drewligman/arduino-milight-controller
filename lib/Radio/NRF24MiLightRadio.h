#ifdef ARDUINO
#include "Arduino.h"
#else
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#endif

#include <RF24.h>
#include <vector>

#include "PL1167_nRF24.h"
#include "MiLightRadioConfig.h"
#include "MiLightRadio.h"
#include "RF24Channel.h"

#ifndef _NRF24_MILIGHT_RADIO_H_
#define _NRF24_MILIGHT_RADIO_H_

class NRF24MiLightRadio : public MiLightRadio {
  public:
    NRF24MiLightRadio(
      RF24& rf,
      const MiLightRadioConfig& config,
      const std::vector<RF24Channel>& channels,
      RF24Channel listenChannel
    );

    virtual int begin();
    virtual bool available();
    virtual int read(uint8_t frame[], size_t &frame_length);
    int dupesReceived();
    virtual int write(uint8_t frame[], size_t frame_length);
    virtual int resend();
    virtual int configure();
    const MiLightRadioConfig& config();

  private:
    const std::vector<RF24Channel>& channels;
    const size_t listenChannelIx;

    PL1167_nRF24 _pl1167;
    const MiLightRadioConfig& _config;
    uint32_t _prev_packet_id;

    uint8_t _packet[10];
    uint8_t _out_packet[10];
    bool _waiting;
    int _dupes_received;
};



#endif
