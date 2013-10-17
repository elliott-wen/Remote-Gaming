package com.sysu.cloudgaming.node;

import java.net.InetAddress;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.quigley.zabbixj.agent.ZabbixAgent;
import com.sysu.cloudgaming.config.Config;

public class ZabbixClient {
	ZabbixAgent agent=null;
	private static Logger logger = LoggerFactory.getLogger(ZabbixClient.class);
	public boolean startClient()
	{
		try
		{
			ZabbixAgent agent = new ZabbixAgent();
			agent.setEnableActive(true);
			agent.setEnablePassive(false);
			agent.setServerAddress(InetAddress.getByName(Config.ZABBIXSERVERADDR));
			agent.setServerPort(Config.ZABBIXSERVERPORT);
			agent.setHostName(Config.HOSTNAME);
			agent.setRefreshInterval(Config.REFRESHINTEVAL);
			agent.addProvider("cloudgaming", new CloudGamingProvider());
			agent.addProvider("system", new SystemProvider());
			agent.start();
			return true;
		}
		catch(Exception e)
		{
			logger.warn(e.getMessage(),e);
			return false;
		}
	}
}
