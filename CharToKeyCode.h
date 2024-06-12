//
// Created by one on 10/06/24.
//


#include <unordered_map>
#include <linux/input-event-codes.h>

    const std::unordered_map<char, int> CHAR_KEYCODE_MAP =
            {
                    {'a',  KEY_A},
                    {'b',  KEY_B},
                    {'c',  KEY_C},
                    {'d',  KEY_D},
                    {'e',  KEY_E},
                    {'f',  KEY_F},
                    {'g',  KEY_G},
                    {'h',  KEY_H},
                    {'i',  KEY_I},
                    {'j',  KEY_J},
                    {'k',  KEY_K},
                    {'l',  KEY_L},
                    {'m',  KEY_M},
                    {'n',  KEY_N},
                    {'o',  KEY_O},
                    {'p',  KEY_P},
                    {'q',  KEY_Q},
                    {'r',  KEY_R},
                    {'s',  KEY_S},
                    {'t',  KEY_T},
                    {'u',  KEY_U},
                    {'v',  KEY_V},
                    {'w',  KEY_W},
                    {'x',  KEY_X},
                    {'y',  KEY_Y},
                    {'z',  KEY_Z},
                    {'0',  KEY_0},
                    {'1',  KEY_1},
                    {'2',  KEY_2},
                    {'3',  KEY_3},
                    {'4',  KEY_4},
                    {'5',  KEY_5},
                    {'6',  KEY_6},
                    {'7',  KEY_7},
                    {'8',  KEY_8},
                    {'9',  KEY_9},
                    {' ',  KEY_SPACE},
                    {'\n', KEY_ENTER},
                    {'!',  KEY_1},
                    {'@',  KEY_2},
                    {'#',  KEY_3},
                    {'$',  KEY_4},
                    {'%',  KEY_5},
                    {'^',  KEY_6},
                    {'&',  KEY_7},
                    {'*',  KEY_8},
                    {'(',  KEY_9},
                    {')',  KEY_0},
                    {'-',  KEY_MINUS},
                    {'_',  KEY_MINUS},
                    {'=',  KEY_EQUAL},
                    {'+',  KEY_EQUAL},
                    {'[',  KEY_LEFTBRACE},
                    {']',  KEY_RIGHTBRACE},
                    {'{',  KEY_LEFTBRACE},
                    {'}',  KEY_RIGHTBRACE},
                    {';',  KEY_SEMICOLON},
                    {':',  KEY_SEMICOLON},
                    {'\'', KEY_APOSTROPHE},
                    {'"',  KEY_APOSTROPHE},
                    {'`',  KEY_GRAVE},
                    {'~',  KEY_GRAVE},
                    {'\\', KEY_BACKSLASH},
                    {'|',  KEY_BACKSLASH},
                    {',',  KEY_COMMA},
                    {'<',  KEY_COMMA},
                    {'.',  KEY_DOT},
                    {'>',  KEY_DOT},
                    {'/',  KEY_SLASH},
                    {'?',  KEY_SLASH},
                    // SPECIAL KEYS =======================================
                    {'\t', KEY_LEFTCTRL},
                    {'\b', KEY_BACKSPACE},
                    {'\a', KEY_DELETE}
            };

    int char_to_keycode(const char in) //gives the keycode equivalent of the character
    {
        auto it = CHAR_KEYCODE_MAP.find(in);
        if (it != CHAR_KEYCODE_MAP.end()) {
            return it->second;
        } else
        {
            std::cout<<"char :" << in << " not found";
            throw std::out_of_range("Character not found in keycode map");
        }
    }
