package com.sysu.cloudgaming.node.network;

public class NodeMessage {
	public final static int INSTANCEREPORTMESSAGE=1;
	public final static int RUNRESPONSEMESSAGE=2;
	public final static int SHUTDOWNRESPONSEMESSAGE=3;
	public final static int RUNNINGFINISHMESSAGE=4;
	private int messageLength;
	private int messageType=1;
	private byte extendedData[]=null;
	public int getMessageLength() {
		return messageLength;
	}
	public void setMessageLength(int messageLength) {
		this.messageLength = messageLength;
	}
	public int getMessageType() {
		return messageType;
	}
	public void setMessageType(int messageType) {
		this.messageType = messageType;
	}
	
	public byte[] getExtendedData() {
		return extendedData;
	}
	public void setExtendedData(byte[] extendedData) {
		this.extendedData = extendedData;
	}
	
}
