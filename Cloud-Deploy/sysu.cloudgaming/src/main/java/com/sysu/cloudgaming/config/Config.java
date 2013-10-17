package com.sysu.cloudgaming.config;

import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.util.Properties;
import java.util.UUID;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;


public class Config {
	public static final String PROPERTYFILE="agentd.conf";
	public static String BASEPATH="d:/cloudgaming/";
	public static String VERSION="1.08 Alpha";
	public static String LOCALPROGRAMPATH="Games/";
	public static String DEAMONPATH="Server.exe";
	public static String LOCALPROGRAMXMLNAME="info.xml";
	public static String HUBSERVERADDR="222.200.182.75";
	public static int SOTIMEOUT=3000;
	public static int HUBSERVERPORT=10060;
	public static int STUNPORT=3478;
	public static int CONNECT_TIMEOUT=3000;
	public static String HOSTNAME="GameHost-X-NONAME";
	public static int REFRESHINTEVAL=10;
	private static Logger logger = LoggerFactory.getLogger(Config.class);
	public static boolean loadConfig()
	{
		logger.info("Build Version:{}",VERSION);
		try
		{
			Properties p = new Properties();
		    p.load(new FileInputStream(BASEPATH+PROPERTYFILE));
		    logger.info("Server address {}",p.getProperty("ServerActive"));
		    HUBSERVERADDR=p.getProperty("ServerActive");
		    BASEPATH=p.getProperty("BasePath");
		  
		    HOSTNAME=p.getProperty("Hostname");
		    logger.info("HostName {}",HOSTNAME);
		    if(HOSTNAME==null)
		    {
		    	HOSTNAME=UUID.randomUUID().toString();
		    	p.setProperty("Hostname", HOSTNAME);
		    	p.store(new FileOutputStream(BASEPATH+PROPERTYFILE), "");
		    	logger.info("Generate A host name {}",HOSTNAME);
		    }
		    
		    return true;
		}
		catch(Exception e)
		{
			logger.warn("Failed to read config file");
			return false;
		}
	}
}
