#include "inputHandler.h"
int main()
{
    std::cout<<"please maximize the window then press any key to continue";
    char a[32];
    std::cin>> a;

    std::cout<<"char input : keycode : actual char" << "\n";

    std::cout<<(int)'a'<<":"<<KEY_A<<":a"<<"\n";
    std::cout<<(int)'z'<<":"<<KEY_Z<<":z"<<"\n";
    std::cout<<(int)'0'<<":"<<KEY_0<<":0"<<"\n";
    std::cout<<(int)'1'<<":"<<KEY_1<<":1"<<"\n";
    std::cout<<(int)'9'<<":"<<KEY_9<<":9"<<"\n";
    std::cout<<(int)'.'<<":"<<KEY_STOP<<":."<<"\n";

    //start program

    auto* ih = new InputHandler();
    usleep(100000);

    ih->quickPress(KEY_A);
    ih->quickPress(KEY_B);
    ih->quickPress(KEY_C);
    ih->quickPress(KEY_D);

    free(ih);
    return 0;
}
