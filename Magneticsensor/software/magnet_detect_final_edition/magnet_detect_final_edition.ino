// ======================================
// Automatic Hall Detection
// Stable Field → Measure
// ======================================

// ----- Pin -----
const int hallPin = A0;

// ----- Mode -----
const bool debugMode = true; 

// ----- Sensor -----
int hallValue = 0;
int centerValue = 0;

int strength = 0;
int previousStrength = 0;

// ----- Detection -----
int stableCount = 0;
bool measured = false;

// ----- Threshold -----

const int detectThreshold = 12;  
const int releaseThreshold = 12;  

const int stableDelta = 5;
const int stableRequired = 4;

// ======================================

void setup()
{
    Serial.begin(115200);
    delay(2000);

    Serial.println("Calibrating...");

    long sum = 0;
    
    for(int i = 0; i < 100; i++)
    {
        sum += analogRead(hallPin);
        delay(5);
    }
    centerValue = sum / 100;

    Serial.print("Center = ");
    Serial.println(centerValue);
    Serial.println("System Ready");
}

// ======================================

// ======================================
void measureStone(int currentHall, int currentStrength)
{
    Serial.println();
    Serial.println("===== RESULT =====");

    if(currentStrength < detectThreshold)
    {
        Serial.println("NO STONE");
    }
    else
    {
        
        if(currentHall > centerValue)
        {
            Serial.println("DIRECTION: UP");
        }
        else
        {
            Serial.println("DIRECTION: DOWN");
        }
    }
    Serial.println("==================");
}

// ======================================

void loop()
{
    // ----- Read -----
    hallValue = analogRead(hallPin);

    strength = abs(hallValue - centerValue);


    int delta = abs(strength - previousStrength);

    // =====================
    // Stable Detection
    // =====================
    if(strength > detectThreshold && delta < stableDelta)
    {
        stableCount++;
    }
    else
    {
        stableCount = 0;
    }

    // =====================

    // =====================
    if(stableCount >= stableRequired && !measured)
    {
        
        measureStone(hallValue, strength); 

        measured = true; 
        stableCount = 0;
    }

    // =====================

    // =====================
    if(strength < releaseThreshold)
    {
        measured = false;
    }

    // =====================
    // Debug Output
    // =====================
    if(debugMode)
    {

        Serial.println(hallValue); 
    }

    previousStrength = strength;

    if(debugMode)
    {
        delay(50); 
    }
    else
    {
        delay(5);  
    }
}