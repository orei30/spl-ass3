package bgu.spl.net.impl.stomp;

import bgu.spl.net.srv.ConnectionHandler;

import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

public class ConnectionsImpl<T> implements Connections<T> {
    private Map<Integer, ConnectionHandler> connections;
    private Map<ConnectionHandler, User> users;
    private Map<String, List<User>> topics;
    private int nextId;

    public ConnectionsImpl() {
        this.connections = new ConcurrentHashMap<>();
        this.users = new HashMap<>();
        this.topics = new HashMap<>();
        nextId = 0;
    }

    @Override
    public boolean send(int connectionId, T msg) {
        if(connections.containsKey(connectionId)) {
            connections.get(connectionId).send(msg);
            return true;
        }
        return false;
    }

    @Override
    public void send(String channel, T msg) {
        for(User user : topics.get(channel))
            send(user.getId(), msg);
    }

    @Override
    public void disconnect(int connectionId) {
        connections.remove(connectionId);
        for(String topic : topics.keySet())
            topics.get(topic).remove(connectionId);
    }

    @Override
    public void addConnection(ConnectionHandler handler) {
        connections.put(nextId++, handler);
    }
}
