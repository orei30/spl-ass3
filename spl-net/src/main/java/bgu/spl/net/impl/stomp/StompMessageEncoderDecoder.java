package bgu.spl.net.impl.stomp;

import bgu.spl.net.api.MessageEncoderDecoder;

public interface StompMessageEncoderDecoder extends MessageEncoderDecoder<String> {
    @Override
    String decodeNextByte(byte nextByte);

    @Override
    byte[] encode(String message);
}
