package com.sysu.cloudgaming.node.network.bean;

public class NodeRunResponseBean {
	public final static int SERVERBUSY=1;
	public final static int NOERROR=0;
	public final static int EXECFAILED=2;
	public final static int ALREADYRUNNING=3;
	public final static int NETWORKERROR=4;
	private boolean successful=false;
	private int errorCode=0;
	private String serverIp=null;
	private int port=0;
	public String getServerIp() {
		return serverIp;
	}
	public void setServerIp(String serverIp) {
		this.serverIp = serverIp;
	}
	public int getPort() {
		return port;
	}
	public void setPort(int port) {
		this.port = port;
	}
	public boolean isSuccessful() {
		return successful;
	}
	public void setSuccessful(boolean successful) {
		this.successful = successful;
	}
	public int getErrorCode() {
		return errorCode;
	}
	public void setErrorCode(int errorCode) {
		this.errorCode = errorCode;
	}
}
