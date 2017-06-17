#include "Mouse.h"
#include "trackpoint.h"

#define DATA  2
#define CLOCK 3
#define RESET 4

static const char buttonStates[] = { MOUSE_LEFT, MOUSE_RIGHT, MOUSE_MIDDLE };

TrackPoint trackpoint(CLOCK, DATA, RESET);

void clockInterrupt()
{
    trackpoint.readData();
}

void setup()
{
    Mouse.begin();

    trackpoint.reset();
    trackpoint.enable();

    attachInterrupt(digitalPinToInterrupt(CLOCK), clockInterrupt, FALLING);
}

void loop()
{
    if (trackpoint.reportReady())
    {
        const DataReport& report = trackpoint.getDataReport();

        for (uint8_t i = 0; i < sizeof(buttonStates); ++i)
        {
            if (report.state & (1 << i))
            {
                Mouse.press(buttonStates[i]);
            }
            else if (Mouse.isPressed(buttonStates[i]))
            {
                Mouse.release(buttonStates[i]);
            }
        }

        Mouse.move(report.x, -report.y);
    }
}
