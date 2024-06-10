#include "inputHandler.h"
int main()
{
    std::cout<<"enter any key to continue";
    std::cin.ignore();


    //start programs
    auto* input = new InputHandler();
    usleep(100000);

    input->quickPress(KEY_A);
    input->quickPress(KEY_B);
    input->quickPress(KEY_C);


    input->inputString("\nhello, how are you, i am under the water, please help me, here too much raining :(\n");

    input->setMousePosition(0,0);
    usleep(100000);
    input->setMousePosition(600,600);
    usleep(100000);
    input->addMousePosition(100,100);
    usleep(100000);
    input->addMousePosition(100,100);


    delete(input);

    return 0;
}
