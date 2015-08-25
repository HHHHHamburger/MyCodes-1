#coding=UTF-8
__metaclass__=type
from nntplib import NNTP
from time import strftime,time,localtime
from email import message_from_string
from urllib import urlopen
import textwrap
import re


day=24*60*60

def wrap(string,max=70):
	"""
	���ַ�������Ϊ����п�
	"""
	return '\n'.join(textwrap.wrap(string))+'\n'

class NewsAgent:
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
	��������������ı��ļ�������Ŀ��
	"""
	def __init__(self,title,body):
		self.title=title
		self.body=body

class NNTPSource:
	"""
	��NNTP���л�ȡ������Ŀ��������Դ��
	"""
	def __init__(self,servername,group,window):
		self.servername=servername
		self.group=group
		self.window=window

	def getItems(self):
		start=localtime(time()-day*self.window)
		date=strftime('%y%m%d',start)
		hour=strftime('%H%M%S',start)

		server=NNTP(self.servername)


		ids=server.newnews(self.group,date,hour)[1]

		for id in ids:
			lines=server.article(id)[3]
			message=message_from_string('\n'.join(lines))

			title=message['subject']
			body=message.get_payload()
			if message.is_multipart():
				body=body[0]

			yield NewsItem(title,body)

		server.quit()

class SimpleWebSource:
	"""
	ʹ��������ʽ����ҳ����ȡ������Ŀ��������Դ��
	"""
	def __init__(self,url,Pattern,contentPattern):
		self.url=url
		self.Pattern=re.compile(Pattern)
		self.contentPattern=re.compile(contentPattern)
	
	def getItems(self):
		text=urlopen(self.url).read()
		results=self.Pattern.findall(text)
		titlePattern=re.compile(r'href="(.*?)">(.*?)</a>')
		results=titlePattern.findall(''.join(results))
		titles=[s[1] for s in results]
		links=[s[0] for s in results]
		bodies=[]
		for link in links:
			print 'Read Link:%s' % (link,)
			content=urlopen(link).read()
			lines=self.contentPattern.findall(content)
			lines=['<p>    '+line[3:] for line in lines]
			bodies.append(''+''.join(lines))
		for title,body in zip(titles,bodies):
			yield NewsItem(title,body)

class PlainDestination:
	"""
	������������Ŀ��ʽ��Ϊ���ı�������Ŀ���ࡣ
	"""
	def receiveItems(self,items):
		for item in items:
			print item.title
			print '-'*len(item.title)
			print item.body

class HTMLDestination:
	"""
	������������Ŀ��ʽ��ΪHTML��Ŀ���ࡣ
	"""
	def __init__(self,filename):
		self.filename=filename
	
	def receiveItems(self,items):

		out=open(self.filename,'w')
		print >>out,"""
		<html>
		 <head>
		  <title>Today's News</title>
		 </head>
		 <body>
		 <h1>Today's News</h1>
		"""

		print >>out,'<ul>'
		id=0
		for item in items:
			id+=1
			print >>out,'<li><a href="#%i">%s</a></li>' % (id,item.title)
		print >>out,'</ul>'

		id=0
		for item in items:
			id+=1
			print >>out,'<h2><a name="%i">%s</a></h2>' % (id,item.title)
			print >>out,'    '+item.body

		print >>out,"""
		 </body>
		</html>
		"""

def runDefaultSetup():
	"""
	��Դ��Ŀ���Ĭ�����ã������Լ��޸ġ�
	"""

	agent=NewsAgent()

	#��������վ��ȡ���ŵ�SimpleWebSource:
	url='http://news.163.com/'
	titleandlinksPattern=r'<!-- ͷ���� -->[\s\S]*?<!-- �б��� -->'
	contentPattern=r'(<p>.*?</p>)'
	bbc=SimpleWebSource(url,titleandlinksPattern,contentPattern)

	agent.addSource(bbc)
	
	#��comp.lang.python.announce��ȡ���ŵ�NNTPSource:
	#�����Ҳ��������������´�����ʱע��

	#clpa_server='news.foo.bar'
	#clpa_group='comp.lang.python.announce'
	#clpa_window=1
	#clpa=NNTPSource(clpa_server,clpa_group,clpa_window)

	#agent.addSource(clpa)
	
	#���Ӵ��ı�Ŀ���HTMLĿ��
	agent.addDestination(PlainDestination())
	agent.addDestination(HTMLDestination('news.html'))

	#����������Ŀ

	agent.distribute()

if __name__=='__main__':
	runDefaultSetup()
