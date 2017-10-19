#include <thread>
#include <chrono>
#include "server.h"

int main(int argc, char* argv[]) {
	char cwd[1024];
	getcwd(cwd, sizeof(cwd));
	std::string root_dir(cwd);
    	int port = 80;
	std::string adress = "0.0.0.0";

    	unsigned thread = std::thread::hardware_concurrency();


    
        int c;
        while((c = getopt(argc, argv, "r:n:p:")) != -1) {
            switch (c) {
                case 'r':
                    root_dir = optarg;
                    break;
                case 'p':
                    port =  (size_t)std::stoi(optarg);
                    break;
                case 'n':
                    thread = (size_t)std::stoi(optarg);
                    break;
                default:
                    throw std::invalid_argument("Unsupported flag");
            }
        }


	Server server(adress, port, thread, root_dir); //addres, port, sizeof threads, root directory
	server.start();
}
