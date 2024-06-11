#include "inputHandler.h"
int main()
{
    std::cout<<"ensure resolution is set to 1360 * 768\n";
    system("xdotool mousemove_relative 1 1");
    std::cout<<"If asked by the OS allow input sharing\n";
    std::cout<<"enter any key to continue\n";

    std::cin.ignore();


    //start programs
    std::cout<<"starting in 5 secconds";
    usleep(500000);

    auto* input = new InputHandler();

    system("sudo firefox");
    usleep(200000);



    input->quickPress(KEY_A);
    input->quickPress(KEY_B);
    input->quickPress(KEY_C);


    input->inputString("\nhello, how are you, i am under the water, please help me, here too much raining :(\n");



    input->setMousePosition(500,500);


    delete(input);

    return 0;
}
