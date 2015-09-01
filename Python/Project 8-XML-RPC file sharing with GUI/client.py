#coding=UTF-8
__metaclass__=type
from xmlrpclib import ServerProxy,Fault
from cmd import Cmd
from random import choice
from string import lowercase
from server import Node,UNHANDLED
from threading import Thread
from time import sleep
import sys

HEAD_START=0.1 #Seconds
SECRET_LENGTH=100

def randomString(length):
	"""
	���ظ������ȵ�����ĸ��ɵ�����ַ�����
	"""
	chars=[]
	letters=lowercase[:26]
	while length>0:
		length-=1
		chars.append(choice(letters))
	return ''.join(chars)

class Client(Cmd):
	"""
	Node��ļ򵥵Ļ����ı��Ľ��档
	"""

	prompt='> '
	
	def __init__(self,url,dirname,urlfile):
		"""
		�趨url��dirname��urlfile,�����ڵ������߳�������Node��������
		"""
		Cmd.__init__(self)
		self.secret=randomString(SECRET_LENGTH)
		n=Node(url,dirname,self.secret)
		t=Thread(target=n._start)
		t.setDaemon(1)
		t.start()
		#�÷�������������
		sleep(HEAD_START)
		self.server=ServerProxy(url)
		for line in open(urlfile):
			line=line.strip()
			self.server.hello(line)
	def do_fetch(self,arg):
		"""
		���÷�������fetch������
		"""
		try:
			self.server.fetch(arg,self.secret)
		except Fault,f:
			if f.faultCode!=UNHANDLED:
				raise
			print "Couldn't find the file",arg
	
	def do_exit(self,arg):
		"""
		�˳�����
		"""
		print 
		sys.exit()
	
	do_EOF=do_exit #EOF��'exit'ͬ�塣

def main():
	urlfile,directory,url=sys.argv[1:]
	client=Client(url,directory,urlfile)
	client.cmdloop()

if __name__=='__main__':
	main()

