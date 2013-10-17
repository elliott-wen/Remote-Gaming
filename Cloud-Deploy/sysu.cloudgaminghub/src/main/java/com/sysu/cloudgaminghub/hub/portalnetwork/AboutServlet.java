package com.sysu.cloudgaminghub.hub.portalnetwork;

import java.io.IOException;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

public class AboutServlet extends HttpServlet{
	/**
	 * 
	 */
	private static final long serialVersionUID = -774435172142614393L;

	protected void doGet(HttpServletRequest req, HttpServletResponse resp)
            throws ServletException, IOException {
        resp.getWriter().print("Cloud gaming hub designed by J.Wen!\n");
    }
}
