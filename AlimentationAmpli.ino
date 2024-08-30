static const uint8_t pinRelai230V = 5;
static const uint8_t pinRelaiAmpliGauche = A3;
static const uint8_t pinRelaiAmpliDroit = 6;
static const uint8_t pinRelaiProtectionHp = A0;
static const uint8_t pinLed = 13;
static const uint8_t pinBP = 10;

typedef enum
{
    off,
    mainPowerOn,
    leftPowerOn,
    rightPowerOn,
    speakerProtectionOn
}
stage;

void flashLed(uint8_t sequence)
{
    for (int i = 0; i < sequence; i++)
    {
        digitalWrite(pinLed, HIGH);
        delay(20);
        digitalWrite(pinLed, LOW);
        delay(100);
    }

}
void setup() 
{
    pinMode(pinRelai230V, OUTPUT);
    pinMode(pinRelaiAmpliGauche, OUTPUT);
    pinMode(pinRelaiAmpliDroit, OUTPUT);
    pinMode(pinRelaiProtectionHp, OUTPUT);
    pinMode(pinLed, OUTPUT);
    pinMode(pinBP, INPUT_PULLUP);
    Serial.begin(115200);
}

void loop() 
{
    static stage currentStage = off;
    static int delayBetweenStages = 1000; // ms

    delay(delayBetweenStages);
    if (digitalRead(pinBP) == LOW)
    {
        switch (currentStage)
        {
        case off:
            digitalWrite(pinRelai230V, HIGH);
            currentStage = mainPowerOn;
            break;
        case mainPowerOn:
            digitalWrite(pinRelaiAmpliGauche, HIGH);
            currentStage = leftPowerOn;
            break;
        case leftPowerOn:
            digitalWrite(pinRelaiAmpliDroit, HIGH);
            currentStage = rightPowerOn;
            break;
        case rightPowerOn:
            digitalWrite(pinRelaiProtectionHp, HIGH);
            currentStage = speakerProtectionOn;
            break;
        case speakerProtectionOn:
            break;
        }
    }
    else
    {
        switch (currentStage)
        {
        case off:
            break;
        case speakerProtectionOn:
            digitalWrite(pinRelaiProtectionHp, LOW);
            currentStage = rightPowerOn;
            break;
        case mainPowerOn:
        case leftPowerOn:
        case rightPowerOn:
            digitalWrite(pinRelai230V, LOW);
            digitalWrite(pinRelaiAmpliDroit, LOW);
            digitalWrite(pinRelaiAmpliGauche, LOW);
            currentStage = off;
            break;
        }
    }
    flashLed(currentStage + 1);
    //Serial.println(currentStage);
}
