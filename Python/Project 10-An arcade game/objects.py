#coding=UTF-8
__metaclass__=type

import pygame,config,os
from random import randrange

"���ģ�����Squish����Ϸ����"

class SquishSprite(pygame.sprite.Sprite):
	"""
	Squish��������ͼ�εķ��ͳ��ࡣ���캯����������ͼ��������ͼ�ε�
	rect��area���ԣ�������������ָ�������ڽ����ƶ���area����Ļ�Ĵ�С
	�����׾�����
	"""

	def __init__(self,image):
		pygame.sprite.Sprite.__init__(self)
		self.image=pygame.image.load(image)
		self.rect=self.image.get_rect()
		screen=pygame.display.get_surface()
		shrink=-config.margin*2
		self.area=screen.get_rect().inflate(shrink,shrink)

class Weight(SquishSprite):
	"""
	���µĳ��ȡ���ʹ����SquishSprite���캯���������ĳ���ͼ�񣬲��һ�
	�Ը������ٶ���Ϊ���캯���Ĳ���������������ٶȡ�
	"""

	def __init__(self,speed):
		SquishSprite.__init__(self,config.Weight_image)
		self.speed=speed
		self.reset()

	def reset(self):
		"""
		�������ƶ�����Ļ����(������),���õ�����ˮƽλ���ϡ�
		"""
		x=randrange(self.area.left,self.area.right)
		self.rect.midbottom=x,0
	
	def update(self):
		"""
		���������ٶȽ����ȴ�ֱ�ƶ�(����)һ�ξ��롣���Ҹ������Ƿ񴥼�
		��Ļ�׶�������landed���ԡ�
		"""
		self.rect.top+=self.speed
		self.landed=self.rect.top>=self.area.bottom

class Banana(SquishSprite):
	"""
	�������㽶����ʹ��SquishSprite���캯�������㽶��ͼ�񣬲��һ�ͣ��
	����Ļ�׶ˡ�����ˮƽλ���ɵ�ǰ�����λ��(��һ������)������
	"""

	def __init__(self):
		SquishSprite.__init__(self,config.Banana_image)
		self.rect.bottom=self.area.bottom
		#��û���㽶�Ĳ��ֽ�����䡣
		#��������ƶ�������Щ����������
		#���ж�Ϊ��ײ������˵�ǽ��㽶ѹ�⣩:
		self.pad_top=config.Banana_pad_top
		self.pad_side=config.Banana_pad_side
	
	def update(self):
		"""
		��Banana���ĵ�ĺ������趨Ϊ��ǰ���ָ��ĺ����꣬����ʹ��rect
		��clamp����ȷ��Bananaͣ����������ķ�Χ�ڡ�
		"""
		self.rect.centerx=pygame.mouse.get_pos()[0]
		self.rect=self.rect.clamp(self.area)

	def touches(self,other):
		"""
		ȷ���㽶�Ƿ��������������ͼ��(�������)������ʹ��rect��coll-
		iderect�����⣬����Ҫ����һ���������㽶ͼ�񶥶˺Ͳ��"������"��
		�¾���(ʹ��rect��inflate�����Զ��˺Ͳ�߽������)��
		"""
		#ʹ���ʵ��������С�߽�:
		bounds=self.rect.inflate(-self.pad_side,-self.pad_top)
		#�ƶ��߽磬�����Ƿ��õ�Banana�ĵײ���
		bounds.bottom=self.rect.bottom
		#���߽��Ƿ�����������rect���档
		return bounds.colliderect(other.rect)

