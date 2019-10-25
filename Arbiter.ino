const char ADDR[] = {52, 50, 48, 46, 44, 42, 40, 38, 36, 34, 32, 30, 28, 26, 24, 22};
const char DATA[] = {53, 51, 49, 47, 45, 43, 41, 39};
#define CLOCK 2
#define READ_WRITE 3

enum { START_ADDR = 0x1234, };
static int count = 0;

void setup()
{
    for(int n = 0; n < 16; n += 1)
    {
        pinMode(ADDR[n], INPUT);
    }
    for(int n = 0; n < 8; n += 1)
    {
        pinMode(DATA[n], INPUT);
    }
    pinMode(CLOCK, INPUT);
    pinMode(READ_WRITE, INPUT);
    attachInterrupt(digitalPinToInterrupt(CLOCK), onClock, RISING);
    Serial.begin(115200);
}

void onClock()
{
    char output[21];
    uint16_t address = 0;
    uint8_t data = 0;

    for(int n = 0; n < 16; n += 1)
    {
        int bit = digitalRead(ADDR[n]) ? 1 : 0;
        address = (address << 1) + bit;
    }
    if(digitalRead(READ_WRITE))
    {
        if(address == 0xFFFC) // Lower byte
        {
            data = START_ADDR;
        }
        else if(address == 0xFFFD) // Upper byte
        {
            data = START_ADDR >> 8;
        }
        else
        {
            data = 0xEA;
        }
        for(int n = 0; n < 8; n += 1)
        {
            pinMode(DATA[n], OUTPUT); // Set data bus to output
        }
        for(int n = 0; n < 8; n += 1)
        {
            int bit = (data & (1 << n)) ? 1 : 0;
            digitalWrite(DATA[n], bit);
        }
        for(int n = 0; n < 8; n += 1)
        {
            pinMode(DATA[n], INPUT);
        }
    }
    else
    {
        for(int n = 0; n < 8; n += 1)
        {
            int bit = digitalRead(DATA[n]) ? 1 : 0;
            data = (data << 1) + bit;
        }
    }
    sprintf(output, "%04i: A:%04X,%c D:%02X", count, address, digitalRead(READ_WRITE) ? 'r' : 'W', data);
    Serial.println(output);
    ++count;
}

void loop()
{
}
