#arduino_interval

This project has the goal of collecting sensor measurements at specific intervals on the Arduino microcontroller platform.

Let's say, for example, that I want to record a temperature measurement once every hour.

When powered from a host computer this is straightforward, using a serial port to write out the measurements.

But for a battery powered system things become more complicated. We need conserve power by using sleep mode between measurements and
therefore:

- We need a way to wake up the system at specific intervals
- We need to take measurements and write them to a SD card
- We need to fall back into sleep mode

There are several ways to do this on the Arduino

I am going to describe each of these and provide practical code and circuits that you can adapt to your own projects.


1. Using millis()

2. Using sleep() and the Arduino watchdog timer to interrupt the sleep

3. Using an external Real Time Clock to interrupt


To achieve this we need to understand several system components

- Taking a sensor measurment
- Writing it to a SD card
- Using an External Interrupt
- Using the sleep library
- Using the Watchdog timer
- Using an external Real Time Clock (RTC)

I could add battery power management to this list but I'm intentionally leaving that out of the mix.
Take a look at [Arduino Power](https://github.com/craic/arduino_power) for some ideas on that topic.

The code examples make use of several libraries that were written by others. To keep the code easy to understand
I have wrapped up each of the components. I have also kept the actual sensor measurement component relatively simple.

I'll introduce each of these in turn.

*The Basic Platform

Arduino + Adafruit PowerBoost 500C Shield + LiPo battery

*Sensor Measurement

Here I am using the BME280 temperature, humidity and pressure sensor on a breakout board
from [Adafruit](https://www.adafruit.com/products/2652). This is a nice multiple sensor board that I
want to use in the project that inspired me to write this guide.

Of course, you can use whatever sensors you want, but in the code examples shown here I am going
read temperature data from the sensor.

[ardiuno_interval_1_bmp_sensor](/ardiuno_interval_1_bmp_sensor) contains the basic code for communicating with the sensor board
using the I2C protocol and writing the values to the Serial Monitor.

[Photo of Arduino Interval - Circuit 1](/images/arduino_interval_1_photo.png)

[Layout of Arduino Interval - Circuit 1](/images/arduino_interval_1_layout.png)

The code used here is pretty simple. There are some sensor board includes and defines at the start of the code.
*sensorSetup()* and *sensorRead()* do what you expect. *flashOnboardLed()* flashes the onboard LED on the Arduino if
there is an error.

The sensor is read every *intervalMilliSec* milliseconds and the result send to the Serial Monitor.

The interval between reads is determined by this code.

```arduino
    if(millis() - prevMilliSec >= intervalMilliSec ) {
      Serial.println(sensorRead());
      prevMilliSec = millis();
    }
```
*millis()* returns the number of milliseconds that the program has been running.
*prevMilliSec* is a variable that records when the last reading was taken.
When the *millis()* value exceeds the interval then another reading is taken and *prevMilliSec* is reset.

This is the basic way to handle intervals on the Arduino which allows you to perform other work.
*delay()* also works but blocks other activity - so don't use this for anything significant.





*Using the SD card


*Using an External Interrupt

We will use an External Interrupt to wake up the Arduino in the examples that follow. So it
is important to understand how this type of Interrupt works.

To do this in isolation, here is a simple example that uses a pushbutton switch to trigger the interrupt.

It's a simple example, but in fact we will use this circuit later on in order to check on battery status.















##Resources

Other people have looked at this issue before me. Here are some of the resources that I used in my journey.

Tony DiCola has a tutorial on a [low power wifi data logger](https://learn.adafruit.com/low-power-wifi-datalogging/overview) on the
[Adafruit](https://www.adafruit.com/) website.

Nick Gammon has a great tutorial on reducing [Power consumption on the Arduino](http://www.gammon.com.au/power) -
really comprehensive and a lot of it is beyond my understanding of the Arduino platform.


These are all valuable resources but, speaking for myself, I need simple, stripped down example code to help me focus
in on the essentials. So that is what I am trying to do here.


##Power Down Sleep

In this example the hardware is simply a basic Arduino Uno (or an Adafruit Metro) and the code will simply blink the
on board LED at predefined intervals.

The example is taken from Tony DiCola's tutorial, but I've stripped out the control of the wi-fi chip and SD card to
leave just a basic sleep/wake system.






![Table of Timer Settings](atmega328_watchdog_timer_prescale_select_table.png)





##Keeping track of time

The *millis()* function is normally the easiest way to keep track of elapsed time on the Arduino.

But we can't use that here as it is not incremented when the device is asleep.

We know how long the watchdog timer is set for and how many iterations we require to wake up the system, so that
seems like a good approach. The problem here is that there is a log of up to 65 msec every time the
system wakes up.

In addition, reading a sensor and logging a measurement will itself take time.

The end result is that a sleep period of one minute will actually be slightly longer than this in practice.

For some applications this is not a big deal but for others you want to know exactly when a measurement was taken.

You can use a *Real Time Clock(RTC)* to keep independent track of time, but even with this it would be
extremely difficult to schedule events to occur at very specific timepoints.



