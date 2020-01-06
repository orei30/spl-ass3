package bgu.spl.net.impl.stomp;

import bgu.spl.net.api.StompMessagingProtocol;

public class StompMessagingProtocolImpl<T> implements StompMessagingProtocol<T> {
    private boolean shouldTerminate = false;
    private int connectionId;
    private Connections connections;

    public void start(int connectionId, Connections<String> connections) {
        this.connectionId = connectionId;
        this.connections = connections;
    }

    public void process(String message) {
        Message msg = new Message(message);
        if(msg.getCommand().equals("CONNECT")) {
            String username = msg.getHeader("login");
            String passcode = msg.getHeader("passcode");
            if(connections)
            if(connections.getUserByUsername(username) == null) {

            }
        }
        if(msg.getCommand().equals("SUBSCRIBE")) {

        }
        if(msg.getCommand().equals("UNSUBSCRIBE")) {

        }
        if(msg.getCommand().equals("SEND")) {

        }
        if(msg.getCommand().equals("DISCONNECT")) {

        }
        if(msg.getCommand().equals(""))
            shouldTerminate = "UNRGISTERED".equals(msg.getCommand());
    }

    public boolean shouldTerminate() {
        return shouldTerminate;
    }
}