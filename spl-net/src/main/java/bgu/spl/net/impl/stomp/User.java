package bgu.spl.net.impl.stomp;

import bgu.spl.net.srv.ConnectionHandler;

import java.util.LinkedList;
import java.util.List;

public class User {
    private int id;
    private String username;
    private String password;
    private List<String> Topics;

    public User(int id, String username, String password, ConnectionHandler handler) {
        this.id = id;
        this.username = username;
        this.password = password;
        this.Topics = new LinkedList<>();
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
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
}
