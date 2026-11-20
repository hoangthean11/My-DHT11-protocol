#define ll long long

inline int get_bit() {
	int i = micros();
	while (digitalRead(4) == 1) { // prevent infinite loop
		if (micros() - i > 100) return -1;
	}

	i = micros();
	while (digitalRead(4) == 0) {
		if (micros() - i > 100) return -1;
	}

	int t1 = i = micros();
	while (digitalRead(4) == 1) {
		if (micros() - t1 > 100) return -1;
	}

	if (micros() - t1 <= 40) return 0;
	else return 1;
}

inline int start_trans() {
	pinMode(4, OUTPUT);
	digitalWrite(4, LOW);
	delay(20);
	pinMode(4, INPUT_PULLUP);

	int t = micros();
	while (digitalRead(4) == 1) {
		if ((micros() - t) > 100) {
			// Serial.println("dead");
			return -1;
		}
	}

	t = micros();
	while (digitalRead(4) == 0) {
		if (micros() - t > 100) {
			// Serial.println("dead");
			return -1;
		}
	}

	// Serial.println("ok");

	return 1;
}

void read() {
	if (start_trans() == -1) {
		// Serial.println("read dead \n\n\n");
		return;
	}
	
	int bits[40];

	for (int i = 0; i < 40; i ++) {
		int k = get_bit();

		if (k == -1) {
			Serial.println("failed");
			return;
		}
		bits[i] = k;
	}

	for (int i = 0; i < 40; i ++) Serial.print(bits[i]);

	ll a, b, c, d;
	a = b = c = d = 0;

	for (int i = 0; i < 8; i ++) a = a * 2 + bits[i];
	for (int i = 8; i < 16; i ++) b = b * 2 + bits[i];
	for (int i = 16; i < 24; i ++) c = c * 2 + bits[i];
	for (int i = 24; i < 32; i ++) d = d * 2 + bits[i];

	ll k = a + b + c + d;

	for (int i = 0; i < 8; i ++) {
		if (((k >> i) & 1) != bits[40 - i - 1]) Serial.println("CHECKSUM FAIL"); 
	}

	Serial.println();
	Serial.print(a);
	Serial.print(" ");
	Serial.print(b);
	Serial.print(" ");
	Serial.print(c);
	Serial.print(" ");
	Serial.print(d);

	Serial.print("\n\n\n\n");
}

void setup() {
	Serial.begin(9600);
	pinMode(4, INPUT_PULLUP);
}

void loop() {
	// Serial.println(micros());

	delay(2000);
	Serial.println();
	Serial.println();
	read();

	Serial.println();
}
