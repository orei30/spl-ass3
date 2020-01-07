package bgu.spl.net.impl.stomp;

import bgu.spl.net.api.StompMessagingProtocol;

public class StompMessagingProtocolImpl<T> implements StompMessagingProtocol<T> {
    private boolean shouldTerminate = false;
    private int connectionId;
    private Connections connections;
    private User user;

    public void start(int connectionId, Connections<String> connections) {
        this.connectionId = connectionId;
        this.connections = connections;
    }

    public void process(String message) {
        Message msg = new Message(message);
        if(msg.getCommand().equals("CONNECT")) {
            String username = msg.getHeader("login");
            String passcode = msg.getHeader("passcode");
            user = connections.getUser(username);
            if(user == null) {
                user = new User(username, passcode);
                connections.addNewUser(user);
            } else{
                if(user.getPassword().equals(passcode))
                    //TODO: send connected frame
                    user.logIn();
                else
                    //TODO: wrong passcode frame
                    connections.disconnect(connectionId);
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
    }

    public boolean shouldTerminate() {
        return shouldTerminate;
    }
}