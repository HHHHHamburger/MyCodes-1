
%���� db4������С���ֽ⣬�������������϶�С��ϵ������Ӳ��ֵ������
%Ϊ�˷����������ֵ��ѡ������ֶ��趨�ķ�����
%����ͼ�����С���ع������沿�ֻ�����ع���Ч��ͼ����Ӧ�ķ�ֵ����ȡ�
clear;
clc;
%X=imread('ori.bmp');
%X=rgb2gray(X);
%figure(1);
%subplot(121);
%imshow(X);
%title('ԭʼͼ��');
%X=double(X);

%XX=imread('img.bmp'); 
%XX=rgb2gray(XX);
%subplot(122);
%imshow(uint8(XX));
%title('����ͼ��');
READ

[c,l]=wavedec2(img,2,'db4'); 

n=[1,2];% detail levels
p=[10.28,10.08]; % corresponding thresholds
%N and T must be of the same length. The vector N must be such that 1 �� N(i) �� size(l,1)-2.
figure(2);

nc_h=wthcoef2('h',c,l,n,p,'h');

X1=waverec2(nc_h,l,'db4');
subplot(131);
imshow(uint8(X1));
title('��һ��������ͼ��'); 
 

nc_v=wthcoef2('v',nc_h,l,n,p,'h');

X2=waverec2(nc_v,l,'db4');
subplot(132);
imshow(uint8(X2));
title('�ڶ���������ͼ��');
 

nc_d=wthcoef2('d',nc_v,l,n,p,'h');

X3=waverec2(nc_d,l,'db4');
%subplot(133);
figure
imshow(uint8(X3));
title('DWT(db4С��)����');
 
snrIMG=snr(ori,img);
snrAFF1=snr(ori,X1);
snrAFF2=snr(ori,X2);
snrAFF3=snr(ori,X3);