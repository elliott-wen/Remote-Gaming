package com.sysu.cloudgaming.node.network;

import org.apache.mina.core.service.IoHandlerAdapter;


import org.apache.mina.core.session.IdleStatus;
import org.apache.mina.core.session.IoSession;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.sysu.cloudgaming.node.NodeManager;






public class NodeNetworkHandler extends IoHandlerAdapter{
		private static Logger logger = LoggerFactory.getLogger(NodeNetworkHandler.class);
	 	@Override
	    public void exceptionCaught( IoSession session, Throwable cause ) throws Exception
	    {
	 		logger.warn("Record A Exception:{}, Session will be closed",cause.getMessage());
	 		//cause.printStackTrace();
	 		session.close(true);
	    }
	    @Override
	    public void messageReceived( IoSession session, Object message ) throws Exception
	    {
	    	logger.info("Recv a message from hub in session {}",session.getId());
	    	NodeManager.getNodeManager().onGotHubMessage(session, message);
	    }
	    @Override
	    public void sessionIdle( IoSession session, IdleStatus status ) throws Exception
	    {
	    	logger.info("Timer to send report to the hub");
	    	NodeManager.getNodeManager().onSessionIdle(session);
	    	
	    }
	    @Override
	    public void sessionClosed(IoSession session)
	    {
	    	
	    }
	    
	    @Override 
	    public void sessionCreated(IoSession session)
	    {
	    	
	    }
	   
}