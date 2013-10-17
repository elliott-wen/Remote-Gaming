package com.sysu.cloudgaming.node.network.upnp;

import java.net.InetAddress;
import java.util.Random;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;



public class UPNPManager {
	private static Logger logger = LoggerFactory.getLogger(UPNPManager.class);
	GatewayDiscover discover =null;
	GatewayDevice d=null;
	InetAddress localAddress=null;
	String externalIPAddress=null;
	int outboundPort=10240;
	boolean isPortOpened=false;
	Random random=new Random();
	public boolean isPortOpened()
	{
		if(!hasInit)
		{
			return false;
		}
		return isPortOpened;
		
	}
	public InetAddress getLocalAddress() {
		return localAddress;
	}
	public void setLocalAddress(InetAddress localAddress) {
		this.localAddress = localAddress;
	}
	public String getExternalIPAddress() {
		return externalIPAddress;
	}
	public void setExternalIPAddress(String externalIPAddress) {
		this.externalIPAddress = externalIPAddress;
	}
	boolean hasInit=false;
	public boolean initManager()
	{
		try
		{
			discover = new GatewayDiscover();
	        logger.info("Looking for Gateway Devices");
	        discover.discover();
	        d = discover.getValidGateway();
	
	        if (null != d) {
	            logger.info("Gateway device found.{} ({})", new Object[]{d.getModelName(), d.getModelDescription()});
	        } else {
	            logger.info("No valid gateway device found.");
	            return false;
	        }
	        localAddress = d.getLocalAddress();
	        logger.info("Using local address: {}", localAddress);
	        String externalIPAddress = d.getExternalIPAddress();
	        logger.info("External address: {}", externalIPAddress);
	        hasInit=true;
	        return true;
		}
		catch(Exception e)
		{
			logger.warn(e.getMessage(),e);
			return false;
		}
	}
	public boolean isInit()
	{
		return hasInit;
	}
	public boolean setupUPNPMapping(int localPort)
	{
		if(!hasInit) return false;
		
		int retryTime=0;
		try
		{
			while(retryTime<8)
			{
				outboundPort=10240+random.nextInt(50000);
		        PortMappingEntry portMapping = new PortMappingEntry();
		        logger.info("Attempting to map port {}---{}", localPort,outboundPort);
		        if (!d.getSpecificPortMappingEntry(outboundPort,"UDP",portMapping)) {
		            logger.info("Sending port mapping request");
		            if (d.addPortMapping(outboundPort,localPort,localAddress.getHostAddress(),"UDP","ForCloudGaming"))
		            {
		                logger.info("Mapping succesful");
		                isPortOpened=true;
		                return true;
		            } else 
		            {
		                logger.info("Port mapping failed");
		                return false;
		            }
		            
		        } 
		        else 
		        {
		        	retryTime++;
		            logger.info("Port was already mapped! Try found another.");
		        }
			}

		}
		catch(Exception e)
		{
			logger.warn("Failed to open upnp device",e);
			return false;
		}
		return false;
	}
	public int getOutboundPort() {
		return outboundPort;
	}
	public void setOutboundPort(int outboundPort) {
		this.outboundPort = outboundPort;
	}
	/*
	 * UPNP UNHOLE FUNCTION
	 */
	public boolean removeUPNPPort(int outboundPort)
	{
		if(!hasInit) return false;
		try
		{
	        d.deletePortMapping(outboundPort,"UDP");
	        isPortOpened=false;
	        return true;
		}
		catch(Exception e)
		{
			logger.info(e.getMessage(),e);
			return false;
		}
	}
}
