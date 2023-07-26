# pragma once
# include <Siv3D.hpp>

class TimerClient {
	long double time_left;
	IPv4Address ip;
	size_t pc_id;
	TCPClient client;
	uint16 port;
public:

	TimerClient() {

	}

	size_t getTimeLeft()  {
		return time_left;
	}

	IPv4Address getIp()  {
		return ip;
	}

	size_t getPort()  {
		return port;
	}

	void update() {
		if (!client.isConnected()) {
			throw std::exception("Client not connected.");
		}
		client.send(pc_id);
		while (client.read(time_left));
	}

	void connect(size_t pcId) {
		if (client.isConnected()) {
			throw std::exception("Client is already connected.");
		}
		pc_id = pcId;
		client.connect(ip, port);
	}

	void reconnect()  {
		client.disconnect();
		client.connect(ip, port);
	}

	TimerClient(const IPv4Address& ip, const uint16& port)
		: ip(ip), port(port)
	{
		time_left = SIZE_MAX;
	}
};
