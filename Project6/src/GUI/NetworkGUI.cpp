#include "NetworkGUI.h"

#include "../BACKEND/Network.h"
#include "../BACKEND/NetworkHandler.h"

#include <string>
#include <vector>

std::vector<unsigned long> networkString() {
    std::vector<unsigned long> network_str;

    NetworkHandler network_handler = NetworkHandler();
    Network net = network_handler.get_network();

    network_str.push_back(net.get_upload_speed());
    network_str.push_back(net.get_download_speed());

    /*
    network_str = "Network:       Upload Speed: " + std::to_string(net.get_upload_speed())
                + ", Download Speed: " + std::to_string(net.get_download_speed());
    */
    return network_str;
}