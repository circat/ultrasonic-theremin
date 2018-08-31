#include <MIDI.h>
#include <midi_Defs.h>
#include <midi_Message.h>
#include <midi_Namespace.h>
#include <midi_Settings.h>


const int trigPin = 2;
const int echoPin = 4;

void setup() {
 MIDI_CREATE_DEFAULT_INSTANCE();
 Serial.begin(115200);
     // Connect the handleNoteOn function to the library,
    // so it is called upon reception of a NoteOn.
//setHandleControlChange   (byte channel, byte number, byte value)
//void HandleControlChange (byte channel, byte number, byte value)
MIDI.setHandleControlChange(HandleControlChange);  // Put only the name of the function
    MIDI.begin(MIDI_CHANNEL_OMNI);
}

void loop()
{
 // establish variables for duration of the ping,
 // and the distance result in inches and centimeters:
 long duration, inches, cm;

 // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
 // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
 pinMode(trigPin, OUTPUT);
 digitalWrite(trigPin, LOW);
 delayMicroseconds(2);
 digitalWrite(trigPin, HIGH);
 delayMicroseconds(10);
 digitalWrite(trigPin, LOW);

 // Read the signal from the sensor: a HIGH pulse whose
 // duration is the time (in microseconds) from the sending
 // of the ping to the reception of its echo off of an object.
 pinMode(echoPin, INPUT);
 duration = pulseIn(echoPin, HIGH);

 // convert the time into a distance
 inches = microsecondsToInches(duration);
 cm = microsecondsToCentimeters(duration);
 
 Serial.print(inches);
 Serial.print("in, ");
 Serial.print(cm);
 Serial.print("cm");
 Serial.println();
 
 cm = map(cm, 0, 250, 0, 127);
 send(ControlChange,7,cm,1);

}

long microsecondsToInches(long microseconds)
{
 // According to Parallax's datasheet for the PING))), there are
 // 73.746 microseconds per inch (i.e. sound travels at 1130 feet per
 // second).  This gives the distance travelled by the ping, outbound
 // and return, so we divide by 2 to get the distance of the obstacle.
 // See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
 return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds)
{
 // The speed of sound is 340 m/s or 29 microseconds per centimeter.
 // The ping travels out and back, so to find the distance of the
 // object we take half of the distance travelled.
 return microseconds / 29 / 2;
}
