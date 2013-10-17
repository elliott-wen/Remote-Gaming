package com.sysu.cloudgaminghub.hub.nodenetwork;

import java.net.InetSocketAddress;


import org.apache.mina.core.service.IoAcceptor;
import org.apache.mina.core.session.IdleStatus;
import org.apache.mina.filter.codec.ProtocolCodecFilter;
import org.apache.mina.transport.socket.nio.NioSocketAcceptor;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.sysu.cloudgaminghub.config.Config;

public class NodeNetwork {
	
	private IoAcceptor acceptor;
	private static Logger logger = LoggerFactory.getLogger(NodeNetwork.class);
	public boolean setupNodeNetwork()
	{
		acceptor = new NioSocketAcceptor();
		acceptor.getSessionConfig().setReadBufferSize( 2048 );
        acceptor.getSessionConfig().setIdleTime( IdleStatus.BOTH_IDLE, 10 );
        acceptor.setHandler(  new NodeNetworkHandler() );
        acceptor.getFilterChain().addLast("protocol", new ProtocolCodecFilter(new NodeNetworkCodecFactory()));
        try
        {
        	 
        	 acceptor.bind( new InetSocketAddress(Config.HUBPORT) );
        	 logger.info("Instance Hub Network Ready!");
        	 return true;
        }
        catch(Exception ex)
        {
        	logger.warn(ex.getMessage(),ex);
        	return false;
        }
	}
	
}
