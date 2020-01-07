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
                user = new User(username, passcode, connectionId);
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
            String genre  = msg.getHeader("destination");
            Integer id = Integer.valueOf(msg.getHeader("id"));
            user.addTopic(id, genre);
            connections.addUserToTopic(genre, user);
            //TODO: send receipt;
        }
        if(msg.getCommand().equals("UNSUBSCRIBE")) {
            Integer id = Integer.valueOf(msg.getHeader("id"));
            connections.deleteUserFromTopic(user.deleteTopicFromUser(id), user);
            //TODO: send receipt;
        }
        if(msg.getCommand().equals("SEND")) {
            String topic  = msg.getHeader("destination");
            String messageBody = msg.getBody();
            //TODO: create message and send it;
//            connections.send(topic);
        }
        if(msg.getCommand().equals("DISCONNECT")) {
            //TODO: disconnect after sending all needed receipts
        }
    }

    public boolean shouldTerminate() {
        return shouldTerminate;
    }
}