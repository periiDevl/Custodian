#include"Include/Custodian.h"

int main() {
    FileMonitor fm("YourPathHere");
    while (true) {
        printf(fm.DetectDirecChanged(1).c_str());
    }

    return 0;
}
