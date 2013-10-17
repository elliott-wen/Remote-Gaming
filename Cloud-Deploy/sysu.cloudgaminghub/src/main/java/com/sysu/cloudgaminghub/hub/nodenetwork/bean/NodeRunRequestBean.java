package com.sysu.cloudgaminghub.hub.nodenetwork.bean;



public class NodeRunRequestBean {
	public static final int HD1024_768_6M=1;
	public static final int SD800_600_4M=2;
	public static final int CD640_480_2M=3;
	public static final int LD320_240_1M=4;
	public int clientPort=0;
	public String clientAddress=null;
	private int quality;
	private String programId;
	public int getQuality() {
		return quality;
	}
	public void setQuality(int quality) {
		this.quality = quality;
	}
	public String getProgramId() {
		return programId;
	}
	public void setProgramId(String programId) {
		this.programId = programId;
	}
	public int getClientPort() {
		return clientPort;
	}
	public void setClientPort(int clientPort) {
		this.clientPort = clientPort;
	}
	public String getClientAddress() {
		return clientAddress;
	}
	public void setClientAddress(String clientAddress) {
		this.clientAddress = clientAddress;
	}
}
