#include <SoftwareSerial.h>

#define HCRx 16
#define HCTx 15
SoftwareSerial HC05(HCRx, HCTx);

// Pin Kontrol Motor L298N
#define IN4 7
#define IN3 2
#define IN2 4
#define IN1 3
#define ENA 5
#define ENB 6

void setup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  Serial.begin(115200);
  HC05.begin(9600);

  // Hentikan motor saat awal menyala
  motorControl(0, 0, 0, 0, 0, 0);
}

void loop() {
  String voice = "";

  // Membaca data Bluetooth
  while (HC05.available() > 0) {
    delay(2); // Delay singkat untuk memastikan buffer data masuk
    char c = HC05.read();
    voice += c;
  }

  // Menghapus karakter newline atau carriage return jika ada
  voice.trim();

  if (voice.length() > 0) {
    Serial.print("Perintah Diterima: ");
    Serial.println(voice);

    if (voice == "forward" || voice == "forwards" || voice == "maju" || voice == "A") {
      // Maju (Kedua motor berputar ke depan)
      motorControl(1, 0, 1, 0, 255, 255);
    } 
    else if (voice == "backward" || voice == "backwards" || voice == "mundur" || voice == "C") {
      // Mundur (Kedua motor berputar ke belakang)
      motorControl(0, 1, 0, 1, 255, 255);
    } 
    else if (voice == "right" || voice == "kanan" || voice == "B") {
      // Belok Kanan (Motor kiri maju, motor kanan mundur/mati)
      motorControl(1, 0, 0, 1, 255, 255);
    } 
    else if (voice == "left" || voice == "kiri" || voice == "D") {
      // Belok Kiri (Motor kiri mundur/mati, motor kanan maju)
      motorControl(0, 1, 1, 0, 255, 255);
    } 
    else if (voice == "stop" || voice == "berhenti" || voice == "S") {
      // Berhenti
      motorControl(0, 0, 0, 0, 0, 0);
    } 
    else {
      // Hentikan motor jika perintah tidak dikenali
      motorControl(0, 0, 0, 0, 0, 0);
    }
  }
}

// Fungsi kontrol motor
void motorControl(int In1, int In2, int In3, int In4, int SpeedL, int SpeedR) {
  digitalWrite(IN1, In1);
  digitalWrite(IN2, In2);
  digitalWrite(IN3, In3);
  digitalWrite(IN4, In4);
  analogWrite(ENA, SpeedL);
  analogWrite(ENB, SpeedR);
}
