package com.sysu.cloudgaming.node;

import java.util.Random;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.quigley.zabbixj.metrics.MetricsException;
import com.quigley.zabbixj.metrics.MetricsKey;
import com.quigley.zabbixj.metrics.MetricsProvider;
import com.sysu.cloudgaming.config.Config;

public class CloudGamingProvider implements MetricsProvider{
	private static Logger logger = LoggerFactory.getLogger(ZabbixClient.class);
	@Override
	public Object getValue(MetricsKey arg0) throws MetricsException {
		
		//System.out.println(arg0.getKey());
		if(arg0.getKey().equals("ApplicationVer"))
		{
			return Config.VERSION;
		}
		return "Unsupported Key";
	}

}
