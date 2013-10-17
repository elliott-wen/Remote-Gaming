package com.sysu.cloudgaminghub.hub.portalnetwork;

import java.io.IOException;
import java.io.PrintWriter;


import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

public class AuthServlet extends HttpServlet{

	/**
	 * 
	 */
	private final static String USERNAME1="test";
	private final static String PASSWORD1="test";
	private final static String USERNAME2="test1";
	private final static String PASSWORD2="test1";
	private static final long serialVersionUID = -5753953317338913902L;
	protected void doPost(HttpServletRequest req, HttpServletResponse resp)
            throws ServletException, IOException {
		execute(req,resp);
	}
	protected void doGet(HttpServletRequest req, HttpServletResponse resp)
            throws ServletException, IOException {
		execute(req,resp);
		
	}
	private void execute(HttpServletRequest req, HttpServletResponse resp)
            throws ServletException, IOException {
		PrintWriter writer=	resp.getWriter();
		String username=req.getParameter("username");
		String password=req.getParameter("password");
		if(username==null||password==null)
		{
			writer.print("{\"errorCode\":0,\"successful\":false}");
			return;
		}
		if((username.equals(USERNAME1)&&password.equals(PASSWORD1))||(username.equals(USERNAME2)&&password.equals(PASSWORD2)))
		{
			writer.print("{\"errorCode\":0,\"successful\":true}");
		}
		else
		{
			writer.print("{\"errorCode\":0,\"successful\":false}");
		}
	}
}
