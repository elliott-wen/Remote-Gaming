package com.sysu.cloudgaming.node.network;

import org.apache.mina.core.session.IoSession;
import org.apache.mina.filter.codec.ProtocolCodecFactory;
import org.apache.mina.filter.codec.ProtocolDecoder;
import org.apache.mina.filter.codec.ProtocolEncoder;

public class NetworkCodecFactory implements ProtocolCodecFactory{
	
	private ProtocolEncoder encoder;
    private ProtocolDecoder decoder;
    public NetworkCodecFactory()
    {
    	encoder = new NodeMessageEncoder();
        decoder = new HubMessageDecoder();

    }
	public ProtocolDecoder getDecoder(IoSession arg0) throws Exception {
		
		return decoder;
	}
	public ProtocolEncoder getEncoder(IoSession arg0) throws Exception {
		
		return encoder;
	}
}
