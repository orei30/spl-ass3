package bgu.spl.net.impl.stomp;

import bgu.spl.net.srv.ConnectionHandler;

import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

public class ConnectionImpl<T> implements Connections<T> {
    private Map<Integer, ConnectionHandler> connections;
    private int nextId;

    public ConnectionImpl() {
        this.connections = new ConcurrentHashMap<>();
        nextId = 0;
    }

    @Override
    public boolean send(int connectionId, T msg) {
        return false;
    }

    @Override
    public void send(String channel, T msg) {

    }

    @Override
    public void disconnect(int connectionId) {

    }

    @Override
    public void addConnection(ConnectionHandler handler) {
        
    }
}
