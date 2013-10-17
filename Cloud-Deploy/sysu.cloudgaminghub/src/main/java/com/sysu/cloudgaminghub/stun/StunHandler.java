package com.sysu.cloudgaminghub.stun;

import java.net.InetSocketAddress;
import java.net.SocketAddress;

import org.apache.mina.core.buffer.IoBuffer;
import org.apache.mina.core.service.IoHandlerAdapter;
import org.apache.mina.core.session.IoSession;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class StunHandler extends IoHandlerAdapter{
	private static Logger logger = LoggerFactory.getLogger(StunHandler.class);
	 @Override
    public void exceptionCaught( IoSession session, Throwable cause ) throws Exception
	{
		 	logger.warn("Record A Exception:{}, Session will be closed",cause.getMessage());
	 		session.close(true);
	}
	@Override
	public void messageReceived(IoSession session, Object message) throws Exception {
		SocketAddress addr=session.getRemoteAddress();
		if(addr instanceof InetSocketAddress)
		{
			InetSocketAddress address=(InetSocketAddress)(addr);
			byte ipbytes[]=address.getAddress().getAddress();
			if(ipbytes.length!=4)
			{
				logger.info("Nowaday, it only support ipv4");
				return;
			}
			IoBuffer buffer=IoBuffer.allocate(8);
			buffer.setAutoExpand(true);
			buffer.setAutoShrink(true);
			buffer.put(ipbytes);
			buffer.putInt(address.getPort());
			buffer.flip();
			session.write(buffer);
		}
		logger.info("Client {} request stun!",addr);
	}
}
