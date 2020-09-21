#include <ACS712.h>

class ACSReader {
    private:
    float voltage;
    float lastVal = 0;
    float suppliedWatts = 0;
    int lastProcessTime = 0;

    public:
    ACS712 acs;
    
    ACSReader(float refVoltage, uint8_t pin, float voltage, uint16_t maxADC, uint8_t mVperA): acs(pin, voltage, maxADC, mVperA) {
        this->voltage = refVoltage;
    }

    float getMedianDC(int loops = 4, int msInterval = 50) {
        float mA = 0;
        int curProcessTime = 0;
        for (int i = 0; i < loops; i++) {
            mA -= (float)acs.mA_DC();
            delay(msInterval);
        }
        lastVal = mA/loops;

        curProcessTime = millis();
        suppliedWatts += getWatt() * (curProcessTime - lastProcessTime) / 1000;
        lastProcessTime = curProcessTime;

        return lastVal;
    }

    float getWatt() {
        return lastVal * voltage / 1000;
    }

    float getSuppliedWatt() {
        return suppliedWatts;
    }

    char* getSuppliedWattString() {
        char res[10];
        if (abs(suppliedWatts) < 1000) {
            sprintf(res, "%.2fW", suppliedWatts);
        } else if (abs(suppliedWatts) < 1000000) {
            sprintf(res, "%.2fkW", suppliedWatts/1000);
        } else {
            sprintf(res, "%.2fMW", suppliedWatts/1000000);
        }

        return res;
    }

    char* getWorkingTime() {
        char res[9];
        int curProcessSecs = (int)lastProcessTime / 1000;
        int hours = floor(curProcessSecs / 3600);
        int minutes = floor((curProcessSecs % 3600) / 60);
        int seconds = curProcessSecs % 60;

        sprintf(res, "%02d:%02d:%02d", hours, minutes, seconds);
        return res;
    }
};