// Gustavo A. Furlan - RA: 2576
// Sistemas Embarcados - 2022/2
// Trabalho pratico do 2 Bimestre
// Prof. Me. Camila Galo

// Componentes Analogicos
int LDRSensor = A0;

// Componentes Digitais
int pirSensor = 2;
int ultrassonicEcho = 3;
int ultrassonicTrigger = 4;
int buzzer = 5;
int blueLed[4] = {6, 7, 8, 9};
int yellowLed = 10;
int greenLed = 11;
int offButton = 12;
int onButton = 13;

// Variaveis de apoio
bool isOn = 0;
unsigned long duration = 0;
float distance = 0;

void setup() { // Setup de entradas e saidas
  pinMode(buzzer, OUTPUT);
  pinMode(blueLed[0], OUTPUT);
  pinMode(blueLed[1], OUTPUT);
  pinMode(blueLed[2], OUTPUT);
  pinMode(blueLed[3], OUTPUT);
  pinMode(yellowLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(ultrassonicTrigger, OUTPUT);

  pinMode(ultrassonicEcho, INPUT);
  pinMode(offButton, INPUT);
  pinMode(onButton, INPUT);
  pinMode(pirSensor, INPUT);
  
  Serial.begin(9600);
}

void loop() { // Função ciclica principal
  isOn = digitalRead(onButton) ? !isOn : isOn; // verifica se o sistema deve ficar online
  if (!isOn) return; // se não estiver online, retorna sem executar o resto do codigo
  SetModeOn(); // chama func de setar os LEDs de estado para o modo online.
  if (!ProximityDetected() && !LightDetected() && !PresenceDetected()) return; // verifica sensores
  FlashLeds(); // pisca os leds simbolizando alarme disparado
  PlayAlertSound(); // ativa o buzzer para tocar som de alarme disparado
  isOn = digitalRead(offButton) ? !isOn : isOn; // verifica se o botão off
  if (!isOn) SetModeOff(); // se o sistema estiver offline, configura os leds de estado
}

void SetModeOn() { // Configura LEDs de estado para modo ON
  digitalWrite(yellowLed, LOW);
  digitalWrite(greenLed, HIGH);
}

void SetModeOff() { // Configura LEDs de estado para modo OFF
  digitalWrite(yellowLed, HIGH);
  digitalWrite(greenLed, LOW);
}

void PlayAlertSound() { // Toca som de alerta pelo Buzzer
  for (int i = 0; i < 5; i++) {
    tone(buzzer, 200, 200);
    delay(100);
    tone(buzzer, 500, 200);
    delay(100);
  }
}

void FlashLeds() { // Pisca os 4 LEDs de alerta
  for (int i = 0; i < 4; i++) {
    digitalWrite(blueLed[i], HIGH);
    delay(50);
  }
  
  for (int i = 3; i >= 0; i--) {
    digitalWrite(blueLed[i], LOW);
    delay(50);
  }

  for (int i = 3; i >= 0; i--) {
    digitalWrite(blueLed[i], HIGH);
    delay(50);
  }

  for (int i = 0; i < 4; i++) {
    digitalWrite(blueLed[i], LOW);
    delay(50);
  }
}

bool ProximityDetected() { // Sensor de distancia ultrassonico
  digitalWrite(ultrassonicTrigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(ultrassonicTrigger, LOW);
  duration = pulseIn(ultrassonicEcho, HIGH);
  distance = ((float)duration / 2.0f) / 29.1f;
  return (distance < 22) ? true : false;
}

bool LightDetected() { // Sensor de Luminosidade (LDR)
  return analogRead(LDRSensor) > 10 ? true : false;
}

bool PresenceDetected() { // Sensor de Presença (PIR)
  return digitalRead(pirSensor) ? true : false;
}