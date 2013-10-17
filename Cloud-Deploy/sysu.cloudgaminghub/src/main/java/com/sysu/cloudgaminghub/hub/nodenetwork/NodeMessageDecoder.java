package com.sysu.cloudgaminghub.hub.nodenetwork;

import org.apache.mina.core.buffer.IoBuffer;

import org.apache.mina.core.session.IoSession;
import org.apache.mina.filter.codec.CumulativeProtocolDecoder;
import org.apache.mina.filter.codec.ProtocolDecoderOutput;


public class NodeMessageDecoder extends CumulativeProtocolDecoder{

	@Override
	protected boolean doDecode(IoSession arg0, IoBuffer arg1,
			ProtocolDecoderOutput arg2) throws Exception {
		if(arg1.remaining()>=8)
		{
			
			arg1.mark();
			NodeMessage message=new NodeMessage();
			int type=arg1.getInt();
			int length=arg1.getInt();
			if(length!=0)
			{
				if(arg1.remaining()>=length)
				{
					message.setMessageType(type);
					message.setMessageLength(length);
					byte extendedData[]=new byte[length];
					arg1.get(extendedData);
					message.setExtendedData(extendedData);
					arg2.write(message);
					return true;
				}
				else
				{
					arg1.reset();
					return false;
				}
			}
			else
			{
				message.setMessageType(type);
				message.setMessageLength(length);
				arg2.write(message);
				return true;
			}
			
		}
		return false;
	}
	

}
