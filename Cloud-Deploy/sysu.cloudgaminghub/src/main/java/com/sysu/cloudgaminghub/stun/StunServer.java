package com.sysu.cloudgaminghub.stun;

import java.net.InetSocketAddress;

import org.apache.mina.transport.socket.nio.NioDatagramAcceptor;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.sysu.cloudgaminghub.config.Config;



public class StunServer {
	private static Logger logger = LoggerFactory.getLogger(StunServer.class);
	NioDatagramAcceptor acceptor=null;
	public boolean startStunServer()
	{
		acceptor = new NioDatagramAcceptor();
		acceptor.setHandler(new StunHandler());
		acceptor.getSessionConfig().setReuseAddress(true);
		try
		{
			acceptor.bind(new InetSocketAddress(Config.STUNPORT));
			logger.info("Stun Server Started");
			return true;
		}
		catch(Exception e)
		{
			logger.warn("Could not start stun server",e);
			return false;
		}
	}
}
