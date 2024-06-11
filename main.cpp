#include "automation.h"
int main()
{
    std::cout<<"ensure resolution is set to 1360 * 768\n";
    std::cout<<"the username must be 'one'\n";
    system("xdotool mousemove_relative 1 1");
    std::cout<<"If asked by the OS allow input sharing\n";
    std::cout<<"enter any key to continue\n";

    std::cin.ignore();


    //start programs
    std::cout<<"starting in 5 secconds";
    usleep(500000);

    auto* input = new automation();

    //open firefox
    auto pid = fork();
    if(pid == 0)
    {
        system("runuser -u one firefox");
        exit(0);
    }
    usleep(2000000);//5s
    input->setMousePosition(700,125);
    input->leftClick();

    usleep(2000000);//2s
    input->setMousePosition(700,125);
    input->leftClick();

    input->inputString("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");//remove url
    input->inputString("google.com\n");

    delete(input);

    return 0;
}
