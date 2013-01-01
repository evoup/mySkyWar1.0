--function GetNewPos (x,y,mode)
--if mode==0 then--模式0为直线向下移动
--x=x
--y=y+1
--return x..","..y
--elseif mode==1 then--模式1为斜线向左下运动
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
--elseif mode==2 then--模式2为斜线向右下运动
--x=x+3
--y=y+3
--return x..","..y
--else
--return x..","..y
--end
--end

function GetNewAnglePos(x,y,speed,mode,angle)
if mode==0 then--模式0为按角度来做移动
--local file=io.open("lualog.txt","w")
--file:write("x:"..x.."\r\ny:"..y.."\r\nspeed:"..speed.."\r\nmode:"..mode.."\r\nangle:"..angle)
--io.close(file)
--Todo为什么传进来的变量会变成不是浮点类型的，而Lua_pushnumber(L,12.897f)却可以！
--这样导致有偏差？
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
local power_w=37--能量宽度
local power_h=31--能量高度
--预先判断是否进入出界区，如果是则修正到非出街区和转向。
if x>522-power_w then--超出右侧
	if angle>270 and angle<360 then--右下变左下
	angle=210
	end
	if angle>0 and angle<90 then--右上变左上
	angle=130
	end
end
if x<119 then--超出左侧
	if angle>180 and angle<270 then--左下变右下
	angle=315
	end
	if angle>90 and angle<180 then--左上变右上
	angle=45
	end
end
if y<0 then--超出上边
	if angle>0 and angle<90 then--右上变右下
	angle=315
	end
	if angle>90 and angle<180 then--左上变左下
	angle=225
	end
end
if y>480-power_h then--超出下边
	if angle>270 and angle<360 then--右下变右上
	angle=45
	end
	if angle>180 and angle<270 then--左下变左上
	angle=135
	end
end
return x..","..y..","..angle
elseif mode==1 then--模式1为根据四壁入射角反弹成反射角轨迹移动
return x..","..y..","..angle
else
return x..","..y..","..angle
end
end


;
