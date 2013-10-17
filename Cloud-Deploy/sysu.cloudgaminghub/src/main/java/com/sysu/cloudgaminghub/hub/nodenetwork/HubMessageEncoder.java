package com.sysu.cloudgaminghub.hub.nodenetwork;

import org.apache.mina.core.buffer.IoBuffer;
import org.apache.mina.core.session.IoSession;
import org.apache.mina.filter.codec.ProtocolEncoder;
import org.apache.mina.filter.codec.ProtocolEncoderOutput;

public class HubMessageEncoder implements ProtocolEncoder{

	public void dispose(IoSession arg0) throws Exception {
		// TODO Auto-generated method stub
		
	}

	public void encode(IoSession arg0, Object arg1, ProtocolEncoderOutput arg2)
			throws Exception {
		HubMessage message=(HubMessage)(arg1);
		IoBuffer buffer=IoBuffer.allocate(8, false);
		buffer.setAutoExpand(true);
		buffer.putInt(message.getMessageType());
		buffer.putInt(message.getMessageLength());
		if(message.getMessageLength()!=0)
		{
			buffer.put(message.getExtendedData());
		}
		buffer.flip();
		arg2.write(buffer);
		
	}

}
