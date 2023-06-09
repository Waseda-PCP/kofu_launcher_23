# pragma once
# include <Siv3D.hpp>

class TimerServer {
	mutable TCPServer server;
	std::map<size_t, long double> time_lefts;
	std::map<size_t, size_t> pc_ids;
	static constexpr uint16 port = 9080;
public:
	size_t defaultTime = 300;

	void update() {
		auto deltaTime = Scene::DeltaTime();
		auto sessionIds = server.getSessionIDs();
		std::vector<size_t> deletelist;
		for (auto& id : time_lefts) {
			if (!sessionIds.count(id.first)) {
				deletelist.push_back(id.first);
			}
			else {
				id.second -= deltaTime;
			}
		}

		for (auto& list : deletelist) {
			time_lefts.erase(list);
		}

		for (auto& id : sessionIds) {
			if (size_t read; server.read(read)) {
				
			}
			if (!time_lefts.count(id)) {
				time_lefts[id] = defaultTime;
			}
		}

		for (auto id : server.getSessionIDs()) {
			server.send(time_lefts[id], id);
			size_t pc_id;
			if (server.read(pc_id, id)) {
				pc_ids[id] = pc_id;
			}
		}
	}

	void connect() const {
		if (server.isAccepting()) {
			throw std::exception("Client is already connected.");
		}
		server.startAcceptMulti(port);
	}

	void reconnect() const {
		server.disconnect();
		server.startAcceptMulti(port);
	}

	std::map<size_t, long double> getTimeLefts() {
		return time_lefts;
	}

	bool setTime(size_t index, long double value) {
		if (time_lefts.count(index)) {
			time_lefts[index] = value;
			return true;
		}
		return false;
	}

	size_t getPCId(size_t sessionId) {
		if (pc_ids.count(sessionId)) {
			return pc_ids[sessionId];
		}
		else {
			return -1;
		}
	}
};
