#include <SoftwareSerial.h>

#define HCRx 16
#define HCTx 15
SoftwareSerial HC05(HCRx, HCTx);

// Pin Kontrol Motor L298N
#define IN4 7 // Motor Kanan (-)
#define IN3 2 // Motor Kanan (+)
#define IN2 4 // Motor Kiri (-)
#define IN1 3 // Motor Kiri (+)
#define ENA 5 // PWM Kecepatan Motor Kiri
#define ENB 6 // PWM Kecepatan Motor Kanan

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
    delay(2);
    char c = HC05.read();
    voice += c;
  }

  voice.trim();

  if (voice.length() > 0) {
    Serial.print("Perintah Diterima: ");
    Serial.println(voice);

    if (voice == "forward" || voice == "forwards" || voice == "maju" || voice == "A") {
      /*
       * MAJU (1,0, 1,0)
       * - Motor Kiri  (IN1=1, IN2=0) -> Roda kiri berputar MAJU
       * - Motor Kanan (IN3=1, IN4=0) -> Roda kanan berputar MAJU
       * - Hasil: Robot bergerak lurus ke depan
       */
      motorControl(1, 0, 1, 0, 70, );
    } 
    else if (voice == "backward" || voice == "backwards" || voice == "mundur" || voice == "C") {
      /*
       * MUNDUR (0,1, 0,1)
       * - Motor Kiri  (IN1=0, IN2=1) -> Roda kiri berputar MUNDUR
       * - Motor Kanan (IN3=0, IN4=1) -> Roda kanan berputar MUNDUR
       * - Hasil: Robot bergerak lurus ke belakang
       */
      motorControl(0, 1, 0, 1, 255, 255);
    } 
    else if (voice == "right" || voice == "kanan" || voice == "B") {
      /*
       * KANAN (1,0, 0,0) - KIRI AKTIF
       * - Motor Kiri  (IN1=1, IN2=0) -> Roda kiri MAJU (Aktif)
       * - Motor Kanan (IN3=0, IN4=0) -> Roda kanan BERHENTI (Mati)
       * - Hasil: Roda kiri mendorong robot membungkuk/belok ke KANAN
       */
      motorControl(1, 0, 0, 0, 255, 255);
    } 
    else if (voice == "left" || voice == "kiri" || voice == "D") {
      /*
       * KIRI (0,0, 1,0) - KANAN AKTIF
       * - Motor Kiri  (IN1=0, IN2=0) -> Roda kiri BERHENTI (Mati)
       * - Motor Kanan (IN3=1, IN4=0) -> Roda kanan MAJU (Aktif)
       * - Hasil: Roda kanan mendorong robot membungkuk/belok ke KIRI
       */
      motorControl(0, 0, 1, 0, 255, 255);
    } 
    else if (voice == "stop" || voice == "berhenti" || voice == "S") {
      /*
       * BERHENTI (0,0, 0,0)
       * - Motor Kiri  (IN1=0, IN2=0) -> Roda kiri BERHENTI
       * - Motor Kanan (IN3=0, IN4=0) -> Roda kanan BERHENTI
       * - Hasil: Robot berhenti total
       */
      motorControl(0, 0, 0, 0, 0, 0);
    } 
    else {
      motorControl(0, 0, 0, 0, 0, 0);
    }
  }
}

// Fungsi kontrol motor: motorControl(IN1, IN2, IN3, IN4, SpeedL, SpeedR)
void motorControl(int In1, int In2, int In3, int In4, int SpeedL, int SpeedR) {
  digitalWrite(IN1, In1); // Kontrol Arah Motor Kiri (+)
  digitalWrite(IN2, In2); // Kontrol Arah Motor Kiri (-)
  digitalWrite(IN3, In3); // Kontrol Arah Motor Kanan (+)
  digitalWrite(IN4, In4); // Kontrol Arah Motor Kanan (-)
  analogWrite(ENA, SpeedL); // Kecepatan Motor Kiri (0-255)
  analogWrite(ENB, SpeedR); // Kecepatan Motor Kanan (0-255)
}