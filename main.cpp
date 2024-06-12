#include "automation.h"
int main()
{
    std::cout<<"ensure resolution is set to 1360 * 768\n";
    std::cout<<"the username must be 'one'\n";
    system("xdotool mousemove_relative 1 1");
    std::cout<<"If asked by the OS allow aut sharing\n";
    std::cout<<"enter any key to continue\n";

    std::cin.ignore();


    //start programs
    std::cout<<"starting in 5 secconds";
    usleep(500000);

    auto* aut = new automation();

    //open firefox
    aut->forkAndRunCmd("runuser -u one firefox");

    usleep(4000000);//4s to allow website load
    aut->setMousePosition(700, 125);
    aut->leftClick();


    for(int i = 0; i < 64; i++)//remove url
        aut->inputString("\b\a");

    aut->inputString("https://www.paknsave.co.nz/shop/category/fresh-foods-and-bakery\n");

    usleep(4000000);//4s

    aut->setMousePosition(333, 418); //capcha
    aut->leftClick();

    usleep(4000000); //wait for website load


    //close location dropdown
    aut->setMousePosition(744, 481);
    aut->leftClick();

    aut->copyPage();
    std::cout<<aut->getClipboardContent();

    delete(aut);

    return 0;
}
