#include <TMRpcm.h>
#include <pcmConfig.h>
#include <pcmRF.h>
#include <SD.h>
#include <SPI.h>
#include <string.h>
#define CS 10

TMRpcm tmrpcm;

void setup() {
  tmrpcm.speakerPin = 9;
  Serial.begin(115200);
  if(!SD.begin(CS)){
    Serial.println("SD fail");
    return;
  }
  tmrpcm.setVolume(5);
}

void loop() {
  if(Serial.available()){
    String data = Serial.readString();
    if(data == "VACA"){
      tmrpcm.play("vaca.wav");
    }
    if(data == "PERRO"){
      tmrpcm.play("perro.wav");
    }
    if(data == "GATO"){
      tmrpcm.play("gato.wav");
    }
    if(data == "POLLO"){
      tmrpcm.play("gallina.wav");
    }
    if(data == "PATO"){
      tmrpcm.play("pato.wav");
    }
    if(data == "RANA"){
      tmrpcm.play("rana.wav");
    }
    if(data == "LEON"){
      tmrpcm.play("leon.wav");
    }
    if(data == "ELEFANTE"){
      tmrpcm.play("elefante.wav");
    }
    if(data == "CABALLO"){
      tmrpcm.play("caballo.wav");
    }
  }

}
