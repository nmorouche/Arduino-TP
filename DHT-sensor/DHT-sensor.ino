#include "DHT.h"

#define DHTPIN D4     // what pin we're connected to
#define DHTTYPE DHT12   // DHT 11
DHT dht(DHTPIN, DHTTYPE);
const int redLed = D1;
const int greenLed = D5;
const int yellowLed = D6;

float* temp = new float[10];
float* median = new float[10];

float* humidity = new float[10];
float* humidityMedian = new float[10];

float previousMoyenne = 0.0;
float previousMoyenneHumidity = 0.0;

int count = 0;

void setup() {
  Serial.begin(9600);
  pinMode(redLed, OUTPUT);
  pinMode(yellowLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  dht.begin();
}

void loop() {
    
  delay(500);
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);

  if(count < 10){
    temp[count] = t;
    humidity[count] = h;
    count += 1;
  } else{
    // Temperatures gestion
    fillAndSortMedianTab();
    displayValues();
    
    //checkTendance();
    checkHumidityTendance();
    
    //previousMoyenne = tempSum();
    previousMoyenneHumidity = humiditySum();
    
    decrementTab(t, h);
   }
  
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Impossible de lire la sonde DHT!");
    return;
  }
}

void checkTendance() {
  if(previousMoyenne < tempSum()) {
    digitalWrite(greenLed, HIGH);
    digitalWrite(redLed, LOW);
    digitalWrite(yellowLed, LOW);
  } else if(previousMoyenne > tempSum()) {
    digitalWrite(redLed, HIGH);
    digitalWrite(greenLed, LOW);
    digitalWrite(yellowLed, LOW);
  } else {
    digitalWrite(yellowLed, HIGH);
    digitalWrite(greenLed, LOW);
    digitalWrite(redLed, LOW);
  }
}

void checkHumidityTendance() {
  if(previousMoyenneHumidity < humiditySum()) {
    digitalWrite(greenLed, HIGH);
    digitalWrite(redLed, LOW);
    digitalWrite(yellowLed, LOW);
  } else if(previousMoyenne > tempSum()) {
    digitalWrite(redLed, HIGH);
    digitalWrite(greenLed, LOW);
    digitalWrite(yellowLed, LOW);
  } else {
    digitalWrite(yellowLed, HIGH);
    digitalWrite(greenLed, LOW);
    digitalWrite(redLed, LOW);
  }
}


float tempSum() {
  float tempSum = 0;
    for(int i = 0; i < 10; i++){
      tempSum += temp[i];
    }
    // Affichage temperature moyenne
    return tempSum = tempSum / 10.0;
}

float humiditySum() {
  float humiditySum = 0;
    for(int i = 0; i < 10; i++){
      humiditySum += humidity[i];
    }
    // Affichage temperature moyenne
    return humiditySum = humiditySum / 10.0;
}

void decrementTab(float t, float h) {
    for(int i = 1; i < 10; i++){
        temp[i-1] = temp[i];
    }
    temp[9] = t;

    for(int i = 1; i < 10; i++){
        humidity[i-1] = humidity[i];
    }
    humidity[9] = h;
}

void displayValues() {
    Serial.print("Température moyenne: ");
    Serial.print(tempSum());
    Serial.println(" °C");
    Serial.print("\nTempérature median: ");
    Serial.print(getMedianValue());
    Serial.println(" °C");

    Serial.print("Humidité moyenne: ");
    Serial.print(humiditySum());
    Serial.println(" %");
    Serial.print("Humidité median: ");
    Serial.print(getHumidityMedianValue());
    Serial.println(" %");
}


void sortMedianTab() {
  int min;
  int minH;
  for(int i = 0; i < 10; i++){
     min = i;
     for(int j = i + 1; j < 10; j++){
         if(median[j] < median[min]){
             min = j;
         }
     }
     if(min != i){
        int medianVal = median[i];
        median[i] = median[min];
        median[min] = medianVal;
     }
  }
  for(int i = 0; i < 10; i++){
     minH = i;
     for(int j = i + 1; j < 10; j++){
         if(humidityMedian[j] < humidityMedian[minH]){
             minH = j;
         }
     }
     if(minH != i){
        int medianVal = humidityMedian[i];
        humidityMedian[i] = humidityMedian[minH];
        humidityMedian[minH] = medianVal;
     }
  }
}

float getMedianValue() {
  return median[4];
}

float getHumidityMedianValue() {
  return humidityMedian[4];
}

void fillAndSortMedianTab() {
  for(int i=0; i<10; i++) {
    median[i] = temp[i];
  }
  for(int i=0; i<10; i++) {
    humidityMedian[i] = humidity[i];
  }
  sortMedianTab();
}
