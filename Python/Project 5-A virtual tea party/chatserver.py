#coding=UTF-8
__metaclass__=type
from asyncore import dispatcher
from asynchat import async_chat
import socket,asyncore

PORT=5005
NAME='David\' Chat Room'

class EndSession(Exception):
	pass

class CommandHandler:
	"""
	�����ڱ�׼����cmd.Cmd�ļ���������
	"""

	def unknown(self,session,cmd):
		'��Ӧδ֪����'
		session.push('Unkown command:%s\r\n' % cmd)

	def handle(self,session,line):
		'����Ӹ����ĻỰ�н��յ�����'
		if not line.strip():
			return
		#��������:
		parts=line.split(' ',1)
		cmd=parts[0]
		try:
			line=parts[1].strip()
		except IndexError:
			line=''
		#���Ų��Ҵ������:
		meth=getattr(self,'do_'+cmd,None)
		try:
			#�ٶ����ǿɵ��õ�:
			meth(session,line)
		except TypeError:
			#��������Ա����ã��˶δ�����Ӧδ֪����
			self.unknown(session,cmd)

class Room(CommandHandler):
	"""
	����һ�������û�(�Ự)�ķ��ͻ���������������������͹㲥
	"""

	def __init__(self,server):
		self.server=server
		self.sessions=[]

	def add(self,session):
		'һ���Ự(�û�)�ѽ��뷿��'
		self.sessions.append(session)

	def remove(self,session):
		'һ���Ự(�û�)���뿪����'
		self.sessions.remove(session)

	def broadcast(self,line):
		'�򷿼��е����лỰ����һ��'
		for session in self.sessions:
			session.push(line)

	def do_logout(self,session,line):
		'��Ӧlogout����'
		raise EndSession

class LoginRoom(Room):
	"""
	Ϊ�ո������ϵ��û�׼���ķ���
	"""
	def add(self,session):
		Room.add(self,session)
		#���û�����ʱ�������ʺ�
		self.broadcast('Welcome to %s\r\n' % self.server.name)

	def unknown(self,session,cmd):
		#����δ֪���� (����login����logout���һ��)
		#�ᵼ��һ������:
		session.push('Please log in\nUse "login <nick>"\r\n')

	def do_login(self,session,line):
		name=line.strip()
		#ȷ���û�����������:
		if not name:
			session.push('Please enter a name\r\n')
		#ȷ���û���û�б�ʹ��:
		elif name in self.server.users:
			session.push('The name "%s" is taken.\r\n' % name)
			session.push('Please try again.\r\n')
		else:
			#����û���⣬���Դ洢�ڻỰ�У�����
			#���û��ƶ������������
			session.name=name
			session.enter(self.server.main_room)

class ChatRoom(Room):
	"""
	Ϊ���û��໥����׼���ķ���
	"""

	def add(self,session):
		#���������������û�����
		self.broadcast(session.name+' has entered the room.\r\n')
		self.server.users[session.name]=session
		Room.add(self,session)
	
	def remove(self,session):
		#�������������û��뿪
		Room.remove(self,session)
		self.broadcast(session.name+' has left the room.\r\n')
	
	def do_say(self,session,line):
		self.broadcast(session.name+': '+line+'\r\n')
	
	def do_look(self,session,line):
		'����look�����������ڲ鿴˭�ڷ�����'
		session.push('The following are in this room:\r\n')
		for other in self.sessions:
			session.push(other.name+'\r\n')

	def do_who(self,session,line):
		'����who�����������ڲ鿴˭��¼��'
		session.push('The following are logged:\r\n')
		for name in self.server.users:
			session.push(name+'\r\n')

	def do_help(self,session,line):
		'����help���������������ӡ������Ϣ'
		session.push('*'*15+'\r\n')
		session.push('Here is the help information:\r\n')
		session.push('Use "say <line>" to chat.\r\n')
		session.push('Use "who" to find logged users.\r\n')
		session.push('Use "look" to find users in this room.\r\n')
		session.push('Use "logout" to logout the chatroom.\r\n')
		session.push('*'*15+'\r\n')

class LogoutRoom(Room):
	"""
	Ϊ���û�׼���ļ򵥷��䡣ֻ���ڽ��û����ӷ������Ƴ���
	"""

	def add(self,session):
		#���Ự(�û�)����Ҫɾ����LogoutRoomʱ
		try:
			del self.server.users[session.name]
		except KeyError:
			pass

class ChatSession(async_chat):
	"""
	���Ự������͵��û�ͨ�š�
	"""

	def __init__(self,server,sock):
		async_chat.__init__(self,sock)
		self.server=server
		self.set_terminator("\r\n")
		self.data=[]
		self.name=None
		#���еĻỰ����ʼ�ڵ�����LoginRoom��:
		self.enter(LoginRoom(server))

	def enter(self,room):
		#�ӵ�ǰ�����Ƴ�����(self),���ҽ�������ӵ�
		#��һ������...
		try:
			cur=self.room
		except AttributeError:
			pass
		else:
			cur.remove(self)
		self.room=room
		room.add(self)
	
	def collect_incoming_data(self,data):
		self.data.append(data)

	def found_terminator(self):
		line=''.join(self.data)
		self.data=[]
		try:
			self.room.handle(self,line)
		except EndSession:
			self.handle_close()
	
	def handle_close(self):
		async_chat.handle_close(self)
		self.enter(LogoutRoom(self.server))

class ChatServer(dispatcher):
	"""
	ֻ��һ������������������
	"""
	
	def __init__(self,port,name):
		dispatcher.__init__(self)
		self.create_socket(socket.AF_INET,socket.SOCK_STREAM)
		self.set_reuse_addr()
		self.bind(('',port))
		self.listen(5)
		self.name=name
		self.users={}
		self.main_room=ChatRoom(self)

	def handle_accept(self):
		conn,addr=self.accept()
		print 'Get connection:%s:%s' %(addr[0],addr[1])
		ChatSession(self,conn)

if __name__=='__main__':
	s=ChatServer(PORT,NAME)
	try:
		asyncore.loop()
	except KeyboardInterrupt:
		print

