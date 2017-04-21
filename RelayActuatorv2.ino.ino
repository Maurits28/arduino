// Enable debug prints to serial monitor
#define MY_DEBUG 

// Enable and select radio type attached
#define MY_RADIO_NRF24

// Enable repeater functionality for this node
#define MY_REPEATER_FEATURE
#define MY_NODE_ID 5


#include <SPI.h>
#include <MySensors.h>

#define RELAY_1 3  // Arduino Digital I/O pin number for first relay (second on pin+1 etc)
#define NUMBER_OF_RELAYS 1 // Total number of attached relays
#define RELAY_ON 1  // GPIO value to write to turn on attached relay
#define RELAY_OFF 0 // GPIO value to write to turn off attached relay

MyMessage msgRelay(1, V_STATUS);

void before() { 
  for (int sensor=1, pin=RELAY_1; sensor<=NUMBER_OF_RELAYS;sensor++, pin++) {
    // Then set relay pins in output mode
    pinMode(pin, OUTPUT);   
    // Set relay to last known state (using eeprom storage) 
    digitalWrite(pin, RELAY_OFF);
  }
}

void setup() {
  
}

void presentation()  
{   
  // Send the sketch version information to the gateway and Controller
  sendSketchInfo("Relay", "1.0");

  for (int sensor=1, pin=RELAY_1; sensor<=NUMBER_OF_RELAYS;sensor++, pin++) {
    // Register all sensors to gw (they will be created as child devices)
    present(sensor, S_BINARY);
  }
}


void loop() 
{
  
}

void receive(const MyMessage &message) {
int msgcmd = mGetCommand(message);

  // Verify its a binary command (V_STATUS)
  if (message.type==V_STATUS) {
     // Check if command type 1 is a set command
     if(msgcmd==1) {
        // Change relay state
        digitalWrite(message.sensor-1+RELAY_1, message.getBool()?RELAY_ON:RELAY_OFF);
        // Store state in eeprom
        saveState(message.sensor, message.getBool());
        // Write some debug info
        Serial.print("Incoming change for sensor:");
        Serial.print(message.sensor);
        Serial.print(", New status: ");
        Serial.println(message.getBool());
     }
     else 
     {
        // assume the request is to send a status update. Send status update.
        send(msgRelay.set(digitalRead(message.sensor-1+RELAY_1)== RELAY_ON ? 1 : 0));
        Serial.print("Current status:");
        Serial.print(message.sensor);
     }
   } 
}

