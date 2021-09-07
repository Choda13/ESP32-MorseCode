#include <Arduino.h>
#include <string.h>

//PINOUTS
#define LED_BUILTIN 1
#define INPUT_PIN 19
#define SPEAKER 21
#define DELAY_MICROS 2000

//MORSE DURATION
#define UNIT 50
#define DOT UNIT
#define DASH 3 * UNIT //DASH traje kao tri tockice
#define LETTER_SPACE 3 * UNIT
#define WORD_SPACE 7 * UNIT

#define SPACE_INDEX 36 // index od space znaka u nizu alphabet
#define MASK 128	   // 0b10000000

unsigned char alphabet[] = {
	0b00000101, // A
	0b00011000, // B
	0b00011010, // C
	0b00001100, // D
	0b00000010, // E
	0b00010010, // F
	0b00001110, // G
	0b00010000, // H
	0b00000100, // I
	0b00010111, // J
	0b00001101, // K
	0b00010100, // L
	0b00000111, // M
	0b00000110, // N
	0b00001111, // O
	0b00010110, // P
	0b00011101, // Q
	0b00001010, // R
	0b00001000, // S
	0b00000011, // T
	0b00001001, // U
	0b00010001, // V
	0b00001011, // W
	0b00011001, // X
	0b00011011, // Y
	0b00011100, // Z
	0b00111111, // 0
	0b00101111, // 1
	0b00100111, // 2
	0b00100011, // 3
	0b00100001, // 4
	0b00100000, // 5
	0b00110000, // 6
	0b00111000, // 7
	0b00111100, // 8
	0b00111110, // 9
	0			// space
};

void Beep(unsigned long duration);
void PrintMorse(char *input_ptr, int length);
bool EncodeString(char *input_ptr, int length);

void setup()
{
	Serial.begin(115200);
	pinMode(LED_BUILTIN, OUTPUT);
	pinMode(SPEAKER, OUTPUT);
	pinMode(INPUT_PIN, INPUT_PULLUP);
}

void loop()
{
	char tekst[] = "Pusi kurac";
	int len = strlen(tekst);
	bool res = EncodeString(tekst, len);
	if (res)
		PrintMorse(tekst, len);
	Serial.print("\n");
	delay(3000);
}

//Beeps for given duration
void Beep(unsigned long duration)
{
	unsigned long start = millis();
	while (true)
	{
		digitalWrite(LED_BUILTIN, HIGH);
		digitalWrite(SPEAKER, HIGH);
		delayMicroseconds(DELAY_MICROS);
		digitalWrite(LED_BUILTIN, LOW);
		digitalWrite(SPEAKER, LOW);
		delayMicroseconds(DELAY_MICROS);
		if (millis() - start >= duration)
			return;
	}
}

void PrintMorse(char *input_ptr, int length)
{
	for (int i = 0; i < length; i++)
	{
		unsigned char c = alphabet[input_ptr[i]];
		//gleda da li je vrednost 0 tj. da li je vrednost space
		if (!c)
		{
			delay(WORD_SPACE);
			Serial.print("   ");
			continue;
		}
		else
		{
			bool started = false;
			for (int j = 0; j < 8; j++)
			{
				if ((c << j) & MASK)
					if (started)
					{
						Beep(DASH);
						if (j != 7)
							delay(UNIT);
						Serial.print("-");
					}
					else
						started = true;
				else if (started)
				{
					Beep(DOT);
					if (j != 7)
						delay(UNIT);
					Serial.print(".");
				}
			}
		}
		delay(LETTER_SPACE);
		Serial.print(" ");
	}
}

bool EncodeString(char *input_ptr, int length)
{
	for (int i = 0; i < length; i++)
	{
		if (isalpha(input_ptr[i]))
			input_ptr[i] = toupper(input_ptr[i]) - 'A';
		else if (isdigit(input_ptr[i]))
			input_ptr[i] = input_ptr[i] - 22; //22 je offset za poziciju u nizu "alphabet"
		else if (input_ptr[i] == ' ')
			input_ptr[i] = SPACE_INDEX;
		else
			return false;
	}
	return true;
}