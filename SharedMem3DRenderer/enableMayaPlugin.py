#! /usr/bin/env python
# -*- coding: utf-8 -*-
# fileName : commandPort.py
import socket
#HOST = '192.168.1.122'	# The remote host
HOST = '127.0.0.1'	# the local host
PORT = 54321	# The same port as used by the server
ADDR=(HOST,PORT)


#EXECUTE THIS IN MAYA FIRST commandPort -n ":54321";

#TO UNLOAD USE
#unloadPluginWithCheck( "C:/Program Files/Autodesk/Maya2016/bin/plug-ins/Custom Plugins/MayaPlugin Project_ MartinClementson.mll" );


#To use : write this in visual studio : c:\Python27\python.exe C:\Windows\Temp\file.py
def SendCommand():
	client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	client.connect(ADDR)
	command = 'unloadPluginWithCheck( "C:/Program Files/Autodesk/Maya2016/bin/plug-ins/Custom Plugins/MayaPlugin Project_ MartinClementson.mll" );'	# the commang from external editor to maya

	MyMessage = command
	client.send(MyMessage)
	data = client.recv(1024)	#receive the result info
	client.close()

	print 'The Result is %s'%data


if __name__=='__main__':
	SendCommand()