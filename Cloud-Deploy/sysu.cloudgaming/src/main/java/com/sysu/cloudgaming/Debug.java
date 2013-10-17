package com.sysu.cloudgaming;

import java.io.File;

public class Debug {
	public static void  main(String args[])
	{
		File f=new File("C:/windows/system32/notepad.exe");
		System.out.println(f.getParent());
	}
}
