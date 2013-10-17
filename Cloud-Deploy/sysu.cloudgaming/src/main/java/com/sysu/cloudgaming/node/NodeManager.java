package com.sysu.cloudgaming.node;


import java.io.File;



import java.net.InetAddress;

import java.util.HashMap;
import java.util.List;
import java.util.Map;



import org.apache.mina.core.session.IoSession;
import org.jdom2.Document;
import org.jdom2.Element;
import org.jdom2.input.SAXBuilder;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.alibaba.fastjson.JSON;
import com.sysu.cloudgaming.config.Config;
import com.sysu.cloudgaming.node.network.HubMessage;
import com.sysu.cloudgaming.node.network.NodeMessage;
import com.sysu.cloudgaming.node.network.NodeNetwork;
import com.sysu.cloudgaming.node.network.bean.NodeReportBean;
import com.sysu.cloudgaming.node.network.bean.NodeRunRequestBean;
import com.sysu.cloudgaming.node.network.bean.NodeRunResponseBean;


import com.sysu.cloudgaming.node.network.upnp.UPNPManager;
//import com.sysu.cloudgaming.utils.SystemMonitor;

public class NodeManager {
	private static NodeManager manager=null;
	private static Logger logger = LoggerFactory.getLogger(NodeManager.class);
	private Map<String,ProgramBean> programMap=new HashMap<String,ProgramBean>();
	private boolean runningFlag=false;
	private ProgramBean runningbean=null;
	private int errorCode=0;
	private Process gameProcess=null;
	private Process deamonProcess=null;
	private NodeNetwork nodeNetwork=new NodeNetwork();
	private UPNPManager upnpManager=new UPNPManager();
	public static int WATCHDOGINTERVAL=1000;
	private int localPort=20000;
	public int getLocalPort() {
		return localPort;
	}
	public void setLocalPort(int localPort) {
		this.localPort = localPort;
	}
	public void onSessionIdle(IoSession session)
	{
		session.write(generateInstanceReportMessage());
	}
	private void onRequestReport(IoSession session)
	{
		session.write(generateInstanceReportMessage());
	}
	public void onGotHubMessage(IoSession session,Object message)
	{
		HubMessage hubMessage=(HubMessage)message;
    	switch(hubMessage.getMessageType())
    	{
    		case HubMessage.INSTANCEREPORTREQUESTMESSAGE:
    		{
    			logger.info("Instance Report Request From Server");
    			onRequestReport(session);
    		}
    		break;
    		case HubMessage.RUNREQUESTMESSAGE:
    		{
    			 logger.info("Run Command Request From Server");
    			 onRequestRun(session, hubMessage);
    		}
    			break;
    		case HubMessage.SHUTDOWNREQUESTMESSAGE:
    		{
    			logger.info("Shutdown Request From Server");
    			onRequestShutdown(session, hubMessage);
    		}
    			break;
    		default:
    			logger.warn("Bad protocol found!");
    			break;
    	}
	}
	private void onRequestRun(IoSession session,HubMessage message)
	{

		NodeRunRequestBean b=JSON.parseObject(message.getExtendedData(), NodeRunRequestBean.class);
		NodeRunResponseBean response=startApplication(b);
		session.write(generateRunCommandResponseMessage(response));
	}
	private void onRequestShutdown(IoSession session,HubMessage message)
	{
		boolean result=NodeManager.getNodeManager().shutdownApplication();
		session.write(generateShutdownCommandResponseMessage(result,NodeManager.getNodeManager().getLastError()));
	}
	 private NodeMessage generateRunCommandResponseMessage(NodeRunResponseBean b)
	    {
	    	NodeMessage msg=new NodeMessage();
	    	msg.setMessageType(NodeMessage.RUNRESPONSEMESSAGE);
	    	byte []extendData=null;
		    extendData=JSON.toJSONBytes(b);
		    msg.setMessageLength(extendData.length);
		    msg.setExtendedData(extendData);
	    	return msg;
	    }
	    private NodeMessage generateShutdownCommandResponseMessage(boolean successful,int errorcode)
	    {
	    	NodeMessage msg=new NodeMessage();
	    	msg.setMessageType(NodeMessage.SHUTDOWNRESPONSEMESSAGE);
	    	//msg.setErrorCode(errorcode);
	    	//msg.setSuccess(successful);
	    	return msg;
	    }
	    private NodeMessage generateInstanceReportMessage() 
	    {
	    	NodeMessage message=new NodeMessage();
	    	message.setMessageType(NodeMessage.INSTANCEREPORTMESSAGE);
	    	NodeReportBean b=new NodeReportBean();
	    	NodeManager manager=NodeManager.getNodeManager();
	    	b.setHostname(Config.HOSTNAME);
	    	b.setRunningFlag(manager.isNodeRunningApplication());
	    	if(manager.isNodeRunningApplication())
	    	{

	    		b.setRunningApplication(manager.getRunningApplicationProgramBean().getProgramVersion());
	    		b.setRunningApplication(manager.getRunningApplicationProgramBean().getProgramName());
	    		b.setRunningApplicationPath(manager.getRunningApplicationProgramBean().getProgramPath());
	    		//b.setCpuCoreNum(SystemMonitor.getCpuCoreNum());
	    		//b.setCpuPower(SystemMonitor.getCpuPower());
	    		//b.setCpuUsage(SystemMonitor.getCpuUsage());
	    		b.setHostname(Config.HOSTNAME);
	    		//b.setIpAddr(SystemMonitor.getIPAddr());
	    		//b.setMacAddr(SystemMonitor.getMacAddr());
	    	}
	    	byte []extendData=null;
	    	extendData=JSON.toJSONBytes(b);
	    	message.setMessageLength(extendData.length);
	    	message.setExtendedData(extendData);
	    	return message;
	    }
	private class WatchDogThread extends Thread
	{
		public void run()
		{

			while(true)
			{
				try
				{
					sleep(WATCHDOGINTERVAL);
				}
				catch(Exception e)
				{

				}
				try
				{
					int exitValue=gameProcess.exitValue();
					logger.info("Game Process Exited:{}",exitValue);
					break;
				}
				catch(IllegalThreadStateException e)
				{
					logger.debug("Game Process still running");
				}
				try
				{
					int exitValue=deamonProcess.exitValue();
					logger.info("Daemon Process Exited:{}",exitValue);
					break;
				}
				catch(IllegalThreadStateException e)
				{
					logger.debug("Deamon Process still running");
				}
			}
			killAllProcess();
		}
	}
	WatchDogThread watchThread=null;
	private void killAllProcess()
	{

		killGameProcess();
		killDeamonProcess();
		runningFlag=false;
		//nodeNetwork.sendRunningFinishMessage(true, 0);

	}
	private void killGameProcess()
	{
		try
		{
			gameProcess.destroy();
			logger.info("Game Process was killed");
		}
		catch(Exception e)
		{
			logger.warn("Try to terminate game process But Failed");
		}
	}
	private void killDeamonProcess()
	{
		try
		{


			logger.info("Shutting down deamon by sending quit button");
			Thread.sleep(10000);
		}
		catch(Exception e)
		{
			logger.warn("Try to terminate deamon process But Failed");
		}
		deamonProcess.destroy();
		logger.info("Daemon Process was killed");
	}
	public static NodeManager getNodeManager()
	{

		if(manager==null)
		{
			manager=new NodeManager();
		}
		return manager;
	}
	private NodeManager()
	{


	}
	public ProgramBean getRunningApplicationProgramBean()
	{
		return this.runningbean;
	}

