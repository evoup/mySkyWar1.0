--function GetNewPos (x,y,mode)
--if mode==0 then--ģʽ0Ϊֱ�������ƶ�
--x=x
--y=y+1
--return x..","..y
--elseif mode==1 then--ģʽ1Ϊб���������˶�
--if y <=100 then
--x=x+1
--y=y+4
--elseif y>100 and y<200 then
--x=x-1
--y=y+1
--elseif y>=200 and y<300 then
--x=x+1
--y=y+1
--elseif y>=300 then
--y=y+4
--end
--return x..","..y
--elseif mode==2 then--ģʽ2Ϊб���������˶�
--x=x+3
--y=y+3
--return x..","..y
--else
--return x..","..y
--end
--end

function GetNewAnglePos(x,y,speed,mode,angle)
if mode==0 then--ģʽ0Ϊ���Ƕ������ƶ�
--local file=io.open("lualog.txt","w")
--file:write("x:"..x.."\r\ny:"..y.."\r\nspeed:"..speed.."\r\nmode:"..mode.."\r\nangle:"..angle)
--io.close(file)
--TodoΪʲô�������ı������ɲ��Ǹ������͵ģ���Lua_pushnumber(L,12.897f)ȴ���ԣ�
--����������ƫ�
local xspeed=speed*math.cos(angle*3.141592654/180)
local yspeed=-speed*math.sin(angle*3.141592654/180)
x=x+xspeed
y=y+yspeed
--file=io.open("lualog2.txt","w")
--file:write("xpeed:"..xspeed.."\r\nyspeed:"..yspeed)
--io.close(file)

--file=io.open("lualog3.txt","w")
--file:write(math.cos(270))
--io.close(file)
--angle=angle+1
local power_w=37--�������
local power_h=31--�����߶�
--Ԥ���ж��Ƿ�������������������������ǳ�������ת��
if x>522-power_w then--�����Ҳ�
	if angle>270 and angle<360 then--���±�����
	angle=210
	end
	if angle>0 and angle<90 then--���ϱ�����
	angle=130
	end
end
if x<119 then--�������
	if angle>180 and angle<270 then--���±�����
	angle=315
	end
	if angle>90 and angle<180 then--���ϱ�����
	angle=45
	end
end
if y<0 then--�����ϱ�
	if angle>0 and angle<90 then--���ϱ�����
	angle=315
	end
	if angle>90 and angle<180 then--���ϱ�����
	angle=225
	end
end
if y>480-power_h then--�����±�
	if angle>270 and angle<360 then--���±�����
	angle=45
	end
	if angle>180 and angle<270 then--���±�����
	angle=135
	end
end
return x..","..y..","..angle
elseif mode==1 then--ģʽ1Ϊ�����ı�����Ƿ����ɷ���ǹ켣�ƶ�
return x..","..y..","..angle
else
return x..","..y..","..angle
end
end


;
