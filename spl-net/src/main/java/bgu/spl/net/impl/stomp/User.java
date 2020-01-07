package bgu.spl.net.impl.stomp;

import bgu.spl.net.srv.ConnectionHandler;

import java.util.LinkedList;
import java.util.List;

public class User {
    private String username;
    private String password;
    private boolean isConnected;
    private List<String> Topics;

    public User(String username, String password) {
        this.username = username;
        this.password = password;
        this.isConnected = true;
        this.Topics = new LinkedList<>();
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

    public void addTopic(String topic) {
        Topics.add(topic);
    }

    public boolean isConnected() {
        return isConnected;
    }

    public void logIn() {isConnected = true;}

    public void logOut() {isConnected = false;}
}
