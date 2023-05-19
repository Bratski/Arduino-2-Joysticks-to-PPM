#include <usbhid.h> 
#include <hiduniversal.h> 
#include <usbhub.h> 
#include "PPMEncoder.h"

#define OUTPUT_PIN 3

unsigned int debug = 1; // turn serial printing on (1) or off (0) for debugging

// Logitech Attack 3 HID report 
struct GamePadEventData { 
	uint8_t X, Y;
	uint8_t Z1, Z2, Rz; 
} __attribute__((packed)); 

class JoystickEvents { 
	public: virtual void OnGamePadChanged(const GamePadEventData *evt, int joystick); 
}; 

class JoystickReportParser : public HIDReportParser { 
	JoystickEvents *joyEvents; 
	uint8_t joyId; 
	uint8_t oldPad[sizeof(GamePadEventData)]; 
	public: JoystickReportParser(JoystickEvents *evt, uint8_t id) : joyEvents(evt), joyId(id) {} 
	virtual void Parse(USBHID *hid, bool is_rpt_id, uint8_t len, uint8_t *buf); 
}; 

void JoystickReportParser::Parse(USBHID *hid, bool is_rpt_id, uint8_t len, uint8_t *buf) { 
	if (!is_rpt_id && len == sizeof(GamePadEventData)) { 
		// Checking if there are changes in report since the method was last called
	 	bool match = (sizeof(oldPad) == len) && (memcmp(oldPad, buf, len) == 0); 
	 	if (!match && joyEvents) { 
	 		joyEvents->OnGamePadChanged((const GamePadEventData*)buf, joyId); 
	 		memcpy(oldPad, buf, len); 
	 	}
	} 
} 
 
void JoystickEvents::OnGamePadChanged(const GamePadEventData *evt, int joystick) { 
	// Serial.print("Joystick "); 
	// Serial.print(joystick); 
	if (joystick == 1) {
		if (debug == 1){
			Serial.print(" J1 X raw: "); Serial.print(evt->X);
			Serial.print(" J1 Y raw: "); Serial.print(evt->Y);
			Serial.print(" J1 Z1 raw: "); Serial.print(evt->Z1); 
			Serial.print(" J1 Z2 raw: "); Serial.print(evt->Z2); 
			Serial.print(" J1 Rz raw: "); Serial.print(evt->Rz); 
			Serial.println();
		} 
		ppmEncoder.setChannelPercent(0, map(evt->X, 0, 255, 0, 50));
		ppmEncoder.setChannelPercent(1, map(evt->Y, 0, 255, 0, 50));
		ppmEncoder.setChannelPercent(2, map(evt->Z1, 0, 255, 0, 50));
		ppmEncoder.setChannelPercent(3, map(evt->Z2, 0, 255, 0, 50)); // buttons
	}
	else if (joystick == 2) {
		if (debug == 1){
			Serial.print(" J2 X raw: "); Serial.print(evt->X);
			Serial.print(" J2 Y raw: "); Serial.print(evt->Y);
			Serial.print(" J2 Z1 raw: "); Serial.print(evt->Z1); 
			Serial.print(" J2 Z2 raw: "); Serial.print(evt->Z2); 
			Serial.print(" J2 Rz raw: "); Serial.print(evt->Rz); 
			Serial.println();
		}

		ppmEncoder.setChannelPercent(4, map(evt->X, 0, 255, 0, 50));
		ppmEncoder.setChannelPercent(5, map(evt->Y, 0, 255, 0, 50));
		ppmEncoder.setChannelPercent(6, map(evt->Z1, 0, 255, 0, 50));
		ppmEncoder.setChannelPercent(7, map(evt->Z2, 0, 255, 0, 50)); // buttons 
	}
} 

USB Usb; 
USBHub Hub(&Usb); 
HIDUniversal Hid1(&Usb); // first Joystick 
HIDUniversal Hid2(&Usb); // second Joystick 

JoystickEvents JoyEvents; 
JoystickReportParser Joy1(&JoyEvents, 1); 
JoystickReportParser Joy2(&JoyEvents, 2); 

void setup() { 
	Serial.begin(115200); 
	// #if !defined(__MIPSEL__) while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection 
	// #endif 
	
	Serial.println("Start"); 
	
	ppmEncoder.begin(OUTPUT_PIN);

	if (Usb.Init() == -1) { 
		Serial.println("OSC did not start."); 
	} 
	
	delay(200); 
	
	if (!Hid1.SetReportParser(0, &Joy1) || !Hid2.SetReportParser(1, &Joy2)) { 
		ErrorMessage<uint8_t>(PSTR("SetReportParser"), 1); 
	} 

} 

void loop() {
	Usb.Task(); 
} 