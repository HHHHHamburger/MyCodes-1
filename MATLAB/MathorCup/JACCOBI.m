function [V,D,iter]=eigcj(A,tol,itermax)
% �ŵ�Jaccobi��������ʵ�Գƾ���A������ֵ������
% (eigen value and vector of classical Jaccobi algorithm)
%
% ����˵��
% A��ʵ�Գƾ���
% tol�����ȣ��ǶԽ�Ԫ��ƽ����
% itermax������������
% V������ֵ
% D����������
% iter��ʵ�ʵ�������
%
% ʵ��˵��
% A=[-12 3 3;3 1 -2;3 -2 7];
% [V,D,iter]=eigcj(A,1e-10,20)
% [V2,D2]=eig(A) % ʹ��MATLAB�Դ�eig����������֤
%
% ����֪ʶ
% 1������ʵ�Գƾ���A���ŵ��ſ˱��㷨�ض����������ȫ��������
% 2��ʵ�Գƾ��������ֵ��Ϊʵ�����Ҵ��ڱ�׼��������������ϵ
% 3��������ʵ�Գƾ���A���ܴ�����������Q��ʹ��QAQ'=diag(��1,��2,...)
%    ��ΪA����������Q���ж�Ӧ����������
% 4����AΪʵ�Գƾ���QΪ����������||A||=||QA||=||AQ||=||QAQ'||=������ƽ����
%    ����||X||��ʾX��F����
%
% �㷨˵��
% 0������ֵH=I��A0=A
% 1��ѡ�ǶԽ�����Ԫ��|a(p,q)|=max(|a(i,j)|)
% 2������Givens����R
% 3������Ԫ��a(p,q)����Givens�任
% 3.1 ����H=R*H
% 3.2 ����A=H*A0*H'
% 4������SA������A�ķǶԽ�Ԫ��ƽ����
% 5��SA�Ƿ񵽴�ָ�����ȣ��Ƿ�ﵽ����������
% 6���񣬼���ѭ�����ǣ��˳�
%
% �ο�����
% �⽨������ֵ����ԭ��.2005��7��.��ѧ������.Page238-243
%
[m,n]=size(A);
H=eye(m,n);
A0=A;
SA=sum(A(:).^2)-sum(diag(A).^2);
iter=0;
while SA>tol && iter<itermax
    iter=iter+1;
    % ѡ��Ԫ��a(p,q)
    T=triu(A,1);
    [tmp,idx]=max(abs(T(:)));
    % ����Givens�����е�cos��sinֵ
    [p,q]=ind2sub([m,n],idx);
    % ����ǶԽ�Ԫ��ƽ����SA
    SA=SA-2*A(p,q).^2;
    if A(p,p)==A(q,q)
        cos=sqrt(2)/2;
        sin=sign(A(p,q))*cos;
    else
        d=(A(p,p)-A(q,q))/(2*A(p,q));
        tan=sign(d)/(abs(d)+sqrt(d^2+1));
        cos=1/sqrt(1+tan^2);
        sin=tan*cos;
    end
    % ����Givens����
    R=eye(m,n);
    R(p,p)=cos;
    R(p,q)=sin;
    R(q,p)=-sin;
    R(q,q)=cos;
    % ����H����
    H=R*H;
    % ����A����
    A=H*A0*H';
end
V=diag(A); % ������
D=H'; % ��������