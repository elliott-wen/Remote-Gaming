package com.sysu.cloudgaming.utils;

import java.net.InetAddress;

//import org.hyperic.sigar.CpuInfo;
//import org.hyperic.sigar.NetFlags;
//import org.hyperic.sigar.NetInterfaceConfig;
//import org.hyperic.sigar.Sigar;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;


public class SystemMonitor {
	/*
	private static Logger logger = LoggerFactory.getLogger(SystemMonitor.class);
	private static Sigar sigar = new Sigar(); 
	public static double getUpTime()
	{
		double upTime=0;
		try
		{
			upTime=sigar.getUptime().getUptime();
		}
		catch(Exception e)
		{
			logger.warn(e.getMessage(),e);
		}
		return upTime;
	}


	public static String getMacAddr()
	{
		String hwaddr = null;
		try
		{
			String[] ifaces = sigar.getNetInterfaceList();
			 for (int i = 0; i < ifaces.length; i++) 
			 {
                 NetInterfaceConfig cfg = sigar.getNetInterfaceConfig(ifaces[i]);
                 if (NetFlags.LOOPBACK_ADDRESS.equals(cfg.getAddress())
                        || (cfg.getFlags() & NetFlags.IFF_LOOPBACK) != 0
                        || NetFlags.NULL_HWADDR.equals(cfg.getHwaddr())) {
                     continue;
                 }
                
                 hwaddr = cfg.getHwaddr();
                 break;
             }
		}
		catch(Exception e)
		{
			logger.warn(e.getMessage(),e);
		}
		if(hwaddr==null)
		{
			hwaddr="0e:0e:12:15:16:17";//Useless hw addr
		}
		return hwaddr;
	}
	public static String getIPAddr()
	{
		String address=null;
		  try 
		  {
              address = InetAddress.getLocalHost().getHostAddress();
             
              	if (!NetFlags.LOOPBACK_ADDRESS.equals(address)) 
              {
                   return address;
              }
          } 
		  catch (Exception e) 
		  {
			  logger.warn("Try another way getting ip",e);
          }
        try 
        {
            address = sigar.getNetInterfaceConfig().getAddress();
        }
        catch(Exception e) 
        {
        	logger.warn(e.getMessage(),e);
            address = NetFlags.LOOPBACK_ADDRESS;
        } 
        return address;
	}
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
	}*/
}
