#include "automation.h"
#include "dataParser.h"
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

    usleep(500000);//4s to allow website load

    //ensure window is maximized
    aut->setMousePosition(724,406);
    aut->leftClick();

    aut->setKey(KEY_LEFTMETA ,1);
    aut->quickPress(KEY_UP);
    aut->setKey(KEY_LEFTMETA ,0);

    usleep(5000000);//4s to allow website load

    //select url bar
    aut->setMousePosition(700, 125);
    aut->leftClick();

    //remove url
    for(int i = 0; i < 128; i++)
        aut->inputString("\b\a");

    //enter url
    aut->inputString("https;//www.paknsave.co.nz/shop/category/fresh-foods-and-bakery");

    //ensure correct input
    usleep(5000);
    aut->copyPage();
    if(aut->getClipboardContent() != "https;//www.paknsave.co.nz/shop/category/fresh-foods-and-bakery")
    {
        std::cout<<"\n\n FAILED!, incorrect context detected";
        exit(-1);
    }

    //confirm search
    aut->inputString("\n");

    usleep(5000000);//4s

    //capcha
    aut->setMousePosition(333, 418);
    aut->leftClick();


    usleep(2000000);

    parser parse = parser();

    //go through all pages
    for(int page = 1; page < 3; page++)
    {

        aut->setMousePosition(700, 125);
        aut->leftClick();

        for(int i = 0; i < 64; i++)
            aut->inputString("\b\a");

        //search with page number
        aut->inputString("https://www.paknsave.co.nz/shop/category/fresh-foods-and-bakery");

        //enter question mark
        aut->setKey(KEY_LEFTSHIFT,1);
        aut->inputString("/");
        aut->setKey(KEY_LEFTSHIFT,0);

        //finnish url
        aut->inputString("pg=" + std::to_string(page));

        //search
        aut->inputString("\n");

        usleep(3000000);// wait for site to load
        aut->copyPage();
        parse.parse( aut->getClipboardContent() );
        usleep(500000);//0.5s
    }
    delete(aut);

    return 0;
}
