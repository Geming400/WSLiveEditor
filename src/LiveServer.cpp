#include "LiveServer.hpp"
#include "ActionInterface.hpp"
#include "ActionResponse.hpp"
#include "actions/Add.hpp"
#include "actions/Remove.hpp"
#include "actions/GetLevelString.hpp"
#include "actions/GetSelectedObjects.hpp"
#include "actions/RemoveSelected.hpp"
#include "actions/GetNextFreeGroups.hpp"
#include "actions/GetNextFreeLinks.hpp"

#include <Geode/Geode.hpp>
#include <Geode/binding/GJGameLevel.hpp>
#include <Geode/binding/LevelEditorLayer.hpp>
#include <ixwebsocket/IXNetSystem.h>
#include <ixwebsocket/IXWebSocket.h>
#include <ixwebsocket/IXWebSocketMessageType.h>
#include <ixwebsocket/IXWebSocketServer.h>
#include <Geode/loader/Log.hpp>
#include <Geode/modify/LevelEditorLayer.hpp>
#include <matjson.hpp>
#include <mutex>



bool LiveServer::init()
{
    if(running)
    {
        return false;
    }
    ix::initNetSystem();

    ws = std::make_unique<ix::WebSocketServer>(1313);


    ws->setOnClientMessageCallback([this](std::shared_ptr<ix::ConnectionState> connectionState, ix::WebSocket & webSocket, const ix::WebSocketMessagePtr & msg)
    {
        using enum ix::WebSocketMessageType;
        switch(msg->type)
        {
        case Message:
            onMessage(msg->str, &webSocket);
            break;
        case Open:
            onConnectionOpen();
            break;
        default: break;
        }
    });

    ws->disablePerMessageDeflate();
    ws->disablePong();
    ws->start();
    ws->listen();
    running = true;
    
    return true;
}

void LiveServer::onMessage(std::string_view message, ix::WebSocket* client)
{
    std::string jsonerror;
    auto json = matjson::parse(message, jsonerror);
    if(!json)
    {
        ActionResponse::make_error("invalid json: " + jsonerror).send(client);
        return;
    }

    switch((*json).type())
    {
        case matjson::Type::Object:
            handleAction((*json).as_object(), client);
            break;
        //TODO mutliple actions
        //case matjson::type::Array:
        default: break;
    }
}

void LiveServer::handleAction(const matjson::Object& action, ix::WebSocket* client)
{
    for(const auto& actionType : actionRunners)
    {
        if(actionType->isType(action) && actionType->isValid(action))
        {
            std::lock_guard lock(actionMutex);
            addActionNoLock(action, actionType.get(), client);
            queuedActions = true;
            return;
        }
    }
    ActionResponse::make_error("action type not found").send(client);
}

void LiveServer::onConnectionOpen()
{
    geode::log::info("connection opened");
}

void LiveServer::stop()
{
    if(!running) return;

    for(const auto& client : ws->getClients())
    {
        client->close(1, "User left the editor");
    }
    
    ws->stop();
    ix::uninitNetSystem();
    running = false;
}

LiveServer::~LiveServer()
{
    stop();
}

void LiveServer::runQueuedActions(LevelEditorLayer* editor)
{
    if(queuedActions)
    {
        std::lock_guard lock(actionMutex);
        for(const auto& action : actions)
        {
            geode::log::info("Running action of type {}", action.runner->type());
            if(action.runner->run(editor, action.object).send(action.client))
            {
                action.checkAndCloseConnection();
            }
        }
        actions.clear();
    }
}


struct LiveServerHooks : geode::Modify<LiveServerHooks, LevelEditorLayer>
{
    struct Fields
    {
        LiveServer server;
    };

    void performQueuedActions(float dt)
    {
        m_fields->server.runQueuedActions(this);
    }

    bool init(GJGameLevel* level, bool idk)
    {
        if(!LevelEditorLayer::init(level, idk)) return false;

        m_fields->server.AddActionRunners<
                AddObjectsAction,
                RemoveObjects,
                RemoveSelectedObjects,
                GetLevelString,
                GetSelectedObjects,
                GetNextFreeGroups,
                GetNextFreeLinks>();
        m_fields->server.init();
        this->schedule(schedule_selector(LiveServerHooks::performQueuedActions), 0.0f);

        return true;
    }
};