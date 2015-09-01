#coding=UTF-8
__metaclass__=type

import os,sys,pygame
from pygame.locals import *
import objects,config
from time import sleep
"���ģ�����Squish��Ϸ����Ҫ��Ϸ�߼�"

class State:
	"""
	������Ϸ״̬�࣬���Դ����¼����ڸ����ı�������ʾ����
	"""
	def handle(self,event):
		"""
		ֻ�����˳��¼���Ĭ���¼�����
		"""
		if event.type==QUIT:
			sys.exit()
		if event.type==KEYDOWN and event.key==K_ESCAPE:
			sys.exit()

	def firstDisplay(self,screen):
		"""
		���ڵ�һ����ʾ״̬��ʹ�ñ���ɫ�����Ļ��
		"""
		screen.fill(config.Background_color)
		pygame.display.flip()

	def display(self,screen):
		"""
		�������Ѿ���ʾ��һ��״̬���ٴ���ʾ��Ĭ�ϵ���Ϊ��ʲô��������
		"""
		pass

class Level(State):
	"""
	��Ϸ�ȼ������ڼ����Ѿ������˶��ٳ��ȣ��ƶ���ͼ���Լ���������Ϸ
	�߼���ص�����
	"""

	def __init__(self,number=1):
		self.number=number
		#�����ڻ�Ҫ���¶��ٳ��ȣ�
		self.remaining=config.Weights_per_level

		speed=config.Drop_speed
		#Ϊÿ������1�ĵȼ�������һ��Speed_increase
		speed+=(self.number-1)*config.Speed_increase
		#�������Ⱥ��㽶
		self.weight=objects.Weight(speed)
		self.banana=objects.Banana()
		both=self.weight,self.banana
		self.sprites=pygame.sprite.RenderUpdates(both)

	def update(self,game):
		"��ǰһ֡������Ϸ״̬"
		#����������ͼ��:
		self.sprites.update()
		#����㽶�����˳��ȣ���ô������Ϸ�л���GameOver״̬:
		if self.banana.touches(self.weight):
			game.nextState=GameOver()
		#�����ڳ������ʱ���临λ
		#������ص����г��ȶ������ˣ�������Ϸ�л���LevelCleared״̬
		elif self.weight.landed:
			self.weight.reset()
			self.remaining-=1
			if self.remaining==0:
				game.nextState=LevelCleared(self.number)

	def display(self,screen):
		"""
		�ڵ�һ����ʾ(ֻ�����Ļ)����ʾ״̬����firstDisplay��ͬ�����
		����ʹ��pygame.display.update��self.sprites.draw�ṩ�ġ���Ҫ
		���µľ����б���и��¡�
		"""
		screen.fill(config.Background_color)
		updates=self.sprites.draw(screen)
		pygame.display.update(updates)

class Paused(State):
	"""
	�򵥵���Ϸ��ͣ״̬�����¼�������������ߵ����궼��������״̬��
	"""

	finished=0 #�û���ͣ��������
	image=None #�����ҪͼƬ�Ļ����������������Ϊ�ļ���
	text='' #�����趨ΪһЩ��ʾ���ı�

	def handle(self,event):
		"""
		ͨ����State����ί��(һ�㴦���˳��¼�)�Լ��԰��������������
		��Ӧ�������¼�������������»�����걻�������self.finished��
		��Ϊ�档
		"""
		State.handle(self,event)
		if event.type in [MOUSEBUTTONDOWN,KEYDOWN]:
			self.finished=1
	
	def update(self,game):
		"""
		���µȼ���������������»�����걻���(����self.finishedΪ��),
		��ô������Ϸ�л�����һ����self.nextState()��ʾ��״̬(Ӧ��������
		ʵ��)��
		"""
		if self.finished:
			game.nextState=self.nextState()

	def firstDisplay(self,screen):
		"""
		��ͣ״̬��һ�γ���ʱ������ͼ��(����еĻ�)���������ı���
		"""
		#���ȣ�ʹ����䱳��ɫ�ķ�ʽ�����Ļ:
		screen.fill(config.Background_color)
		#ʹ��Ĭ�ϵ���ۺ�ָ���Ĵ�С����Font����
		font=pygame.font.Font('STXIHEI.ttf',config.font_size)
		#��ȡself.text�е��ı��У����Կ�ͷ�ͽ�β�Ŀ���:
		lines=self.text.strip().splitlines()
		#�����ı��ĸ߶�(ʹ��font.get_linesize())�Ի�ȡÿ���ı��ĸ߶�
		height=len(lines)*font.get_linesize()
		#�����ı��ķ���λ��(��Ļ����):
		center,top=screen.get_rect().center
		top-=height//2
		#�����ͼƬҪ��ʾ�Ļ�...
		if self.image:
			#����ͼƬ
			image=pygame.image.load(self.image)
			#��ȡ����rect
			r=image.get_rect()
			#��ͼ�������ƶ���߶ȵ�һ��ľ���
			top+=r.height//2
			#��ͼƬ�������ı��Ϸ�20���ش�
			r.midbottom=center,top-20
			#��ͼ���ƶ�����Ļ��
			screen.blit(image,r)

		antialias=1
		black=0,0,0

		#���������У��Ӽ������top��ʼ������
		#����ÿһ�������ƶ�font.get_linesize()����
		for line in lines:
			text=font.render(line.strip(),antialias,black)
			r=text.get_rect()
			r.midtop=center,top
			screen.blit(text,r)
			top+=font.get_linesize()

		#��ʾ���иı�
		pygame.display.flip()

