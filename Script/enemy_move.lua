function GetNewPos (x,y,mode)
if mode==-1 then
return x..","..y
elseif mode==0 then--ģʽ0Ϊֱ�������ƶ�
if x<=0 then
x=100
end
if x>=600 then
x=560
end
y=y+2
return x..","..y
elseif mode==1 then--ģʽ1Ϊб���������˶�
if y <=100 then
x=x+1
y=y+4
elseif y>100 and y<200 then
x=x-1
y=y+1
elseif y>=200 and y<300 then
x=x+1
y=y+1
elseif y>=300 then
y=y+4
end
return x..","..y
elseif mode==2 then--ģʽ2Ϊб���������˶�
x=x+1
y=y+1
return x..","..y
elseif mode==3 then--ģʽ3Ϊб���������������˶�
x=x+y*0.01
y=y+1
return x..","..y
elseif mode==4 then--ģʽ4Ϊ����ֱ�������ƶ���ʽ
if y <=100 then
x=x+1
y=y+2
elseif y>100 and y<200 then
x=x-1
y=y+2
elseif y>=200 and y<230 then
x=x+1
y=y+2
elseif y>=230 then
x=x-1
y=y+2
end
return x..","..y
elseif mode==5 then--ģʽ5Ϊ��������λ���ƶ���ʽ(��һ�㵥λҪ��)
if y <=100 then
x=x+2
y=y+4
elseif y>100 and y<200 then
x=x-2
y=y+2
elseif y>=200 and y<300 then
x=x+2
y=y+2
elseif y>=300 then
y=y+4
end
return x..","..y
elseif mode==6 then--ģʽ6Ϊֱ�������ƶ�
if x<=0 then
x=100
end
if x>=600 then
x=560
end
y=y+8
return x..","..y
elseif mode==7 then--ģʽ7Ϊֱ�������ƶ���for���ƶ�״̬����
x=x-2
y=y+2
return x..","..y
elseif mode==8 then--ģʽ8Ϊֱ�������ƶ���for���ƶ�״̬����
x=x+2
y=y+2
return x..","..y
elseif mode==9 then--ģʽ9Ϊֱ�������ƶ���for���ƶ�״̬����
x=x+1
y=y+3
return x..","..y
elseif mode==10 then--ģʽ10Ϊֱ�������ƶ���for���ƶ�״̬����
x=x-1
y=y+3
return x..","..y
elseif mode==11 then--ģʽ11Ϊbossֱ�������ƶ���for���ƶ�״̬����
x=x-4
return x..","..y
elseif mode==12 then--ģʽ12Ϊbossֱ�������ƶ���for���ƶ�״̬����
x=x+4
return x..","..y
elseif mode==13 then--̹�����ң����ŵ�ͼ��ȥ
x=x+1
y=y+3
return x..","..y
elseif mode==14 then--̹���������ŵ�ͼ��ȥ
x=x-1
y=y+3
return x..","..y
elseif mode==99 then--���������ŵ�ͼ��ȥ
x=x
y=y+3
return x..","..y
--���Ҳ�����ȥʵ���ˣ���״̬���жϾͿ���ʵ�֣�
else
return x..","..y
end
end;
