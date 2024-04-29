#pragma comment(lib , "..\\SimpleDirectMediaLayer\\SDL2main.lib")
#pragma comment(lib , "..\\SimpleDirectMediaLayer\\SDL2.lib")
#pragma comment(lib , "..\\SimpleDirectMediaLayer\\Network\\SDL2_net.lib")

#include"Server.hpp"

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
    if(SDLNet_ResolveHost(&LAddress , nullptr , 4242) < 0)
    {
        std::cout << SDLNet_GetError() << "\n";
        SDLNet_Quit();
        SDL_Quit();
        std::system("Pause");
        return(0);
    }
    TCPsocket LServer{SDLNet_TCP_Open(&LAddress)};
    if(!LServer)
    {
   	    std::cout << SDLNet_GetError() << "\n";
        SDLNet_Quit();
        SDL_Quit();
        std::system("Pause");
        return(0);
    }
    std::array<TCPsocket , 2> LClients{};
    std::array<SDLNet_SocketSet , LClients.size()> LSets{};
    for(std::intmax_t LClient{0} ; LClient < static_cast<std::intmax_t>(LClients.size()) ; LClient++)
    {
        LSets[LClient] = SDLNet_AllocSocketSet(1);
        if(!LSets[LClient])
        {
            std::cout << SDLNet_GetError() << "\n";
            SDLNet_TCP_Close(LServer);
            SDLNet_Quit();
            SDL_Quit();
            std::system("Pause");
            return(0);
        }
        while(!LClients[LClient])
        {
   	        LClients[LClient] = SDLNet_TCP_Accept(LServer);
            SDLNet_TCP_AddSocket(LSets[LClient] , LClients[LClient]);
        }
    }
    while(true)
    {
        for(std::intmax_t LClient{0} ; LClient < static_cast<std::intmax_t>(LClients.size()) ; LClient++)
        {
            if(SDLNet_CheckSockets(LSets[LClient] , 0))
            {
                std::string LBuffer;
                LBuffer.resize(1024);
                std::int32_t LBytes{SDLNet_TCP_Recv(LClients[LClient] , LBuffer.data() , static_cast<std::int32_t>(LBuffer.size()))};
                if(LBytes <= 0)
                {
                    std::cout << SDLNet_GetError() << "\n";
                    for(std::intmax_t LClient{0} ; LClient < static_cast<std::intmax_t>(LClients.size()) ; LClient++)
                    {
                        SDLNet_TCP_DelSocket(LSets[LClient] , LClients[LClient]);
                        SDLNet_TCP_Close(LClients[LClient]);
                        SDLNet_FreeSocketSet(LSets[LClient]);
                    }
                    SDLNet_TCP_Close(LServer);
                    SDLNet_Quit();
                    SDL_Quit();
                    std::system("Pause");
                    return(0);
                }
                LBuffer.resize(LBytes);
                std::cout << LClient << ": " << LBuffer << "\n";
            }
        }
    }
    for(std::intmax_t LClient{0} ; LClient < static_cast<std::intmax_t>(LClients.size()) ; LClient++)
    {
        SDLNet_TCP_DelSocket(LSets[LClient] , LClients[LClient]);
        SDLNet_TCP_Close(LClients[LClient]);
        SDLNet_FreeSocketSet(LSets[LClient]);
    }
    SDLNet_TCP_Close(LServer);
    SDLNet_Quit();
    SDL_Quit();
    std::system("Pause");
    return(0);
}