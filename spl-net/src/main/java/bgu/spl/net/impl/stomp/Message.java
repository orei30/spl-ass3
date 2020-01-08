package bgu.spl.net.impl.stomp;

import java.util.HashMap;
import java.util.Map;

public class Message {

    private String command;
    private Map<String, String> headers;
    private String body;

    public Message() {
        headers = new HashMap<>();
        body = "";
    }

    public Message(String msg) {
        System.out.println("message: " + msg);
        String[] detailes = msg.split("\n");
        command = detailes[0];
        headers = new HashMap<>();
        int i = 1;
        while (!detailes[i].equals("")) {
            addHeader(detailes[i]);
            i++;
        }
        i++;
        while (!detailes[i].equals("^@")) {
            body = body + detailes[i];
            i++;
        }
    }

    public String getCommand() {
        return command;
    }

    public void setCommand(String command) {
        this.command = command;
    }

    public void addHeader(String header) {
        int colonIndex = header.indexOf(':');
        String name = header.substring(0, colonIndex);
        String value = header.substring(colonIndex + 1);
        headers.put(name, value);
    }

    public String getHeader(String title) {
        return headers.get(title);
    }

    public String getBody() {
        return body;
    }

    public void setBody(String body) {
        this.body = body;
    }

    public String toString() {
        String headersString = "";
        for (Map.Entry<String, String> header : headers.entrySet())
            headersString = header.getKey() + ":" + header.getValue() + "\n";
        if(getBody() == "")
            return command + "\n" + headersString + "\n^@";
        else
            return command + "\n" + headersString + "\n" + getBody() + "\n^@";
    }
}
