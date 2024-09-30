#include <Control_Surface.h>

// COM15
// leonardo.build.pid=0x8040
// leonardo.build.usb_product="FannonFoot"
// https://gist.github.com/nebhead/c92da8f1a8b476f7c36c032a0ac2592a

USBMIDI_Interface midi;
// USBDebugMIDI_Interface debugMidi;

// https://github.com/tttapa/Control-Surface/issues/882
FilteredAnalog<10, 8, uint32_t> a2 = A2;
FilteredAnalog<10, 8, uint32_t> a3 = A3;

// Expression Pedals
CCPotentiometer sustain{ A2, { MIDI_CC::Damper_Pedal, Channel_1 } };
CCPotentiometer expression{ A3, { MIDI_CC::Expression_Controller, Channel_1 } };

// Switch Buttons
CCButtonLatched b1{ 3, { MIDI_CC::General_Purpose_Controller_1, Channel_1 } };
CCButtonLatched b2{ 4, { MIDI_CC::General_Purpose_Controller_2, Channel_1 } };

void setup() {
  Serial.begin(9600);
  sustain.map(sustainMapping);
  Control_Surface.begin();
  delay(500);
  Serial.println("Fannon MIDI Foot Controller v1 started.");
}

long previousMillis = 0;
long interval = 3000;

void loop() {
  Control_Surface.loop();
  
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis > interval) {
    Serial.println("FannonFootV1");
    previousMillis = currentMillis;
  }

  // Visualize Controller State (Serial Plotter):
  // Serial.print("0 "); // To freeze the lower limit
  // Serial.print(127); // To freeze the upper limit
  // Serial.print(" ");
  // Serial.print(" ");
  // Serial.print(sustain.getValue());
  // Serial.print(" ");
  // Serial.print(expression.getValue());
  // Serial.println();
}

// Mapping for Sustain Pedal
// https://tttapa.github.io/Control-Surface-doc/Doxygen/d3/d26/CCPotentiometer-Map_8ino-example.html
constexpr analog_t maxRawValue = CCPotentiometer::getMaxRawValue();
constexpr analog_t minimumValueSustain = (maxRawValue / 64) + 6400;
constexpr analog_t maximumValueSustain = maxRawValue - (maxRawValue / 64) - 200;

analog_t sustainMapping(analog_t raw) {
  raw = constrain(raw, minimumValueSustain, maximumValueSustain);
  analog_t originalRaw = raw;
  analog_t result = map(raw, minimumValueSustain, maximumValueSustain, 0, maxRawValue);

  // Serial.print("0 "); // To freeze the lower limit
  // Serial.print(maxRawValue); // To freeze the upper limit
  // Serial.print(" ");
  // Serial.print(result); // To send all three 'data' points to the plotter
  // Serial.print(" ");
  // Serial.println(originalRaw); // To send all three 'data' points to the plotter
  // delay(50); // Comment out!

  return map(raw, minimumValueSustain, maximumValueSustain, 0, maxRawValue);
}
