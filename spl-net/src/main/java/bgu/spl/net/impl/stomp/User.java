package bgu.spl.net.impl.stomp;

import bgu.spl.net.srv.ConnectionHandler;

import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;

public class User {
    private String username;
    private String password;
    private boolean isConnected;
    private Map<Integer, String> topics;
    private Integer connectionHandlerId;

    public User(String username, String password, Integer connectionHandlerId) {
        this.username = username;
        this.password = password;
        this.connectionHandlerId = connectionHandlerId;
        this.isConnected = true;
        this.topics = new HashMap<>();
    }

    public String getUsername() {
        return username;
    }

    public void setUsername(String username) {
        this.username = username;
    }

    public String getPassword() {
        return password;
    }

    public void setPassword(String password) {
        this.password = password;
    }

    public void addTopic(Integer id, String topic) {
        topics.put(id, topic);
    }

    public String deleteTopicFromUser(Integer id) {
        return topics.remove(id);
    }

    public boolean isConnected() {
        return isConnected;
    }

    public void logIn() {isConnected = true;}

    public void logOut() {isConnected = false;}

    public Integer getConnectionHandlerId() {
        return connectionHandlerId;
    }

    public void setConnectionHandlerId(Integer connectionHandlerId) {
        this.connectionHandlerId = connectionHandlerId;
    }
}
