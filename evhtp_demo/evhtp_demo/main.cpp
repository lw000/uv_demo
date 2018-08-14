#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <string>

#include "example.h"

#include <log4z/log4z.h>
using namespace zsummer::log4z;

#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <rapidjson/reader.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <rapidjson/filereadstream.h>

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
		FILE *f = NULL;
		f = fopen(conf.c_str(), "r");
		fseek(f, 0, SEEK_SET);
        char buff[2048];
        rapidjson::Document root;
        rapidjson::FileReadStream is(f, buff, sizeof(buff));
        root.ParseStream<0>(is);
        if (root.HasParseError()) {
            LOGE("fail parse json.");
            fclose(f);
            break;
        }
		fclose(f);

		std::string s = root["platform"].GetString();
		std::string host = root["host"].GetString();
		int port = root["port"].GetInt();

		if (s.compare("s") == 0) {
			main_server(port);
		} else if (s.compare("c") == 0) {
			main_client(host.c_str(), port);
		} else {
			printf("error. \n");
		}

		while (1) {
			sleep(1);
		}

	} while (0);

	return 0;
}
