#include <linux/uinput.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <cerrno>
#include "CharToKeyCode.h"

class InputHandler
{
    bool debug = true;
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
    void sendMouseEvent(int type, int code, int value) const
    {
        input_event ev;
        memset(&ev, 0, sizeof(ev));
        ev.type = type;
        ev.code = code;
        ev.value = value;
        write(fd, &ev, sizeof(ev));

        // Send the sync event to ensure the change is registered.
        ev.type = EV_SYN;
        ev.code = SYN_REPORT;
        ev.value = 0;
        write(fd, &ev, sizeof(ev));
    }
    void quickPress(int keyCode) const
    {
        usleep(10000); // 0.01 second
        setKey(keyCode, 1); // Press
        usleep(10000); // 0.01 second
        setKey(keyCode, 0); // Release
    }
    void inputString(std::string in) const
    {
        for (int i = 0; i < in.length(); i++)
            quickPress( char_to_keycode(in.at(i)) );
    }
    void setMousePosition(int screenX,int screenY) const
    {
        sendMouseEvent(EV_ABS, ABS_X, screenX);
        sendMouseEvent(EV_ABS, ABS_Y, screenY);
    }
    void addMousePosition(int screenX,int screenY) const
    {
        sendMouseEvent(EV_REL , REL_Y , screenX);
        sendMouseEvent(EV_REL , REL_X , screenY);
    }
    InputHandler()
    {
        fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
        if (fd < 0)
        {
            std::cerr << "Error opening /dev/uinput: " << strerror(errno) << std::endl;
            exit(-1);
        }

        if (ioctl(fd, UI_SET_EVBIT, EV_KEY) < 0) {
            std::cerr << "Error setting EV_KEY: " << strerror(errno) << std::endl;
            close(fd);
            exit(-1);
        }

        // Enable all keys made available by char to key code
        for(auto mapped_key_code : CHAR_KEYCODE_MAP)
            ioctl(fd, UI_SET_KEYBIT, mapped_key_code.second);

        ioctl(fd, UI_SET_EVBIT, EV_REL); // Enable relative events (for mouse movement)
        ioctl(fd, UI_SET_RELBIT, REL_X); // Enable X-axis movement
        ioctl(fd, UI_SET_RELBIT, REL_Y); // Enable Y-axis movement

        ioctl(fd, UI_SET_EVBIT, EV_ABS); // Enable non relative
        ioctl(fd, UI_SET_RELBIT, EV_ABS); // Enable X-axis movement
        ioctl(fd, UI_SET_RELBIT, EV_ABS); // Enable Y-axis movement



        memset(&uidev, 0, sizeof(uidev));
        snprintf(uidev.name, UINPUT_MAX_NAME_SIZE, "uinput-sample");
        uidev.id.bustype = BUS_USB;
        uidev.id.vendor = 0x1234;
        uidev.id.product = 0x5678;
        uidev.id.version = 1;

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
    ~InputHandler()
    {
        if (ioctl(fd, UI_DEV_DESTROY) < 0) {
            std::cerr << "Error destroying uinput device: " << strerror(errno) << std::endl;
            close(fd);
        }
        close(fd);
    }
};
