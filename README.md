
##The project is a cloud gaming prototype system (or called remote gaming system).

The following is the quick guide for compliing and deploying the system. If you have any problem, feel free to send email
to jq.elliott.wen@gmail.com. If you want to contribute your code, you can just fork it or pull your request after sending mail to me. I would be glad if you join the development.

Warning: Please obey the license from SDL and FFMPEG2.

##1. Brief Introduction 

	With the development of cloud computing technology, cloud applications become very popular in the recent years.
	Cloud gaming is a promising application of the rapidly developing cloud computing technology. 
	As for traditional games, most calculations are normally performed in client side. 
	It's necessary for users to set up a powerful machine and install huge application packages advanced. 
	In contrast, most calculations are performed in remote servers with cloud gaming technology. 
	Under this case, it's easy for users to play games without worrying about the performance of machine.

	Most of existing virtual desktop technology can meet the requirement of controlling remote computer in a low frame rate. 
	However, such technology works poor in high frame rate condition when playing games. It not only consumes lots of bandwidth but also yields high latency. 
	To sum up, the biggest obstacle for cloud gaming is to design an efficient system with high responsiveness and low bandwidth consummation.

	In this paper, we present a novel cloud gaming system which is full of extensibility, portability and configurability. 
	Based on this system, it's easy to set up a private gaming cloud. As is known to all, most games only run in the Windows environment. 
	So we implement the server part in Windows platform, while its client can be readily ported to other OS's, including Windows, Linux, Android and IOS. 
	We also conduct extensive experiments to evaluate the performance of the system. 
	My experimental results indicate that our system is efficient enough and can provide high responsiveness and video quality with low latency and low bandwidth consummation.
	The system yields a per-frame video delay of 34 milliseconds and audio delay of 10 milliseconds, which is tiny for human-being to notice. 
	The system also provides several quality configuration including high definition, standard definition and low definition. 
	My experiments also reveal that all these performance gains are achieved without the expense of higher network loads. Actually, common ADSL bandwidth can meet the requirement of my system. 
	The proposed system can be employed by the researchers, game developers, service providers, and end users for setting up cloud gaming test beds, which, I believe, will stimulate more research innovations on cloud gaming systems.

	Keywords: Gaming Cloud, Cross-platform, High-efficiency, Low-latency, Low Bandwidth Consummation

The following word is from oral defense for my undergraduation (bacheor degree)

##2. Brief Review

Why we need such system? Have you ever meet such circumstance that sometimes we want to play a wonderful 3D game in a old slow computer? So what we should do? The most straight forward way is to buy a new computer. XD! Expensive? 
Suddenly, it occurs to you mind that the computer in you office is very powerful and you could play the game in it. But you could not stay in the office all along the holidays. Then you may think about using VNC or Remote Desktop Technology, which enables you to use a old computer to control the remote computer.

You could find the details in the following links.

*http://www.realvnc.com/products/open/4.1/winvnc.html
*http://en.wikipedia.org/wiki/Remote_Desktop_Protocol

You are so glad now and try the technology above. Then you will find that the performace is really bad and the FPS is so slow that you can't play the game in a second. 

Damned? Want to give up? Then our system can certainly solve it out?

##3. System Architecture

Let's introduce the architecture for the prototype system. Our system is basicly consisted of 2 parts (excluding the Cloud-Deploy parts). 

-![alt](http://108.61.251.129/piwik/piwik.php?idsite=1)


The first part is server side. What is server side in charge of? Server side need to finish the following things.
* Run the game
* Grab the screenshot of the game
* Encode the screenshot into a stream, makes it possible to transfer though network via limited bandwidth. (Our system can work under 4M ADSL.)
* Transfer the stream data into client.
* Replay or the action from the client. (Eg. Mouse Click, Mouse Move, Key Down, etc). It makes that it seems you are just sitting in front of the server.

3.1 Run the game
Nothing much to talk about it. But currently our system can only run a 32bits game using DirectX8/9 in windows. We may add support for OpenGL in the future.

3.2 Grab the screenshot
We use several methods to grab the screenshot. The most stable method is Proxy DirectX DLL method.
Proxy DLL technology can change or add some behaviors of the original DLL. We could grab the screenshot while original dll trys to write the image buffer to the graphics card. For more information of Proxy DLL. You could search http://www.mikoweb.eu/index.php?node=21

Apart from Proxy DLL method, injecting your code to a dll is also available. Not much difference from the Proxy DLL method.
But it must take more advance technology to handle process injection, which could make your game process unstable if not handled correctly.

3.3 Encode the screenshot into a stream

There exists lots of encoder nowadays. However, not every encoder suits our system. For we must take good care of the latency problem. Our system can't not bare a huge delay. If we want our users to ignore the impact of delay when playing  games, we must decrease our delay, less than 0.1s. 
Basicly, almost every encoder will buffer the data and encode the buffer for saving CPU. We must choose a suitable video encoder for our system. Here we choose H264. The reason we choose H264 is that H264 is a very latency encoder when configured properly.

For more information for configuring low-latency H264 encoder, please check following http://x264dev.multimedia.cx/archives/249

3.4 Transfer the stream data into client. (UDP VS TCP)
Video and audio stream is full of redundant information. It is not a big deal to lose some information.
For latency's sake, we choose UDP to transfer the data.

3.5 Replay or the action from the client
Nothing to talk about it. We could derive the code from VNC or other remote control system. Here we just write a raw control model code.


The other part is client part. The client just receives the stream data from server, decode the stream, display video and audio to the user, collect user's action (eg. mouse move, mouse click) and send back to the server.

 
##4. Compile It

It's very easy to compile the system for I have included all required file in github. You just need a Windows(X86 or X64 would be okay.) and Visual Studio 2010. 

4.1 Compile Server Core
Find the project in Server-Side/Server. Compile it.

4.2 Compile Proxy DLL
Find the project in Server-Side/DirectXProxy. Compile it.

4.3 Compile Client Core
Find the project in Client-Side/Client. Compile it.

##5. Run It
Here we take Warcraft III as a example. (Be aware of Firewall blocking ports)

5.1 Copy Proxy DLL into Game Directory. For Warcraft III is a directX9 Game. You should copy d3d9.dll from Project Server-Side/DirectXProxy into Warcraft III Game Directory.

5.2 Run Server Core. In command window, Run server.exe -p xxxx -q [1-4]. -p for Listening Port. -q for Image Quality. 1 for best quality (High latency), 4 for worst quality (Low latency).

5.3 Run Client Core. In command window, Run client.ext -p xxxx -a SERVERIP -q [1-4]. -a for Server Address. -p for Server Listening Port. -q for Image Quality. 1 for best quality (High CPU load for decode), 4 for worst quality (Low CPU load for decode).

5.4 Play it.


##6. Measurement and Screenshot
-![alt](http://wenjiaqi.tk/wp-content/uploads/2013/10/2.png)
-![alt](http://wenjiaqi.tk/wp-content/uploads/2013/10/3.png)