	public boolean initNodeManager()
	{
		if(!searchLocalProgram())
		{
			logger.warn("Failed to search local program!");
        	return false;
		}
		if(!nodeNetwork.setupNodeNetwork())
        {
        	logger.warn("Init Node Network Failed!");
        	return false;
        }
		if(!upnpManager.initManager())
		{
			logger.warn("Unable to find upnp device!");
		}
		return true;
	}
	public boolean isNodeRunningApplication()
	{
		return this.runningFlag;
	}
	public int getLastError()
	{
		return this.errorCode;
	}
	public NodeRunResponseBean startApplication(NodeRunRequestBean rb)
	{
		NodeRunResponseBean b=new NodeRunResponseBean();
		if(runningFlag)
		{
			b.setSuccessful(false);
			b.setErrorCode(NodeRunResponseBean.ALREADYRUNNING);
			logger.warn("System Busy! Already Running Game");
			return b;
		}

		b.setPort(localPort);
		try
		{
			b.setServerIp(InetAddress.getLocalHost().getHostAddress());
		}
		catch(Exception e)
		{
			logger.warn("Unable to get local node address!",e);
			b.setSuccessful(false);
			b.setErrorCode(NodeRunResponseBean.NETWORKERROR);
			return b;
		}
		if(upnpManager.isInit())
		{
			logger.info("Try to open upnp port");
			if(upnpManager.setupUPNPMapping(localPort))//Some Error may happen!!!!
			{
				logger.info("UPNP port open ok!");
				b.setServerIp(upnpManager.getExternalIPAddress());
				b.setPort(upnpManager.getOutboundPort());
			}
			else
			{
				logger.warn("Unable to open port");
			}
		}
		if(!executeDeamonApplication(rb.getQuality()))
		{
			logger.warn("Unable to init server deamon");
			killAllProcess();
			b.setSuccessful(false);
			b.setErrorCode(NodeRunResponseBean.EXECFAILED);
			return b;
		}
		if(!executeGameApplication(rb.getProgramId()))
		{
			logger.warn("Unable to init game application");
			killAllProcess();
			b.setSuccessful(false);
			b.setErrorCode(NodeRunResponseBean.EXECFAILED);
			return b;
		}
		b.setSuccessful(true);
		runningFlag=true;
		watchThread=new WatchDogThread();
		watchThread.start();
		return b;
	}
	private boolean executeDeamonApplication(int quality)
	{
		try
		{
			logger.info("Try to open deamon process in quality {}",quality);
			ProcessBuilder builder=new ProcessBuilder(Config.BASEPATH+Config.DEAMONPATH,"-q "+quality+" -p "+this.localPort);
			builder.redirectOutput(new File("DeamonLog.txt"));
			builder.redirectErrorStream(true);

			deamonProcess=builder.start();

			return true;
		}
		catch(Exception e)
		{
			logger.warn(e.getMessage(),e);
			return false;
		}

	}
	private boolean executeGameApplication(String programId)
	{
		if(!programMap.containsKey(programId))
		{
			logger.warn("Local disk don't have such game!");
			return false;
		}
		else
		{
			this.runningbean=programMap.get(programId);
			logger.info("Try to execute {} in {}",runningbean.getProgramName(),runningbean.getProgramPath());
			try
			{
				ProcessBuilder builder=new ProcessBuilder(runningbean.getProgramPath());
				builder.directory(new File(runningbean.getProgramPath()).getParentFile());
				gameProcess=builder.start();
				return true;
			}
			catch(Exception e)
			{
				logger.warn(e.getMessage(),e);
				return false;
			}
		}
	}

