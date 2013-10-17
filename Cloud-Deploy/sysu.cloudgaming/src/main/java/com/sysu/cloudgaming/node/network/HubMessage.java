package com.sysu.cloudgaming.node.network;



public class HubMessage {
	public static final int RUNREQUESTMESSAGE=1;
	public static final int SHUTDOWNREQUESTMESSAGE=2;
	public static final int INSTANCEREPORTREQUESTMESSAGE=3;
	
	private int messageLength=0;
	private int messageType=0;
	private byte[] extendedData=null;
	
	public void setMessageType(int type)
	{
		this.messageType=type;
	}
	public int getMessageType()
	{
		return messageType;
	}
	
	public int getMessageLength() {
		return messageLength;
	}
	public void setMessageLength(int messageLength) {
		this.messageLength = messageLength;
	}
	public byte[] getExtendedData() {
		return extendedData;
	}
	public void setExtendedData(byte[] extendedData) {
		this.extendedData = extendedData;
	}
}
