
The project is a cloud gaming prototype system (or called remote gaming system).

The following is the quick guide for compliing and deploying the system. If you have any problem, feel free to send email
to jq.elliott.wen@gmail.com. If you want to contribute your code, you can just fork it or pull your request after sending mail to me. I would be glad if you join the development.

1. Brief Introduction 

	With the development of cloud computing technology, cloud applications become very popular in the recent years. Cloud gaming is a promising application of the rapidly developing cloud computing technology. As for traditional games, most calculations are normally performed in client side. It's necessary for users to set up a powerful machine and install huge application packages advanced. In contrast, most calculations are performed in remote servers with cloud gaming technology. Under this case, it's easy for users to play games without worrying about the performance of machine.

	Most of existing virtual desktop technology can meet the requirement of controlling remote computer in a low frame rate. However, such technology works poor in high frame rate condition when playing games. It not only consumes lots of bandwidth but also yields high latency. To sum up, the biggest obstacle for cloud gaming is to design an efficient system with high responsiveness and low bandwidth consummation.

	In this paper, we present a novel cloud gaming system which is full of extensibility, portability and configurability. Based on this system, it's easy to set up a private gaming cloud. As is known to all, most games only run in the Windows environment. So we implement the server part in Windows platform, while its client can be readily ported to other OS's, including Windows, Linux, Android and IOS. We also conduct extensive experiments to evaluate the performance of the system. My experimental results indicate that our system is efficient enough and can provide high responsiveness and video quality with low latency and low bandwidth consummation. The system yields a per-frame video delay of 34 milliseconds and audio delay of 10 milliseconds, which is tiny for human-being to notice. The system also provides several quality configuration including high definition, standard definition and low definition. My experiments also reveal that all these performance gains are achieved without the expense of higher network loads. Actually, common ADSL bandwidth can meet the requirement of my system. The proposed system can be employed by the researchers, game developers, service providers, and end users for setting up cloud gaming test beds, which, I believe, will stimulate more research innovations on cloud gaming systems.

	Keywords: Gaming Cloud, Cross-platform, High-efficiency, Low-latency, Low Bandwidth Consummation

The following word is from oral defense for my undergraduation (bacheor degree)

2. Brief Review

Why we need such system? Have you ever meet such circumstance that sometimes we want to play a wonderful 3D game in a old slow computer? So what we should do? The most straight forward way is to buy a new computer. XD! Expensive? 
Suddenly, it occurs to you mind that the computer in you office is very powerful and you could play the game in it. But you could not stay in the office all along the holidays. Then you may think about using VNC or Remote Desktop Technology, which enables you to use a old computer to control the remote computer.

You could find the details in the following links.

*http://www.realvnc.com/products/open/4.1/winvnc.html
*http://en.wikipedia.org/wiki/Remote_Desktop_Protocol

You are so glad now and try the technology above. Then you will find that the performace is really bad and the FPS is so slow that you can't play the game in a second. 

Damned? Want to give up? Then our system can certainly solve it out?

2. System Architecture

Let's introduce the architecture for the prototype system.

