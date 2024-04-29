#pragma comment(lib , "..\\SimpleDirectMediaLayer\\SDL2main.lib")
#pragma comment(lib , "..\\SimpleDirectMediaLayer\\SDL2.lib")
#pragma comment(lib , "..\\SimpleDirectMediaLayer\\Network\\SDL2_net.lib")

#include"Client.hpp"

std::int32_t main(std::int32_t , char**)
{
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        std::cout << SDL_GetError() << "\n";
        std::system("Pause");
        return(0);
    }
    if(SDLNet_Init() < 0)
    {
        std::cout << SDLNet_GetError() << "\n";
        SDL_Quit();
        std::system("Pause");
        return(0);
    }
    IPaddress LAddress{};
    if(SDLNet_ResolveHost(&LAddress , "localhost" , 4242) < 0)
    {
        std::cout << SDLNet_GetError() << "\n";
        SDLNet_Quit();
        SDL_Quit();
        std::system("Pause");
        return(0);
    }
    TCPsocket LSocket{SDLNet_TCP_Open(&LAddress)};
    if(!LSocket)
    {
   	    std::cout << SDLNet_GetError() << "\n";
        SDLNet_Quit();
        SDL_Quit();
        std::system("Pause");
        return(0);
    }
    while(true)
    {
        std::string LBuffer;
        std::getline(std::cin , LBuffer);
        std::int32_t LBytes{SDLNet_TCP_Send(LSocket , LBuffer.data() , static_cast<std::int32_t>(LBuffer.size()))};
        if(LBytes != LBuffer.size())
        {
   	        std::cout << SDLNet_GetError() << "\n";
            SDLNet_TCP_Close(LSocket);
            SDLNet_Quit();
            SDL_Quit();
            std::system("Pause");
            return(0);
        }
    }
    SDLNet_TCP_Close(LSocket);
    SDLNet_Quit();
    SDL_Quit();
    std::system("Pause");
    return(0);
}