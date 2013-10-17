package com.sysu.cloudgaminghub.hub.portalnetwork;

import org.eclipse.jetty.server.Server;

import org.eclipse.jetty.servlet.ServletContextHandler;
import org.eclipse.jetty.servlet.ServletHolder;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.sysu.cloudgaminghub.config.Config;



public class PortalNetwork {
	private static Logger logger = LoggerFactory.getLogger(PortalNetwork.class);
	private Server server=null;
	private ServletContextHandler context=null;
	public boolean startPortalNetwork()
	{
		 try
	     {
			 server = new Server(Config.PORTALPORT);
		     context = new ServletContextHandler(ServletContextHandler.SESSIONS);
		     context.setContextPath("/");
		     server.setHandler(context);
		     context.addServlet(new ServletHolder(new PlayServlet()),"/play");
		     context.addServlet(new ServletHolder(new AuthServlet()),"/auth");
			 server.start();
			 return true;
	     }
		 catch(Exception e)
		 {
			 logger.warn(e.getMessage(),e);
			 return false;
		 }
	}
}