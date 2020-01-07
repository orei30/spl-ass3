package bgu.spl.net.impl.stomp;

import bgu.spl.net.srv.ConnectionHandler;

import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

public class ConnectionsImpl<T> implements Connections<T> {
    private Map<Integer, ConnectionHandler<T>> connections;
    private Map<String, User> users;
    private Map<String, List<String>> topics;
    private int nextId;

    public ConnectionsImpl() {
        this.connections = new ConcurrentHashMap<>();
        this.users = new HashMap<>();
        this.topics = new HashMap<>();
        nextId = 0;
    }

    public boolean send(int connectionId, T msg) {
        if(connections.containsKey(connectionId)) {
            connections.get(connectionId).send(msg);
            return true;
        }
        return false;
    }

    public void send(String channel, T msg) {
//        for(User user : topics.get(channel))
//            send(user.getId(), msg);
    }

    @Override
    public void disconnect(int connectionId) {
//        connections.remove(connectionId);
//        for(String topic : topics.keySet())
//            topics.get(topic).remove(connectionId);
    }

    @Override
    public int addConnection(ConnectionHandler handler) {
        int id = nextId++;
        connections.put(id, handler);
        return id;
    }

    @Override
    public boolean userConnected(String username) {
        return users.get(username).isConnected();
    }

    @Override
    public User getUser(String username) {
        return users.get(username);
    }

    @Override
    public void addNewUser(User user) {
        users.put(user.getUsername(), user);
    }


//    public User getUserById(int id) {
//        return users.get(id);
//    }
//
//    public User getUserByUsername(String username) {
//        for(int id : users.keySet()) {
//            if(users.get(id).getUsername().equals(username))
//                return users.get(id);
//        }
//        return null;
//    }

}
