package com.sysu.cloudgaming.node.network.bean;

public class NodeReportBean {
	private String Hostname=null;
	private long totalMemory=0;
	private long freeMemory=0;
	private long totalSwapMemory=0;
	private long freeSwapMemory=0;
	private int cpuUsage=0;
	private int cpuCoreNum=0;
	private int cpuPower=0;
	private String ipAddr=null;
	private String macAddr=null;
	private double upTime=0;
	private boolean runningFlag=false;
	private String runningApplication=null;
	private String runningApplicationVersion=null;
	private String runningApplicationPath=null;
	public String getHostname() {
		return Hostname;
	}
	public void setHostname(String hostname) {
		Hostname = hostname;
	}
	public boolean isRunningFlag() {
		return runningFlag;
	}
	public void setRunningFlag(boolean runningFlag) {
		this.runningFlag = runningFlag;
	}
	public String getRunningApplication() {
		return runningApplication;
	}
	public void setRunningApplication(String runningApplication) {
		this.runningApplication = runningApplication;
	}
	public String getRunningApplicationVersion() {
		return runningApplicationVersion;
	}
	
	public String getRunningApplicationPath() {
		return runningApplicationPath;
	}
	public void setRunningApplicationPath(String runningApplicationPath) {
		this.runningApplicationPath = runningApplicationPath;
	}
	public long getTotalMemory() {
		return totalMemory;
	}
	public void setTotalMemory(long totalMemory) {
		this.totalMemory = totalMemory;
	}
	public long getFreeMemory() {
		return freeMemory;
	}
	public void setFreeMemory(long freeMemory) {
		this.freeMemory = freeMemory;
	}
	public long getTotalSwapMemory() {
		return totalSwapMemory;
	}
	public void setTotalSwapMemory(long totalSwapMemory) {
		this.totalSwapMemory = totalSwapMemory;
	}
	public long getFreeSwapMemory() {
		return freeSwapMemory;
	}
	public void setFreeSwapMemory(long freeSwapMemory) {
		this.freeSwapMemory = freeSwapMemory;
	}
	public int getCpuUsage() {
		return cpuUsage;
	}
	public void setCpuUsage(int cpuUsage) {
		this.cpuUsage = cpuUsage;
	}
	public int getCpuCoreNum() {
		return cpuCoreNum;
	}
	public void setCpuCoreNum(int cpuCoreNum) {
		this.cpuCoreNum = cpuCoreNum;
	}
	public int getCpuPower() {
		return cpuPower;
	}
	public void setCpuPower(int cpuPower) {
		this.cpuPower = cpuPower;
	}
	public String getIpAddr() {
		return ipAddr;
	}
	public void setIpAddr(String ipAddr) {
		this.ipAddr = ipAddr;
	}
	public String getMacAddr() {
		return macAddr;
	}
	public void setMacAddr(String macAddr) {
		this.macAddr = macAddr;
	}
	public double getUpTime() {
		return upTime;
	}
	public void setUpTime(double upTime) {
		this.upTime = upTime;
	}
	public void setRunningApplicationVersion(String runningApplicationVersion) {
		this.runningApplicationVersion = runningApplicationVersion;
	}
	
}
