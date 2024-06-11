//=======================================================================================
//
//  THIS IS ONLY INTENDED TO BE USED ON LINUX SYSTEMS, OR VIRTUAL MACHINES
//  THE MOUSE FUNCTIONALITY DOES NOT WORK WITHOUT INSTALLING xdotool
//
//=======================================================================================
#include <linux/uinput.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <cerrno>
#include "CharToKeyCode.h"
#include <cstdlib>

#define POST_INPUT_DELAY() usleep(10000); //.01s delay

class automation
{
    int fd;
    uinput_user_dev uidev{};

public:
    void setKey(int keycode, int value) const
    {
        input_event ev;
        memset(&ev, 0, sizeof(ev));
        ev.type = EV_KEY;
        ev.code = keycode;
        ev.value = value; // Press or release
        if (write(fd, &ev, sizeof(ev)) < 0) {
            std::cerr << "Error writing event: " << strerror(errno) << std::endl;
            close(fd);
            exit(-1);
        }
        ev.type = EV_SYN;
        ev.code = SYN_REPORT;
        ev.value = 0;

        if (write(fd, &ev, sizeof(ev)) < 0) {
            std::cerr << "Error writing sync event: " << strerror(errno) << std::endl;
            close(fd);
            exit(-1);
        }
    }
    void quickPress(int keyCode) const
    {
        setKey(keyCode, 1); // Press
        POST_INPUT_DELAY()
        setKey(keyCode, 0); // Release
        POST_INPUT_DELAY()
    }
    void inputString(std::string in) const
    {
        for (int i = 0; i < in.length(); i++)
            quickPress( char_to_keycode(in.at(i)) );
    }
    void leftClick()
    {
        system("xdotool click 1");
        std::cout<<"\nclicked at ";
        std::cout<<system("xdotool getmouselocation") << "\n";
        POST_INPUT_DELAY()
    }
    void setMousePosition(int x = 0, int y = 0 ) const
    {
        system(("xdotool mousemove " + std::to_string(x) + " " + std::to_string(y)).c_str());
        POST_INPUT_DELAY()
    }
    void changeMousePosition(int x = 0, int y = 0)
    {
        std::cout<<"mouse pos after move: " << system("xdotool getmouselocation") << "\n";
        system(  ("xdotool mousemove_relative " + std::to_string(x) + " " + std::to_string(y)).c_str());
        std::cout<<"mouse pos before move: " << system("xdotool getmouselocation") << "\n";
        POST_INPUT_DELAY()
    }
    void forkAndRunCmd(std::string in)
    {
        auto pid = fork();
        if(pid == 0)
        {
            system(in.c_str());
            exit(0);
        }
    }

    automation()
    {
        //get screen dimensions for mouse input

        //setup virtual input device for keyboard events

        fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
        if (fd < 0){std::cerr << "Error opening /dev/uinput: " << strerror(errno) << std::endl; exit(-1);} //check for error

        ioctl(fd, UI_SET_EVBIT, EV_KEY);
        //enable sync
        ioctl(fd, UI_SET_EVBIT, EV_SYN);
        // Enable all keys made available by char to key code
        for(auto mapped_key_code : CHAR_KEYCODE_MAP)
            ioctl(fd, UI_SET_KEYBIT, mapped_key_code.second);


        memset(&uidev, 0, sizeof(uidev));
        snprintf(uidev.name, UINPUT_MAX_NAME_SIZE, "uinput-sample");

        uidev.id.bustype = BUS_USB;
        uidev.id.vendor = 0x1234;
        uidev.id.product = 0x5678;
        uidev.id.version = 1;


        //bounds
        uidev.absmin[ABS_X] = 0;
        uidev.absmax[ABS_X] = 1920;
        uidev.absmin[ABS_Y] = 0;
        uidev.absmax[ABS_Y] = 1080;


        if (write(fd, &uidev, sizeof(uidev)) < 0) {
            std::cerr << "Error writing device information: " << strerror(errno) << std::endl;
            close(fd);
            exit(-1);
        }

        if (ioctl(fd, UI_DEV_CREATE) < 0) {
            std::cerr << "Error creating uinput device: " << strerror(errno) << std::endl;
            close(fd);
            exit(-1);
        }
    }
    ~automation()
    {
        if (ioctl(fd, UI_DEV_DESTROY) < 0) {
            std::cerr << "Error destroying uinput device: " << strerror(errno) << std::endl;
            close(fd);
        }
        close(fd);
    }
};
