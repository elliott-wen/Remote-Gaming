package com.sysu.cloudgaminghub.hub;

import org.apache.mina.core.session.IoSession;
import org.eclipse.jetty.continuation.Continuation;


import com.sysu.cloudgaminghub.hub.nodenetwork.bean.NodeReportBean;


public class NodeBean {
	private String Hostname=null;
	private IoSession session=null;
	private NodeReportBean reportBean=null;
	private boolean runningFlag=false;
	private Continuation continuation=null;
	public String getHostname() {
		return Hostname;
	}
	public void setHostname(String hostname) {
		Hostname = hostname;
	}
	public IoSession getSession() {
		return session;
	}
	public void setSession(IoSession session) {
		this.session = session;
	}
	public NodeReportBean getReportBean() {
		return reportBean;
	}
	public void setReportBean(NodeReportBean reportBean) {
		this.reportBean = reportBean;
	}
	public boolean isRunningFlag() {
		return runningFlag;
	}
	public void setRunningFlag(boolean runningFlag) {
		this.runningFlag = runningFlag;
	}
	public Continuation getContinuation() {
		return continuation;
	}
	public void setContinuation(Continuation continuation) {
		this.continuation = continuation;
	}

}