class Info(Paused):
	"""
	�򵥵���ͣ״̬����ʾ�й���Ϸ����Ϣ����Level״̬����ʾ(��һ��)
	"""
	
	nextState=Level
	text='''
	In this game you are a banana,
	trying to survive a course in
	self-denfense against fruit,where
	the paricipants will "defend"
	themselves against you with a 
	16 kg weight.'''

class StartUp(Paused):
	"""
	��ʾչʾͼƬ�ͻ�ӭ��Ϣ����ͣ״̬����Info״̬ǰ��ʾ��
	"""
	nextState=Info
	image=config.Splash_image
	text='''
	Welcome to Squish,
	the game of Fruit Self-Defense'''

class LevelCleared(Paused):
	"""
	��ʾ�û����ص���ͣ״̬����next level״̬����ʾ��
	"""

	def __init__(self,number):
		self.number=number
		self.text='''Level %d cleared
		Click to start next level''' % self.number

	def nextState(self):
		return Level(self.number+1)

class GameOver(Paused):
	"""
	��ʾ�û������Ϸ��״̬����First level����ʾ��
	"""

	nextState=Level
	text='''
	Game Over
	Click to Restart,Esc to Quit
	'''

class Game:
	"""
	������ʱ��ѭ������Ϸ������������ڲ�ͬ״̬���л���
	"""

	def __init__(self,*args):
		#��ȡ��Ϸ��ͼ����õ�Ŀ¼:
		path=os.path.abspath(args[0])
		dir=os.path.split(path)[0]
		#�л����Ǹ�Ŀ¼(����ͼƬ�ļ�����������)
		os.chdir(dir)
		#��״̬��ʽ����
		self.state=None
		#�ڵ�һʱ��ѭ���������ƶ���StartUp
		self.nextState=StartUp()
	
	def run(self):
		"""
		���������̬�趨����������һЩ��Ҫ�ĳ�ʼ�����������ҽ�����
		�¼�ѭ����
		"""
		pygame.init() #��ʼ������pygameģ��
		#�����Դ���ģʽ����ȫ��ģʽ��ʾ��Ϸ
		flag=0 #Ĭ�ϴ���
		if config.full_screen:
			flag=FULLSCREEN #ȫ��ģʽ
		screen_size=config.Screen_size
		screen=pygame.display.set_mode(screen_size,flag)

		pygame.display.set_caption('Fruit Self Defense')
		pygame.mouse.set_visible(False)

		#��ѭ��
		while True:
			#(1)���nextState���޸��ˣ���ô�ƶ�����״̬��������ʾ��
			#(��һ��)
			if self.state!=self.nextState:
				self.state=self.nextState
				self.state.firstDisplay(screen)
			#(2)����ǰ״̬���¼�����
			for event in pygame.event.get():
				self.state.handle(event)
			#(3)���µ�ǰ״̬
			self.state.update(self)
			#(4)��ʾ��ǰ״̬
			self.state.display(screen)

if __name__=='__main__':
	game=Game(*sys.argv)
	game.run()

