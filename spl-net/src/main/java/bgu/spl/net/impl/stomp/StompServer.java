package bgu.spl.net.impl.stomp;

import bgu.spl.net.api.MessageEncoderDecoder;
import bgu.spl.net.api.MessagingProtocol;
import bgu.spl.net.srv.Server;

import java.util.function.Supplier;

public class StompServer {
    public static void main(String[] args) {
        int port = Integer.parseInt(args[0]);
        String server = args[1];
        if(server.equals("tcp")) Server.threadPerClient(port,
                ()->new StompMessagingProtocolImpl<String>(),
                StompMessageEncoderDecoderImpl::new).serve();
        if(server.equals("reactor")) Server.reactor(Runtime.getRuntime().availableProcessors(), port,
                ()-> new StompMessagingProtocolImpl<String>(),
                StompMessageEncoderDecoderImpl::new).serve();
    }
}
