package com.sysu.cloudgaminghub.hub.nodenetwork;

import org.apache.mina.core.service.IoHandlerAdapter;

import org.apache.mina.core.session.IdleStatus;
import org.apache.mina.core.session.IoSession;
import org.eclipse.jetty.continuation.Continuation;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.alibaba.fastjson.JSON;

import com.sysu.cloudgaminghub.config.Config;
import com.sysu.cloudgaminghub.hub.HubManager;
import com.sysu.cloudgaminghub.hub.NodeBean;
import com.sysu.cloudgaminghub.hub.nodenetwork.NodeNetworkHandler;
import com.sysu.cloudgaminghub.hub.nodenetwork.bean.NodeReportBean;
import com.sysu.cloudgaminghub.hub.nodenetwork.bean.NodeRunResponseBean;


public class NodeNetworkHandler extends IoHandlerAdapter{
	private static Logger logger = LoggerFactory.getLogger(NodeNetworkHandler.class);
	 @Override
	    public void exceptionCaught( IoSession session, Throwable cause ) throws Exception
	    {
		 	logger.warn("Record A Exception:{}, Session will be closed",cause.getMessage());
	 		session.close(true);
	    }
	    @Override
	    public void messageReceived( IoSession session, Object message ) throws Exception
	    {
	    	HubManager.getHubManager().onGotNodeMessage(session, message);
	    }
	    @Override
	    public void sessionIdle( IoSession session, IdleStatus status ) throws Exception
	    {
	    	
	    }
	    @Override
	    public void sessionCreated(IoSession session)
	    {
	    	logger.info("Got a connection from {}",session.getRemoteAddress());
	    }
	    @Override
	    public void sessionClosed(IoSession session)
	    {
	    	String hostName=(String)session.getAttribute(Config.HOSTNAMEKEY);
	    	if(hostName!=null)
	    	{
	    		HubManager.getHubManager().removeNode(hostName);
	    	}
	    	session.removeAttribute(Config.HOSTNAMEKEY);
	    	session.close(true);
	    }
	    
	
}
