#coding=UTF-8
__metaclass__=type
from urllib import urlopen
import re
class Agent:
	"""
	���Դ�������Դ��ȡ������Ŀ���ҷ���������Ŀ��Ķ���
	"""
	def __init__(self):
		self.sources=[]
		self.destinations=[]
	
	def addSource(self,source):
		self.sources.append(source)
	
	def addDestination(self,dest):
		self.destinations.append(dest)

	def distribute(self):
		"""
		��������Դ��ȡ����������Ŀ���ҷ���������Ŀ�ꡣ
		"""
		items=[]
		for source in self.sources:
			items.extend(source.getItems())
		for dest in self.destinations:
			dest.receiveItems(items)	

class NewsItem:
	"""
	��������������ı���ʱ����Դ�ļ�������Ŀ��
	"""
	def __init__(self,title,body):
		self.title=title
		self.body=body
