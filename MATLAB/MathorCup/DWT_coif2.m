clear;
clc;
X=imread('ori.bmp');
X=rgb2gray(X);
figure(1);
subplot(121);
imshow(X);
title('ԭʼͼ��');
X=double(X);

XX=imread('img.bmp'); 
XX=rgb2gray(XX);
subplot(122);
imshow(uint8(XX));
title('����ͼ��');

 [c,l]=wavedec2(XX,2,'coif2'); 
 
n=[1,2];
p=[10.28,10.08]; 
 
nc_h=wthcoef2('h',c,l,n,p,'s');

X1=waverec2(nc_h,l,'coif2');
figure(2);
subplot(131);
imshow(uint8(X1));
title('��һ��������ͼ��'); 
 

nc_v=wthcoef2('v',nc_h,l,n,p,'s');

X2=waverec2(nc_v,l,'coif2');
subplot(132);
imshow(uint8(X2));
title('�ڶ���������ͼ��');
 

nc_d=wthcoef2('d',nc_v,l,n,p,'s');

X3=waverec2(nc_d,l,'coif2');
subplot(133);
imshow(uint8(X3));
title('������������ͼ��');
 
snr0=psnr(X,XX);
snr1=psnr(X,X1);
snr2=psnr(X,X2);
snr3=psnr(X,X3);