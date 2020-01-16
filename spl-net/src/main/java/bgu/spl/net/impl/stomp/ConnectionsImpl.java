package bgu.spl.net.impl.stomp;

import bgu.spl.net.srv.ConnectionHandler;

import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.atomic.AtomicInteger;

public class ConnectionsImpl<T> implements Connections<T> {
    private Map<Integer, ConnectionHandler<T>> connections;
    private Map<String, User> users;//TODO: consider change the keys for id. will affect the isConnected() method.
    private Map<String, List<User>> topics;
    private int nextId;
    private AtomicInteger msgId;

    public ConnectionsImpl() {
        this.connections = new ConcurrentHashMap<>();
        this.users = new HashMap<>();
        this.topics = new HashMap<>();
        nextId = 0;
        msgId= new AtomicInteger(0);
    }

    public int upMsgId() {
        int val;
        do {
            val = msgId.get();
        } while (!msgId.compareAndSet(val, val + 1));
        return msgId.intValue();
    }

    public boolean send(int connectionId, T msg) {
        if(connections.containsKey(connectionId)) {
            connections.get(connectionId).send(msg);
            return true;
        }
        return false;
    }

    public void send(String channel, T msg) {
        if(topics.get(channel)!=null)
            for(User user : topics.get(channel))
                send(user.getConnectionHandlerId(), msg);
    }

    @Override
    public void disconnect(int connectionId) {
        for(String topic : topics.keySet())
            topics.get(topic).removeIf(user -> user.getConnectionHandlerId() == connectionId);
        try {
            connections.get(connectionId).close();
        } catch (IOException e) {
            e.printStackTrace();
        }
        connections.remove(connectionId);
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

    @Override
    public void addUserToTopic(String topic, User user) {
        List<User> topicUsersList = topics.get(topic);
        if(topicUsersList == null) {
            topicUsersList = new ArrayList();
            topics.put(topic, topicUsersList);
        }
        topicUsersList.add(user);
    }

    @Override
    public void deleteUserFromTopic(String topic, User user) {
        topics.get(topic).remove(user);
    }
}
