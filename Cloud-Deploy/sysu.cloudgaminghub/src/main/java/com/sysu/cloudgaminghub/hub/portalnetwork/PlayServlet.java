package com.sysu.cloudgaminghub.hub.portalnetwork;

import java.io.IOException;

import java.io.PrintWriter;


import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.eclipse.jetty.continuation.Continuation;
import org.eclipse.jetty.continuation.ContinuationSupport;

import com.alibaba.fastjson.JSON;
import com.sysu.cloudgaminghub.config.Config;
import com.sysu.cloudgaminghub.hub.HubManager;
import com.sysu.cloudgaminghub.hub.nodenetwork.bean.NodeRunRequestBean;
import com.sysu.cloudgaminghub.hub.nodenetwork.bean.NodeRunResponseBean;

public class PlayServlet extends HttpServlet{

	/**
	 * 
	 */
	
	private static final long serialVersionUID = 1L;
	protected void doGet(HttpServletRequest req, HttpServletResponse resp)
            throws ServletException, IOException {
		execute(req,resp);
		
	}
	protected void doPost(HttpServletRequest req, HttpServletResponse resp)
            throws ServletException, IOException {
		execute(req,resp);
		
	}
	private void execute(HttpServletRequest req, HttpServletResponse resp)
            throws ServletException, IOException {
		Continuation continuation = ContinuationSupport.getContinuation(req);
		if(req.getAttribute(Config.CONTINUATIONKEY)==null)
		{
			
			continuation.setTimeout(Config.CONTINUATIONTIMEOUT);
			int quality=4;
			try
			{
				quality=Integer.valueOf(req.getParameter("quality"));
			}
			catch(Exception e)
			{
				
			}
			String programId=req.getParameter("programId");
			if(programId==null)
			{
				programId="1";
			}
			NodeRunRequestBean bean=new NodeRunRequestBean();
			bean.setProgramId(programId);
			bean.setQuality(quality);
			int result=HubManager.getHubManager().sendPlayRequest(bean, continuation);
			if(result==NodeRunResponseBean.NOERROR)
			{
				continuation.suspend();
			}
			else
			{
				PrintWriter writer=	resp.getWriter();
				writer.print("{\"errorCode\":0,\"successful\":false}");
			}
		}
		else
		{
			
			PrintWriter writer=	resp.getWriter();
			if(continuation.isExpired())
			{
				writer.print("{\"errorCode\":0,\"successful\":false}");
				return;
			}
			NodeRunResponseBean b=(NodeRunResponseBean)req.getAttribute(Config.RUNRESPONSEBEAN);
			//Json output not implement yet
			if(b==null)
			{
				writer.print("{\"errorCode\":0,\"successful\":false}");
			}
			else
			{
				writer.print(JSON.toJSONString(b));
			}
			
		}
		
	}
}
