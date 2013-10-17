package com.sysu.cloudgaming.node;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.quigley.zabbixj.metrics.MetricsException;
import com.quigley.zabbixj.metrics.MetricsKey;
import com.quigley.zabbixj.metrics.MetricsProvider;

public class SystemProvider implements MetricsProvider{
	private static Logger logger = LoggerFactory.getLogger(ZabbixClient.class);
	@Override
	public Object getValue(MetricsKey mKey) throws MetricsException {
		if(mKey.getKey().equals("cpuload"))
		{
			return SystemMonitor.getCpuUsage();
		}
		else if(mKey.getKey().equals("cpupower"))
		{
			return SystemMonitor.getCpuPower();
		}
		else if(mKey.getKey().equals("cpucorenum"))
		{
			return SystemMonitor.getCpuCoreNum();
		}
		else if(mKey.getKey().equals("memorysize"))
		{
			if(!mKey.isParameters())
			{
				logger.warn("Suppose to have paremater");
			}
			if(mKey.getParameters()[0].equals("total"))
			{
				return SystemMonitor.getTotalMemory();
			}
			else if(mKey.getParameters()[0].equals("free"))
			{
				return SystemMonitor.getFreeMemory();
			}
			else 
			{
				logger.warn("Unrecoginize paramater");
			}
		}
		else if(mKey.getKey().equals("swapsize"))
		{
			if(!mKey.isParameters())
			{
				logger.warn("Suppose to have paremater");
			}
			if(mKey.getParameters()[0].equals("total"))
			{
				return SystemMonitor.getTotalSwapMemory();
			}
			else if(mKey.getParameters()[0].equals("free"))
			{
				return SystemMonitor.getFreeSwapMemory();
			}
			else 
			{
				logger.warn("Unrecoginize paramater");
			}
		}
		return "Unsupported";
	}

}
