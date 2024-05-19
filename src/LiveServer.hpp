#include <atomic>
#include <ixwebsocket/IXWebSocket.h>
#include <matjson.hpp>
#include <mutex>
#include <vector>
#include <ixwebsocket/IXWebSocketServer.h>
#include <memory>
#include "ActionInterface.hpp"
#include "Geode/binding/LevelEditorLayer.hpp"

class LiveServer
{
private:
    struct Action
    {
        matjson::Object object;
        ActionInterface* runner;
        ix::WebSocket* client;
        void checkAndCloseConnection() const
        {
            if(auto closekey = object.find("close"); closekey != object.end() && closekey->second.is_bool() && closekey->second.as_bool())
            {
                client->close();
            }
        }
    };
    std::unique_ptr<ix::WebSocketServer> ws;
    std::mutex actionMutex;
    std::vector<Action> actions;
    std::vector<std::unique_ptr<ActionInterface>> actionRunners;
    bool running = false;
    std::atomic_bool queuedActions = false;

    void onMessage(std::string_view message, ix::WebSocket* client);
    void onConnectionOpen();
    void onServerCallback(std::shared_ptr<ix::ConnectionState> connectionState, ix::WebSocket& webSocket, const ix::WebSocketMessagePtr& msg);

    inline void addActionNoLock(const matjson::Object& obj, ActionInterface* runner, ix::WebSocket* client)
    {
        actions.emplace_back(obj, runner, client);
    }

public:
    ~LiveServer();

    bool init();
    void stop();
    void runQueuedActions(LevelEditorLayer*);

    void handleAction(const matjson::Object& action, ix::WebSocket* client);
    
    template <typename... T>
    void AddActionRunners()
    {
        (actionRunners.emplace_back(std::make_unique<T>()), ...);
    }
};