// topic_publish.cpp
//
// This is a Paho MQTT C++ client, sample application.
//
// It's an example of how to send messages as an MQTT publisher using the
// C++ asynchronous client interface using a 'topic' object to repeatedly
// send data to the same topic.
//
// The sample demonstrates:
//  - Connecting to an MQTT server/broker
//  - Publishing messages
//  - Use of the 'topic' class
//

/*******************************************************************************
 * Copyright (c) 2019 Frank Pagliughi <fpagliughi@mindspring.com>
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    http://www.eclipse.org/legal/epl-v10.html
 * and the Eclipse Distribution License is available at
 *   http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    Frank Pagliughi - initial implementation and documentation
 *******************************************************************************/

#include <iostream>
#include <cstdlib>
#include <string>
#include <thread>	// For sleep
#include <atomic>
#include <chrono>
#include <cstring>
#include "mqtt/async_client.h"

using namespace std;

const string DFLT_SERVER_ADDRESS { "tcp://localhost:1883" };

const string TOPIC { "test" };
const int QOS = 1;

const char* PAYLOADS[] = {
	"System Temp: 20.0",
	"System Temp: 28.5",
	"System Temp: 35.2",
	"System Temp: 27.0",
	nullptr
};

const auto TIMEOUT = std::chrono::seconds(10);

/////////////////////////////////////////////////////////////////////////////

int main(int argc, char* argv[])
{
	//unsigned int microseconds = 1000000;

	string address = ""; // address of mqtt server
	string topic = ""; // topic to publish to
	
	cout << "###########################################################" << endl;
	cout << "MQTT Temperature Monitoring App" << endl;

	// get user arguments
	if (argc == 3) { 
		address = string(argv[1]);
		topic = argv[2];		
	} 
	else {
		cout << "\nMore Arguments than expected !!!" << endl;
		cout << "Please enter exactly 2 arguments: \"addres\" \"topic\"..." << endl;

		return 0;
	}

	cout << "Initializing for server '" << address << "'..." << endl;
	mqtt::async_client cli(address, "");

	cout << "  ...OK" << endl;

	try {
		cout << "\nConnecting..." << endl;
		cli.connect()->wait();
		cout << "  ...OK" << endl;

		cout << "\nPublishing messages to topic '" << topic << "'..." << endl;

		mqtt::topic top(cli, topic, QOS);
		mqtt::token_ptr tok;

		size_t i = 0;
		while (PAYLOADS[i]) {
			std::this_thread::sleep_for(1s);
			tok = top.publish(PAYLOADS[i++]);
			std::this_thread::sleep_for(3s);
			//usleep(3*microseconds); // sleep for 3seconds
		}
		tok->wait();	// Just wait for the last one to complete.
		cout << "OK" << endl;

		// Disconnect
		cout << "\nDisconnecting..." << endl;
		cli.disconnect()->wait();
		cout << "  ...OK" << endl;
	}
	catch (const mqtt::exception& exc) {
		cerr << exc << endl;
		return 1;
	}

 	return 0;
}

