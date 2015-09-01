#coding=UTF-8
__metaclass__=type
from xmlrpclib import ServerProxy,Fault
from os.path import join,isfile,abspath
from SimpleXMLRPCServer import SimpleXMLRPCServer
from urlparse import urlparse
import sys

SimpleXMLRPCServer.allow_reuse_address=1

MAX_HISTORY_LENGTH=6

UNHANDLED=100
ACCESS_DENIED=200

class UnhandledQuery(Fault):
	"""
	��ʾ�޷�����Ĳ�ѯ���쳣��
	"""
	def __init__(self,message="Couldn't handle the query"):
		Fault.__init__(self,UNHANDLED,message)
	
class AccessDenied(Fault):
	"""
	���û���ͼ����δ����Ȩ���ʵ���Դʱ�������쳣��
	"""
	def __init__(self,message="Access denied"):
		Fault.__init__(self,ACCESS_DENIED,message)
	
def inside(dir,name):
	"""
	��������Ŀ¼���Ƿ��и������ļ�����
	"""
	dir=abspath(dir)
	name=abspath(name)
	return name.startswith(join(dir,''))

def getPort(url):
	"""
	��URL����ȡ�˿ڡ�
	"""
	name=urlparse(url)[1]
	parts=name.split(':')
	return int(parts[-1])

class Node:
	"""
	P2P�����еĽڵ�
	"""
	def __init__(self,url,dirname,secret):
		self.url=url
		self.dirname=dirname
		self.secret=secret
		self.known=set()

	def query(self,query,history=[]):
		"""
		��ѯ�ļ������ܻ���������֪�ڵ�������������ļ���Ϊ�ַ������ء�
		"""
		try:
			return self._handle(query)
		except UnhandledQuery:
			print 'UnhandledQuery'
			history=history+[self.url]
			if len(history)>=MAX_HISTORY_LENGTH:
				raise
			return self._broadcast(query,history)
	
	def hello(self,other):
		"""
		���ڽ��ڵ���ܸ������ڵ�
		"""
		self.known.add(other)
		return 0

	def fetch(self,query,secret):
		"""
		�����ýڵ��ҵ��ļ������ء�
		"""
		if secret!=self.secret:
			print 'AccessDenied'
			raise AccessDenied
		result=self.query(query)
		f=open(join(self.dirname,query),'w')
		f.write(result)
		f.close()
		return 0
	
	def _start(self):
		"""
		�ڲ�ʹ�ã���������XML_RPC��������
		"""
		s=SimpleXMLRPCServer(("",getPort(self.url)),logRequests=False)
		s.register_instance(self)
		s.serve_forever()

	def _handle(self,query):
		"""
		�ڲ�ʹ�ã����ڴ�������
		"""
		dir=self.dirname
		name=join(dir,query)
		if not isfile(name):
			print 'UnhandledQuery'
			raise UnhandledQuery
		if not inside(dir,name):
			print 'AccessDenied'
			raise AccessDenied
		return open(name).read()

	def _broadcast(self,query,history):
		"""
		�ڲ�ʹ�ã����ڽ���ѯ�㲥��������֪Node��
		"""
		for other in self.known.copy():
			if other in history:
				continue
			try:
				s=ServerProxy(other)
				return s.query(query,history)
			except Fault,f:
				print f
				if f.faultCode==UNHANDLED:
					pass
				else:
					self.known.remove(other)
			except:
				print 'Unknown Error'
				self.known.remove(other)
		raise UnhandledQuery
def main():
	url,directory,secret=sys.argv[1:]
	n=Node(url,directory,secret)
	n._start()

if __name__=='__main__':
	main()
