#http://www.vpython.org/

from visual import *
from random import random, randrange

class Bar:
    def __init__(self, pos=vector(0,-3,0)):
    
        self.ShipSphere1 = sphere (radius=0.5, color=color.red)
        self.ShipSphere2 = sphere (radius=0.5, color=color.red)
        self.ShipBox = box (pos=vector(pos), length=2, height=0.4, width=0.4, color=(1,1,1))

        self.ShipSphere1.pos=vector(pos.x-1, pos.y, pos.z)
        self.ShipSphere2.pos=vector(pos.x+1, pos.y, pos.z)

        self.pos=pos

    def Move(self, newpos=0):
        if ((newpos-1)<-4):
            self.ShipSphere1.pos.x=-4
            self.ShipSphere2.pos.x=-2
            self.ShipBox.pos.x=-3
        elif ((newpos+1)>4):
            self.ShipSphere1.pos.x=4
            self.ShipSphere2.pos.x=2
            self.ShipBox.pos.x=3
        else:
            self.ShipSphere1.pos.x=newpos-1
            self.ShipSphere2.pos.x=newpos+1
            self.ShipBox.pos.x=newpos


class Ball:
    def __init__(self, pos=vector(0,-2.5,0)):
        self.ball = sphere (pos=vector(pos), radius=0.2, color=color.yellow)
        self.depx=-1
        self.depy=1
        self.speed = 0.05
        self.send=0

        self.gameover=0
        self.tested=1

    def Send(self):
        self.send=1
        
    def Move(self, initpos=0):
        if self.send==0:
            self.ball.pos.x=initpos
        else:
            self.ball.pos.x+=self.depx*self.speed
            self.ball.pos.y+=self.depy*self.speed

            if (self.ball.x<-4) or (self.ball.x>4):
                self.depx=-self.depx

            if (self.ball.y>4):
                self.depy=-self.depy

            if (self.ball.y<-4):
                self.gameover=1

    def Test(self, testpos=0):
        if self.ball.y<=-2.5 and self.tested==0:
            self.tested=1
            if self.ball.x<testpos-1.5 or self.ball.x>testpos+1.5:
                self.depy=self.depy
            else:
                self.depy=-self.depy

        if self.ball.y>-2.5:
            self.tested=0

    def Reset(self):
        self.depx=-1
        self.depy=1
        self.speed = 0.05
        self.send=0

        self.gameover=0
        self.tested=1

        self.ball.pos=vector(0,-2.5,0)


class Brick:
    def __init__(self, pos=vector(0,0,0)):
        self.brick = box (pos=vector(pos), length=1, height=0.5, width=0.5, color=(0.75,0,0.25))
        self.brick.visible=1
        self.destroyed=0

    def Test(self, posball=vector(0,0,0)):
        if self.destroyed==0:
            if posball.y>self.brick.pos.y-0.35 and posball.y<self.brick.pos.y+0.35:
                if posball.x>self.brick.pos.x-0.6 and posball.x<self.brick.pos.x+0.6:
                    self.brick.visible=0
                    self.destroyed=1

                    if posball.x<self.brick.pos.x-0.5:
                        return 1

                    if posball.x>self.brick.pos.x+0.5:
                        return 2
                    
                    if posball.y<self.brick.pos.y-0.25:
                        return 3

                    if posball.y>self.brick.pos.y+0.25:
                        return 4

        return 0

    def Reset(self):
        self.brick.visible=1
        self.brick.length=1
        self.brick.height=0.5
        self.brick.width=0.5
        self.destroyed=0

class PlayField:
    def __init__(self, s=4.5):
        c=curve (color=color.blue, radius=0.1)
        pts = [(-s, -s, -s),(-s, -s, s), (-s, s, s),
               (-s, s, -s), (-s, -s, -s), (s, -s, -s),
               (s, s, -s), (-s, s, -s), (s, s, -s),
               (s, s, s), (-s, s, s), (s, s, s),
               (s, -s, s), (-s, -s, s), (s, -s, s),(s, -s, -s)]
        for pt in pts:
            c.append(pos=pt)

scene.title="arkanoid 3D"
scene.height=640
scene.width=640

Level=0
GameClosed=0

Bar=Bar()
Ball=Ball()
PlayField=PlayField()

Brick1=Brick()
Brick2=Brick()
Brick3=Brick()
Brick4=Brick()
Brick5=Brick()
Brick6=Brick()
Brick7=Brick()
Brick8=Brick()
Brick9=Brick()
Brick10=Brick()
Brick11=Brick()
Brick12=Brick()
Brick13=Brick()
Brick14=Brick()
Brick15=Brick()
Brick16=Brick()
Brick17=Brick()
Brick18=Brick()
Brick19=Brick()
Brick20=Brick()
Brick21=Brick()

Bricks=[Brick1, Brick2, Brick3, Brick4, Brick5, Brick6, Brick7,
        Brick8, Brick9, Brick10, Brick11, Brick12, Brick13, Brick14,
        Brick15, Brick16, Brick17, Brick18, Brick19, Brick20, Brick21]

for i in range(3):
    for j in range (7):
        Red=random()
        Green=random()
        Blue=random()
        Bricks[j+(i*7)].brick.pos=vector(-3.3+(1.1*j), 1.6-(0.6*i), 0)
        Bricks[j+(i*7)].brick.color=(Red, Green, Blue)

scene.autoscale=0

while GameClosed==0:

    rate (100+(Level*10))
    Ball.Move(Bar.ShipBox.pos.x)
    Ball.Test(Bar.ShipBox.pos.x)

    Bricks[1].brick.hidden=1
    
    DestroyedBricks=0
    
    for i in range(21):
        Result = Bricks[i].Test(Ball.ball.pos)
        if Result==1:
            Ball.depx=-1
        elif Result==2:
            Ball.depx=1
        elif Result==3:
            Ball.depy=-1
        elif Result==4:
            Ball.depy=1

        DestroyedBricks+=Bricks[i].destroyed

    if DestroyedBricks==21:
        levelup_text=label(pos=(0,0,0), text="Level %d completed" % (Level+1), opacity=0.5, box=1, line=0)
        m = scene.mouse.getevent()
        while not m.click:
            m = scene.mouse.getevent()

        Ball.Reset()
        for i in range(21):
            Bricks[i].Reset()
        Level+=1
        levelup_text.visible=0

    if (Level>10):
        congratulation_text=label(pos=(0,0,0), text="Congratulations - You WIN", opacity=0.5, box=1, line=0)
        GameClosed=1

    if Ball.gameover==1:
        gameover_text=label(pos=(0,0,0), text="Game Over", opacity=0.5, box=1, line=0)
        gameover_text.visible=1
        m = scene.mouse.getevent()
        while not m.click:
            m = scene.mouse.getevent()

        Ball.Reset()
        for i in range(21):
            Bricks[i].Reset()
        gameover_text.visible=0
        Level=0

    if scene.mouse.events:
        m = scene.mouse.getevent()

        if (m.click):
            Ball.Send()

    newpos = scene.mouse.project(normal=(0,0,1))
    if (newpos!=None):
        Bar.Move(newpos.x)
