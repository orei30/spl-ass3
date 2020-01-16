package bgu.spl.net.impl.stomp;

import bgu.spl.net.api.StompMessagingProtocol;

public class StompMessagingProtocolImpl<T> implements StompMessagingProtocol<T> {
    private boolean shouldTerminate = false;
    private int connectionId;
    private Connections<String> connections;
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
                Message retmsg = new Message();
                retmsg.setCommand("CONNECTED");
                retmsg.addHeader("version:1.2");
                user.logIn();
                connections.send(connectionId, retmsg.toString());
            } else{
                if(user.getPassword().equals(passcode)) {
                    if(user.isConnected()) {
                        Message retmsg = new Message();
                        retmsg.setCommand("ERROR");
                        retmsg.addHeader("message:User already logged in");
                        retmsg.setBody("User already logged in");
                        connections.send(connectionId, retmsg.toString());
                    } else {
                        Message retmsg = new Message();
                        retmsg.setCommand("CONNECTED");
                        retmsg.addHeader("version:1.2");
                        user.logIn();
                        user.setConnectionHandlerId(connectionId);
                        connections.send(connectionId, retmsg.toString());
                    }
                } else {
                    Message retmsg = new Message();
                    retmsg.setCommand("ERROR");
                    retmsg.addHeader("message:Wrong password");
                    retmsg.setBody("Wrong password");
                    connections.send(connectionId, retmsg.toString());
                    connections.disconnect(connectionId);
                }
            }
        }
        if(msg.getCommand().equals("SUBSCRIBE")) {
            String genre  = msg.getHeader("destination");
            Integer id = Integer.valueOf(msg.getHeader("id"));
            Integer receiptId = Integer.valueOf(msg.getHeader("receipt"));
            user.addTopic(id, genre);
            connections.addUserToTopic(genre, user);
            Message retmsg = new Message();
            retmsg.setCommand("RECEIPT");
            retmsg.addHeader("receipt-id:" + receiptId);
            connections.send(connectionId, retmsg.toString());

        }
        if(msg.getCommand().equals("UNSUBSCRIBE")) {
            Integer id = Integer.valueOf(msg.getHeader("id"));
            Integer receiptId = Integer.valueOf(msg.getHeader("receipt"));
            connections.deleteUserFromTopic(user.deleteTopicFromUser(id), user);
            Message retmsg = new Message();
            retmsg.setCommand("RECEIPT");
            retmsg.addHeader("receipt-id:" + receiptId);
            connections.send(connectionId, retmsg.toString());
        }
        if(msg.getCommand().equals("SEND")) {
            String topic  = msg.getHeader("destination");
            String messageBody = msg.getBody();
            Message retmsg = new Message();
            retmsg.setCommand("MESSAGE");
            retmsg.addHeader("subscription:"+connectionId);
            retmsg.addHeader("Message-id:"+connections.upMsgId());
            retmsg.addHeader("destination:" + topic);
            retmsg.setBody(messageBody);
            connections.send(topic, retmsg.toString());
        }
        if(msg.getCommand().equals("DISCONNECT")) {
            Integer receiptId = Integer.valueOf(msg.getHeader("receipt"));
            user.logOut();
            Message retmsg = new Message();
            retmsg.setCommand("RECEIPT");
            retmsg.addHeader("receipt-id:" + receiptId);
            connections.send(connectionId, retmsg.toString());
            connections.disconnect(connectionId);
        }
    }

    public boolean shouldTerminate() {
        return shouldTerminate;
    }
}