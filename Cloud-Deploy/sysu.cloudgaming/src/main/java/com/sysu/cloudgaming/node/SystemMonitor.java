package com.sysu.cloudgaming.node;


import org.hyperic.sigar.CpuInfo;
import org.hyperic.sigar.Sigar;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;


public class SystemMonitor {
	private static Logger logger = LoggerFactory.getLogger(SystemMonitor.class);
	private static Sigar sigar = new Sigar(); 
	
	public static long getTotalMemory()
	{
		long mem=-1;
		try
		{
			mem=sigar.getMem().getTotal();
		}
		catch(Exception e)
		{
			logger.warn(e.getMessage(),e);
		}
		return mem;
	}
	public static long getFreeMemory()
	{
		long mem=-1;
		try
		{
			mem=sigar.getMem().getActualFree();
		}
		catch(Exception e)
		{
			logger.warn(e.getMessage(),e);
		}
		return mem;
	}
	public static long getTotalSwapMemory()
	{
		long mem=-1;
		try
		{
			mem=sigar.getSwap().getTotal();
		}
		catch(Exception e)
		{
			logger.warn(e.getMessage(),e);
		}
		return mem;
	
	}
	public static long getFreeSwapMemory()
	{
		long mem=-1;
		try
		{
			mem=sigar.getSwap().getFree();
		}
		catch(Exception e)
		{
			logger.warn(e.getMessage(),e);
		}
		return mem;
	}
	public static int getCpuUsage()
	{
		int combined=-1;
		try
		{
			combined=(int)(sigar.getCpuPerc().getCombined()*100);
		}
		catch(Exception e)
		{
			logger.warn(e.getMessage(),e);
		}
		return combined;
	}
	public static int getCpuPower()
	{
		int mhz=-1;
		try
		{
			CpuInfo[] list=sigar.getCpuInfoList();
			if(list!=null&&list.length!=0)
			{
				mhz=list[0].getMhz();
			}
		}
		catch(Exception e)
		{
			logger.warn(e.getMessage(),e);
		}
		return mhz;
	}
	public static int getCpuCoreNum()
	{
		int num=-1;
		try
		{
			CpuInfo[] list=sigar.getCpuInfoList();
			if(list!=null&&list.length!=0)
			{
				num=list[0].getTotalCores();
			}
		}
		catch(Exception e)
		{
			logger.warn(e.getMessage(),e);
		}
		return num;
	}
}
