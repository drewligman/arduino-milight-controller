#include "RF24Channel.h"
#include "MiLightRadioConfig.h"

RF24Channel RF24ChannelHelpers::defaultValue() {
  return RF24Channel::RF24_HIGH;
}

std::vector<RF24Channel> RF24ChannelHelpers::allValues() {
  std::vector<RF24Channel> vec;

  for (size_t i = 0; i < MiLightRadioConfig::NUM_CHANNELS; ++i) {
    vec.push_back(static_cast<RF24Channel>(i));
  }

  return vec;
}
