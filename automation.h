//=======================================================================================
//
//  THIS IS ONLY INTENDED TO BE USED ON LINUX SYSTEMS, OR VIRTUAL MACHINES
//  THE MOUSE FUNCTIONALITY DOES NOT WORK WITHOUT INSTALLING xdotool
//
//  MUST BE RUN WITH SUDO
//
//  ! DEPENDENCIES !
//   xdotool
//   xsel
//=======================================================================================
#include <linux/uinput.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <cerrno>
//#include "CharToKeyCode.h"
#include <cstdlib>
#include <memory>
#include <array>
#include <unordered_map>

#pragma region char_to_keycode
const std::unordered_map<char, int> CHAR_KEYCODE_MAP =
        {       // STANDARD KEYS ========================================
                {'a',  KEY_A},{'b',  KEY_B},{'c',  KEY_C},{'d',  KEY_D},{'e',  KEY_E},{'f',  KEY_F},{'g',  KEY_G},{'h',  KEY_H},{'i',  KEY_I},{'j',  KEY_J},{'k',  KEY_K},{'l',  KEY_L},{'m',  KEY_M},{'n',  KEY_N},{'o',  KEY_O},{'p',  KEY_P},{'q',  KEY_Q},{'r',  KEY_R},{'s',  KEY_S},{'t',  KEY_T},
                {'u',  KEY_U},{'v',  KEY_V},{'w',  KEY_W},{'x',  KEY_X},{'y',  KEY_Y},{'z',  KEY_Z},{'0',  KEY_0},{'1',  KEY_1},{'2',  KEY_2},{'3',  KEY_3},{'4',  KEY_4},{'5',  KEY_5},{'6',  KEY_6},{'7',  KEY_7},{'8',  KEY_8},{'9',  KEY_9},{' ',  KEY_SPACE},{'\n', KEY_ENTER},{'!',  KEY_1},{'@',  KEY_2},
                {'#',  KEY_3},{'$',  KEY_4},{'%',  KEY_5},{'^',  KEY_6},{'&',  KEY_7},{'*',  KEY_8},{'(',  KEY_9},{')',  KEY_0},{'-',  KEY_MINUS},{'_',  KEY_MINUS},{'=',  KEY_EQUAL},{'+',  KEY_EQUAL},{'[',  KEY_LEFTBRACE},{']',  KEY_RIGHTBRACE},{'{',  KEY_LEFTBRACE},{'}',  KEY_RIGHTBRACE},{';',  KEY_SEMICOLON},{':',  KEY_SEMICOLON},{'\'', KEY_APOSTROPHE},{'"',  KEY_APOSTROPHE},
                {'`',  KEY_GRAVE},{'~',  KEY_GRAVE},{'\\', KEY_BACKSLASH},{'|',  KEY_BACKSLASH},{',',  KEY_COMMA},{'<',  KEY_COMMA},{'.',  KEY_DOT},{'>',  KEY_DOT},{'/',  KEY_SLASH},{'?',  KEY_SLASH},
                // SPECIAL KEYS =======================================
                {'\t', KEY_LEFTCTRL},
                {'\b', KEY_BACKSPACE},
                {'\a', KEY_DELETE},
                {'\r',KEY_LEFTMETA},
                {'\f',KEY_UP},
                {'\v',KEY_LEFTSHIFT}
        };

inline int char_to_keycode(const char in) //gives the keycode equivalent of the character
{
    auto it = CHAR_KEYCODE_MAP.find(in);
    if (it != CHAR_KEYCODE_MAP.end())
        return it->second;
    else
        throw std::out_of_range("Character not found in keycode map");

}
#pragma endregion

#define POST_INPUT_DELAY() usleep(500); //.001s delay

class automation
{
    int fd;
    uinput_user_dev uidev{};

public:
    void setKey(int keycode, int value) const
    {
        input_event ev = {};
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
    inline void quickPress(int keyCode) const
    {
        setKey(keyCode, 1); // Press
        POST_INPUT_DELAY()
        setKey(keyCode, 0); // Release
        POST_INPUT_DELAY()
    }
    inline void inputString(const std::string& in) const
    {
        for (int i = 0; i < in.length(); i++)
            quickPress( char_to_keycode(in.at(i)) );
    }
    inline void leftClick() const
    {
        system("xdotool click 1");
        POST_INPUT_DELAY()
    }
    inline void setMousePosition(int x = 0, int y = 0 ) const
    {
        system(("xdotool mousemove " + std::to_string(x) + " " + std::to_string(y)).c_str());
        POST_INPUT_DELAY()
    }
    inline void changeMousePosition(int x = 0, int y = 0) const
    {
        system(  ("xdotool mousemove_relative " + std::to_string(x) + " " + std::to_string(y)).c_str());
        POST_INPUT_DELAY()
    }
    static void forkAndRunCmd(const std::string& in)
    {
        auto pid = fork();
        if(pid == 0)
        {
            system(in.c_str());
            exit(0);
        }
    }
    inline void copyPage() const
    {
        setKey(KEY_LEFTCTRL,1);
        quickPress(KEY_A);
        quickPress(KEY_C);
        setKey(KEY_LEFTCTRL,0);
    }

    static std::string getClipboardContent()
    {
        std::string command = "xsel --output --clipboard";
        FILE* pipe = popen(command.c_str(), "r");

        std::string clipboardContent;
        char buffer[2048];
        while (fgets(buffer, 2048, pipe) != nullptr) {
            clipboardContent += buffer;
        }
        pclose(pipe);
        return clipboardContent;
    }

    automation()
    {
        setenv("XDG_RUNTIME_DIR", "/run/user/1000", 0);
        setenv("DISPLAY", ":0", 0);



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