	public boolean shutdownApplication()
	{
		if(!runningFlag)
		{
			logger.warn("Application is not running yet");
			return false;
		}
		else
		{
			killAllProcess();
			return true;
		}
	}

	public boolean sendRunningFinishMessage(boolean successful,int errorcode)
	{
		NodeMessage msg=new NodeMessage();
		msg.setMessageType(NodeMessage.RUNNINGFINISHMESSAGE);
		//msg.setSuccess(successful);
		//msg.setErrorCode(errorcode);
		nodeNetwork.getNetworkSession().write(msg);
		return true;
	}


	/*
	 * UDP HOLE FUNCTION
	 */
/*	private SocketAddress requestStunForPublicAddress()
	{
		DatagramSocket client=null;
		byte[] sendBuf=new byte[32];
		byte[] recvBuf = new byte[32];
		DatagramPacket sendPacket =null;
		DatagramPacket recvPacket=null;
		try
		{
			client = new DatagramSocket();
			client.setReuseAddress(true);
			client.setSoTimeout(3000);
			
			sendPacket  = new DatagramPacket(sendBuf ,sendBuf.length , InetAddress.getByName(Config.HUBSERVERADDR) , Config.STUNPORT);
			recvPacket= new DatagramPacket(recvBuf , recvBuf.length);
		}
		catch(Exception e)
		{
			logger.warn("Init Socket client error");
		}
		int tryTime=0;
		while(tryTime<8)
		{
			try
			{
				client.send(sendPacket);
				client.receive(recvPacket);
				IoBuffer buffer=IoBuffer.allocate(recvBuf.length);
				buffer.put(recvBuf);
				buffer.flip();
				byte []ipaddress=new byte[4];
				buffer.get(ipaddress);
				int port=buffer.getInt();
				break;
			}
			catch(Exception e)
			{
				logger.warn("Recv Packet Failed! Retrying",e);
				tryTime++;
			}
		}
    	return null;
	}*/

	public  boolean searchLocalProgram()
	{

		try
		{
			programMap.clear();
			File infoFile=new File(Config.BASEPATH+Config.LOCALPROGRAMPATH+"/"+Config.LOCALPROGRAMXMLNAME);
			if(infoFile.exists())
			{
				SAXBuilder builder=new SAXBuilder();

					Document doc=builder.build(infoFile);
					Element info=doc.getRootElement();
					List<Element> games=info.getChildren("program");
					logger.info("Local disk have {} game",games.size());
					for(Element g: games)
					{
						ProgramBean b=new ProgramBean();
						b.setProgramID(g.getChildText("id"));
						b.setProgramName(g.getChildText("name"));
						b.setProgramVersion(g.getChildText("ver"));
						b.setProgramPath(Config.BASEPATH+Config.LOCALPROGRAMPATH+'/'+g.getChildText("path"));
						logger.info("Add Game to Map Id:{}, Name:{}",b.getProgramID(),b.getProgramName());
						programMap.put(b.getProgramID(), b);
					}
					return true;


			}
			else
			{
				logger.warn("Info File Not Existed!");
			}
			return false;
		}

		catch(Exception e)
		{
			logger.warn(e.getMessage(),e);
			return false;
		}
	}

}