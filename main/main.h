#include <StandardCplusplus.h>
#include <system_configuration.h>
#include <unwind-cxx.h>
#include <vector>
#include <utility.h>
using namespace std;
#include "sd_avg.h"
#ifdef __cplusplus
extern "C" {
#endif
void led_on_off(int ledPin, int buttonPin, int &buttonState, int &ledState, int &lastButtonState, int &lastLEDState, float &difference, float &endTime, float &startTime, double &currAvg, double &currSD, int &msgCount, vector<int> &timeDifferences);
void check_user(int &ledPin, int &msgCount, int &ledState, vector<int> &timeDifferences, float &difference);
#ifdef __cplusplus
}
#endif
