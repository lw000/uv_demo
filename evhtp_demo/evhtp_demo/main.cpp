#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <string>

#include "server_config.hpp"

#include "example.h"

#include <log4z/log4z.h>
using namespace zsummer::log4z;

int main(int argc, char ** argv) {
//    ILog4zManager::getInstance()->start();

	if (argc < 2) {
		LOGE("(-c config.cfg)");
		return 0;
	}

	std::string conf;
	int ch = 0;
	while ((ch = getopt(argc, argv, "c:")) != -1) {
		switch (ch) {
		case 'c': {
			if (optarg != NULL) {
				conf = optarg;
			}
			break;
		}
		default: {
			LOGE("(-c config.cfg)");
			break;
		}
		}
	}

	if (conf.empty()) {
		LOGE("config file is empty");
		return 0;
	}

	do {
        std::map<std::string, std::string> config = load_config(conf);
        if (config.empty()) {
            break;
        }
        
        std::string s = config["platform"];
        std::string host = config["host"];
        std::string port = config["port"];

		if (s.compare("s") == 0) {
			main_server(atoi(port.c_str()));
		} else if (s.compare("c") == 0) {
            main_client(host.c_str(), atoi(port.c_str()));
		} else {
			printf("error. \n");
		}

		while (1) {
			sleep(1);
		}

	} while (0);

	return 0;
}
