#include <Windows.h>
#include <ViGem/Client.h>
#include <ViGem/Common.h>
#include <ViGem/Util.h>
#include <ViGem/km/BusShared.h>

#include <iostream>

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

int main(){
    std::cout << "Initialising..." << std::endl;
    PVIGEM_CLIENT driverHook = vigem_alloc();
    VIGEM_ERROR err = vigem_connect(driverHook);
    if(err != VIGEM_ERROR_NONE){
        std::cout << "There was an error connecting to the ViGEm driver. Code: " << std::hex << err << std::dec 
                  << "\n  This code has a name, seen in ViGEmClient's Client.h file." << std::endl;
        return 1;
    }
    PVIGEM_TARGET nullDevice = vigem_target_x360_alloc();

    err = vigem_target_add(driverHook, nullDevice);
    if(err != VIGEM_ERROR_NONE){
        std::cout << "There was an error creating the virtual device. Code: " << std::hex << err << std::dec
                  << "\n  This code has a name, seen in ViGEmClient's Client.h file." << std::endl;
        return 2;
    }

    vigem_target_x360_register_notification(driverHook, nullDevice, notify_x360, nullptr);

    ////Running loop/////////////////////////////
    std::cout << "Null device running, press q to quit." << std::endl;
    bool running = true;
    while(running){
        std::cout << ">>> ";
        std::string in;
        std::cin >> in;
        if(in == "q"){
            running = false;
        }
    }
    
    ////Closing up///////////////////////////////
    vigem_target_x360_unregister_notification(nullDevice);
    vigem_target_remove(driverHook, nullDevice);
    vigem_target_free(nullDevice);
    vigem_disconnect(driverHook);
    vigem_free(driverHook);
    return 0;
}