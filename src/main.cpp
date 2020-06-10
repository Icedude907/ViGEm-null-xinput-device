#include <Windows.h>
#include <signal.h>

#include <ViGem/Client.h>
#include <ViGem/Common.h>
#include <ViGem/Util.h>
#include <ViGem/km/BusShared.h>

#include <iostream>
#include <thread>

// Invoked for a lot of reasons. Does nothing. See Client.h in ViGEmClient for details
VOID CALLBACK notify_x360(
	PVIGEM_CLIENT Client,
	PVIGEM_TARGET Target,
	UCHAR LargeMotor,
	UCHAR SmallMotor,
	UCHAR LedNumber,
	LPVOID UserData
){

}
static PVIGEM_CLIENT driverHook;
static PVIGEM_TARGET nullDevice;
static bool running = true;
void quitHandler(int signalID){
    if(!running){return;} //If already closing due to another event don't close again.
    running = false;
    std::cout << "Exiting..." << std::endl;
    vigem_target_x360_unregister_notification(nullDevice);
    vigem_target_remove(driverHook, nullDevice);
    vigem_target_free(nullDevice);
    vigem_disconnect(driverHook);
    vigem_free(driverHook);
}

int main(){
    std::cout << "Initialising..." << std::endl;
    driverHook = vigem_alloc();
    VIGEM_ERROR err = vigem_connect(driverHook);
    if(err != VIGEM_ERROR_NONE){
        std::cout << "There was an error connecting to the ViGEm driver. Code: " << std::hex << err << std::dec 
                  << "\n  This code has a name, seen in ViGEmClient's Client.h file." << std::endl;
        return 1;
    }
    nullDevice = vigem_target_x360_alloc();

    err = vigem_target_add(driverHook, nullDevice);
    if(err != VIGEM_ERROR_NONE){
        std::cout << "There was an error creating the virtual device. Code: " << std::hex << err << std::dec
                  << "\n  This code has a name, seen in ViGEmClient's Client.h file." << std::endl;
        return 2;
    }

    vigem_target_x360_register_notification(driverHook, nullDevice, notify_x360, nullptr);

    //Set a signal handler to catch window closes and exit rightly.
    signal(SIGINT, &quitHandler);
    signal(SIGBREAK, &quitHandler);

    ////Running loop/////////////////////////////
    PULONG index = 0;
    vigem_target_x360_get_user_index(driverHook, nullDevice, index);
    std::cout << "Null device running, with user index " << index << ".\n  Press q to quit. Press p to push the left stick down and move the stick for a second." << std::endl;
    while(true){
        std::cout << ">>> ";
        std::string in;
        std::cin >> in;
        if(in == "q"){
            //Closing
            quitHandler(INT_MIN);
            break;
        }else if(in == "p"){
            XUSB_REPORT x = {};
            x.wButtons = _XUSB_BUTTON::XUSB_GAMEPAD_LEFT_THUMB;
            x.sThumbLX = 30000;
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            vigem_target_x360_update(driverHook, nullDevice, x);
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            x.wButtons = 0;
            x.sThumbLX = 0;
            vigem_target_x360_update(driverHook, nullDevice, x);
            std::cout << "  done." << std::endl;
        }
    }
    return 0;
}