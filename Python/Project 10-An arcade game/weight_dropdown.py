#coding=UTF-8
__metaclass__=type
import sys,pygame
from pygame.locals import *
from random import randrange
from time import sleep
class Weight(pygame.sprite.Sprite):
	
	def __init__(self):
		pygame.sprite.Sprite.__init__(self)
		#�ڻ�spriteʱʹ�õ�ͼ��;���
		self.image=weight_image
		self.rect=self.image.get_rect()
		self.reset()
	
	def reset(self):
		"""
		�������ƶ�����Ļ���˵����λ�á�
		"""
		self.rect.top=-self.rect.height
		self.rect.centerx=randrange(self.rect.width,
				screen_size[0]-self.rect.width)
		self.speed=0

	def update(self):
		"""
		���³��ȣ���ʾ��һ֡��
		"""
		self.speed+=0.1
		self.rect.top+=self.speed

		if self.rect.top>screen_size[1]:
			self.reset()

#��ʼ��
pygame.init()
screen_size=1366,768
pygame.display.set_mode(screen_size,FULLSCREEN)
pygame.mouse.set_visible(0)

#������ȵ�ͼ��
weight_image=pygame.image.load('Weight1.png')
weight_iamge=weight_image.convert()
#����һ����ͼ����(sprite group),����weight
sprites=pygame.sprite.RenderUpdates()
sprites.add(Weight())

#��ȡ��Ļ���棬�������
screen=pygame.display.get_surface()
bg=(255,255,255)
screen.fill(bg)
pygame.display.flip()

#���������ͼ��
def clear_callback(surf,rect):
	surf.fill(bg,rect)

while True:
	#����˳��¼�
	for event in pygame.event.get():
		if event.type==QUIT:
			sys.exit()
		if event.type==KEYDOWN and event.key==K_ESCAPE:
			sys.exit()
	sleep(0.01)
	#���ǰ���λ��
	sprites.clear(screen,clear_callback)
	#����������ͼ��
	sprites.update()
	#����������ͼ��
	updates=sprites.draw(screen)
	#�����������ʾ����
	pygame.display.update(updates)



