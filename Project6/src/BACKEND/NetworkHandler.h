// NetworkHandler.h
#ifndef NETWORK_HADNLER_H
#define NETWORK_HANDLER_H

#include "Network.h"

class NetworkHandler {
	private:
		Network net;
	public:
		NetworkHandler();

		Network get_network() {
			return this->net;
		}

		void update();
};
#endif
