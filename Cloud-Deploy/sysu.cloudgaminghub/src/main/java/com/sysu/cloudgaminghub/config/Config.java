package com.sysu.cloudgaminghub.config;

import java.io.FileInputStream;
import java.util.Properties;


import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.sysu.cloudgaminghub.App;

public class Config {
	private static Logger logger = LoggerFactory.getLogger(App.class);
	public static int HUBPORT=10060;
	public static int PORTALPORT=8090;
	public static int STUNPORT=3478;
	public static final String PROPERTYFILE="agentd.conf";
	public static String BASEPATH="d:/cloudgaming/";
	public static String VERSION="1.03 Alpha";
	public static final String HOSTNAMEKEY="HOSTNAME";
	public static final String CONTINUATIONKEY="CONTINUATION";
	public static final String RUNRESPONSEBEAN="RUNRESPONSEBEAN";
	public static final String SUCCESSFULKEY="SUCCESS";
	public static int CONTINUATIONTIMEOUT=30000;
	public static boolean loadConfig()
	{
		logger.info("Build Version:{}",VERSION);
		try
		{
			Properties p = new Properties();
		    p.load(new FileInputStream(BASEPATH+PROPERTYFILE));
		    return true;
		}
		catch(Exception e)
		{
			logger.warn("Failed to read config file");
			return false;
		}
	}
	
}
