#include <linux/uinput.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <cerrno>

class InputHandler
{
    bool debug = true;
    int fd;
    uinput_user_dev uidev{};
public:
    void setKey(int keycode, int value)
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

    void quickPress(int keyCode)
    {
        usleep(100000); // 0.1 second
        setKey(keyCode, 1); // Press
        usleep(100000); // 0.1 second
        setKey(keyCode, 0); // Release
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

        // Enable KEY_A, KEY_B, KEY_C, and KEY_D
        if (ioctl(fd, UI_SET_KEYBIT, KEY_A) < 0) {
            std::cerr << "Error setting KEY_A: " << strerror(errno) << std::endl;
            close(fd);
            exit(-1);
        }
        if (ioctl(fd, UI_SET_KEYBIT, KEY_B) < 0) {
            std::cerr << "Error setting KEY_B: " << strerror(errno) << std::endl;
            close(fd);
            exit(-1);
        }
        if (ioctl(fd, UI_SET_KEYBIT, KEY_C) < 0) {
            std::cerr << "Error setting KEY_C: " << strerror(errno) << std::endl;
            close(fd);
            exit(-1);
        }
        if (ioctl(fd, UI_SET_KEYBIT, KEY_D) < 0) {
            std::cerr << "Error setting KEY_D: " << strerror(errno) << std::endl;
            close(fd);
            exit(-1);
        }

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
