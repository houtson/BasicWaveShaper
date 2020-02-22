/*

Houtson, PMF 02/2020
Testing of differnet waveshapes

pushbuttonA = change to next waveshape on single push
pushbuttonB = pass signal straigh through when pressed down (to help compare flat signal to shaped signal)

*/

#include <Audio.h>
#include <Bounce.h>
#include "waveshapes.h"

AudioSynthWaveform       waveform;
AudioEffectWaveshaper    waveshape_effect;
AudioOutputI2S           i2s2;
AudioConnection          patchCord1(waveform, 0, waveshape_effect, 0);
AudioConnection          patchCord2(waveshape_effect, 0, i2s2, 0);
AudioConnection          patchCord3(waveshape_effect, 0, i2s2, 1);
AudioControlSGTL5000     sgtl5000_1;

const int buttonPinA = 34;
const int buttonPinB = 33;
Bounce pushbuttonA = Bounce(buttonPinA, 20);  // 10 ms debounce
Bounce pushbuttonB = Bounce(buttonPinB, 20);  // 10 ms debounce
int i = 0;
unsigned int waveShapeEffectNumber = 0;            // how many times has it changed to low
bool ledOn= false;
bool changeShape = false;

void setup() {
  //start serial
  Serial.begin(9600);
  // setup buttons
  pinMode(buttonPinA, INPUT_PULLUP);
  pinMode(buttonPinB, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, ledOn);
  // make a basic waveform
  AudioMemory(40);
  waveform.begin(1.0, 200.0, WAVEFORM_SINE);

sgtl5000_1.enable(); // Enable the audio shield
sgtl5000_1.volume(0.3);


}

void loop() {

  if (pushbuttonA.update()) {
    if (pushbuttonA.fallingEdge()) {
      ledOn =!ledOn;
      digitalWrite(LED_BUILTIN, ledOn);
      waveShapeEffectNumber++;
      changeShape = true;
    }
  }
  if (pushbuttonB.update()) {
    if (pushbuttonB.fallingEdge()) {
      waveshape_effect.shape((float *)wave_shape_flat, 2);
    }
    if (pushbuttonB.risingEdge()) {
      Serial.println("Straight through flat....");
      changeShape = true;
    }
  }

  if (changeShape) {
    changeShape = false;
    switch(waveShapeEffectNumber) {
    case WAVE_SHAPE_COMPRESS1:          
      waveshape_effect.shape((float *)wave_shape_compress1, 257);
      Serial.println("Compress 1");
    break;
    case WAVE_SHAPE_CHEBYSHEV_3RD_256:  
      waveshape_effect.shape((float *)wave_shape_chebyshev_3rd, 257);
      Serial.println("WAVE_SHAPE_CHEBYSHEV_3RD_256");
    break;
    case WAVE_SHAPE_CHEBYSHEV_5TH_256:  
      waveshape_effect.shape((float *)wave_shape_chebyshev_5th, 257);
      Serial.println("WAVE_SHAPE_CHEBYSHEV_5TH_256");
    break;
    case WAVE_SHAPE_CHEBYSHEV_6TH_1025:  
      waveshape_effect.shape((float *)wave_shape_chebyshev_6th_1025, 1025);
      Serial.println("WAVE_SHAPE_CHEBYSHEV_6TH_1025");
    break;
    case WAVE_SHAPE_FLAT:               
      waveshape_effect.shape((float *)wave_shape_flat, 2);
      Serial.println("WAVE_SHAPE_FLAT");
    break;
    case WAVE_SHAPE_INVERT:             
      waveshape_effect.shape((float *)wave_shape_invert, 2);
      Serial.println("WAVE_SHAPE_INVERT");
    break;
    case WAVE_SHAPE_UNIPOLAR:           
      waveshape_effect.shape((float *)wave_shape_unipolar, 2);
      Serial.println("WAVE_SHAPE_UNIPOLAR");
    break;
    case WAVE_SHAPE_RECTIFY:            
      waveshape_effect.shape((float *)wave_shape_rectify, 3);
      Serial.println("WAVE_SHAPE_RECTIFY");
    break;
    case WAVE_SHAPE_SOFTCLIP:            
      waveshape_effect.shape((float *)wave_shape_softclip, 257);
      Serial.println("WAVE_SHAPE_SOFTCLIP");
    break;
    // tube sims
    case WAVE_SHAPE_TUBE_6V6:               
      waveshape_effect.shape((float *)wave_shape_tube_6v6, 513);
      Serial.println("WAVE_SHAPE_TUBE_6V6");
    break;
    case WAVE_SHAPE_TUBE_12AT7:           
      waveshape_effect.shape((float *)wave_shape_tube_12at7, 513);
      Serial.println("WAVE_SHAPE_TUBE_12AT7");
    break;
    case WAVE_SHAPE_TUBE_12AU7:               
      waveshape_effect.shape((float *)wave_shape_tube_12au7, 513);
      Serial.println("WAVE_SHAPE_TUBE_12AU7");
    break;
    case WAVE_SHAPE_TUBE_12AX7:           
      waveshape_effect.shape((float *)wave_shape_tube_12ax7, 513);
      Serial.println("WAVE_SHAPE_TUBE_12AX7");
    break;
    case WAVE_SHAPE_TUBE_12AX7_1025:           
      Serial.println("WAVE_SHAPE_TUBE_12AX7_1025");
      waveshape_effect.shape((float *)wave_shape_tube_12ax7_1025, 1025);
    break;
    case WAVE_SHAPE_TUBE_KT88:               
      waveshape_effect.shape((float *)wave_shape_tube_kt88, 513);
      Serial.println("WAVE_SHAPE_TUBE_KT88");
    break;
    default:
      Serial.println("Washeshape not assigned");
      waveShapeEffectNumber = 0;
    break;
  }
  }
}