package bgu.spl.net.impl.stomp;

import bgu.spl.net.srv.ConnectionHandler;

public interface Connections<T> {
    boolean send(int connectionId, T msg);

    void send(String channel, T msg);

    void disconnect(int connectionId);

    int addConnection(ConnectionHandler handler);

    boolean userConnected(String username);

    User getUser(String username);

    void addNewUser(User user);

    void addUserToTopic(String topic, User user);

    void deleteUserFromTopic(String topic, User user);
}
