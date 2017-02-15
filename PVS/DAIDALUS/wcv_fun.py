
AE = 0.00000001

def ae(a,b):
    return (abs(a-b)<AE)

import math

class Vector:
    
    def __init__(self, x = 0, y = 0, z = 0):
        self.x = x
        self.y = y
        self.z = z
    
    def __add__(self, val):
        return Vector( self.x + val.x, self.y + val.y, self.z + val.z )
    
    def __sub__(self,val):
        return Vector( self.x - val.x, self.y - val.y, self.z - val.z )

    def __div__(self, val):
        return Vector( self.x / val, self.y / val, self.z / val )
    
    def __mul__(self, val):
        return Vector( self.x * val, self.y * val, self.z * val )
                
    def __getitem__(self, i):
        if( i == 0):
            return self.x
        elif( i == 1):
            return self.y
        elif( i == 2):
            return self.z
        else:
            raise Exception("Invalid i to Vector")

    def __setitem__(self, i, value):
        if( i == 0):
            self.x = value
        elif( i == 1):
            self.y = value
        elif( i == 2):
            self.z = value
        else:
            raise Exception("Invalid i to Vector")
        
    def __str__(self):
        return "(" + str(self.x) + "," + str(self.y) + "," + str(self.z) + ")"

    def dot2(self,other):
        return self.x*other.x+self.y*other.y

    def norm2(self):
        return math.sqrt(self.sqv2())

    def sqv2(self):
        ans = self.x**2+self.y**2
        return ans

    def Scal(self,val):
        return self*val

    def AddScal(self,val,other):
        return self+other.Scal(val)

    def perpR(self):
        return Vector(self.y,-self.x, self.z)

    def det2(self,other):
        return self.dot2(other.perpR())

    def ae(self,other):
        return (ae(self.x,other.x) and ae(self.y,other.y) and ae(self.z,other.z))

    # def hat(self):
    #     if self.norm()==0: return self
    #     return self.Scal(1/self.norm())


def Delta(D,s,v):
    return sq(D)*v.sqv2()-sq(s.det2(v))

def Theta_D(D,s,v,eps):
  a = v.sqv2()
  b = s.dot2(v)
  c = s.sqv2()-sq(D)
  if Delta(D,s,v)>=0:
      return root2b(a,b,c,eps)
  else:
      return 0

def tcpa(s,v):
    if ae(v.norm2(),0): return 0
    return -(s.dot2(v))/v.sqv2()

def discr(a,b,c):
    return sq(b) - 4*a*c

def discr2b(a,b,c):
    return sq(b) - a*c

def root(a,b,c,eps):
    return (-b+eps*math.sqrt(discr(a,b,c)))/(2*a)

def root2b(a,b,c,eps):
    return (-b+eps*math.sqrt(discr2b(a,b,c)))/(a)

def sq(x):
    return x*x

def tcoa(sz,vz):
    if sz*vz < 0:
        return -(sz/vz)
    else:
        return -1

def sympoly2(x,y):
    if max(x,y)>=1:
        return max(x,y)
    else:
        return math.sqrt(sq(x)+(1-sq(x))*sq(y))

def sympoly3(x,y,z):
    if max(x,y,z)>=1:
        return max(x,y,z)
    else:
        return sympoly2(sympoly2(x,y),z)
    #math.sqrt(sq(x)+sq(y)+sq(x)-sq(x)*sq(y)-sq(x)*sq(z)-sq(y)*sq(z)+sq(x)*sq(y)*sq(z))

def timein(TTHR,DTHR,s,v):
    a = v.sqv2()
    b = 2*(s.dot2(v))+TTHR*v.sqv2()
    c = s.sqv2()+TTHR*(s.dot2(v))-sq(DTHR)
    if discr(a,b,c)<0: return 0
    return root(a,b,c,-1)

def horizontal_wcv_xdist(TTHR,DTHR,s,v):
    if ae(v.sqv2(),0):
        return s.x/DTHR
    # if (s.dot2(v)>=0):
    #     return v.norm2()*tcpa(s,v)/DTHR
    return (s+(v*tcpa(s,v))).norm2()/DTHR

def horizontal_wcv_ydist(TTHR,DTHR,s,v):
    tca = tcpa(s,v)
    tcadist = (s+(v*tca)).norm2()
    if ae(v.sqv2(),0):
        return s.y/DTHR
    if (s.dot2(v)>=0):
        return v.norm2()*abs(tcpa(s,v))/DTHR
    if tcadist>DTHR:
        return 0
    a = v.sqv2()
    b = 2*(s.dot2(v))+TTHR*v.sqv2()
    c = s.sqv2()+TTHR*(s.dot2(v))-sq(DTHR)
    thisroot = root(a,b,c,-1)
    ydist = tca/(tca-thisroot)
    if thisroot<=0:
        return ydist
    else:
        return thisroot/(tca-thisroot)+1

def vertical_wcv_norm(ZTHR,TCOA,sz,vz):
    if ae(TCOA,0):
        return abs(sz)/ZTHR
    else:
        return min(abs(sz)/ZTHR,max(abs(sz+max(tcoa(sz,vz),0)*vz)/ZTHR,tcoa(sz, vz)/TCOA))

def wcv_norm(TTHR,TCOA,DTHR,ZTHR,s,v):
    hnormx = horizontal_wcv_xdist(TTHR,DTHR,s,v)
    hnormy = horizontal_wcv_ydist(TTHR,DTHR,s,v)
    vnorm  = vertical_wcv_norm(ZTHR,TCOA,s.z,v.z)
    if s.dot2(v)>=0:
        hnorm = math.sqrt(sq(hnormx)+sq(hnormy))
    else:
        hnorm = sympoly2(hnormx,hnormy)
    return sympoly2(hnorm,vnorm)

def Si(TTHR,DTHR,s,v):
    if ae(s.norm2(),0): return DTHR
    rdot = s.dot2(v)/s.norm2()
    sqrtarg = sq(rdot*TTHR)+4*sq(DTHR)
    return max(DTHR,0.5*(math.sqrt(sqrtarg)-rdot*TTHR))

def RangePeni_dist(TTHR,DTHR,s,v):
    Sians = Si(TTHR,DTHR,s,v)
    if ae(Sians,0): return 1
    return 1-max((Sians-s.norm2())/Sians,0)

def RangePeni_dist2(TTHR,DTHR,s,v):
    sqrtin = DTHR**2+max(0,-TTHR*(s.dot2(v)))
    return s.norm2()/math.sqrt(sqrtin)

def HMDPen_dist(TTHR,DTHR,s,v):
    return (s+(v*tcpa(s,v))).norm2()/DTHR

def VertPen_dist(ZTHR,sz):
    return abs(sz)/ZTHR

def wcv_norm_alt(TTHR,DTHR,ZTHR,s,v):
    hnormx = RangePeni_dist(TTHR,DTHR,s,v)
    hnormy = HMDPen_dist(TTHR,DTHR,s,v)
    vnorm  = VertPen_dist(ZTHR,s.z)
    return sympoly3(hnormx,hnormy,vnorm)

def wcv_norm_alt_orig(TTHR,DTHR,ZTHR,s,v):
    hnormx = RangePeni_dist(TTHR,DTHR,s,v)
    hnormy = HMDPen_dist(TTHR,DTHR,s,v)
    vnorm  = VertPen_dist(ZTHR,s.z)
    return 1-(1-hnormx)*(1-hnormy)*(1-vnorm)

def wcv_norm_alt_other(TTHR,DTHR,ZTHR,s,v):
    hnormx = RangePeni_dist2(TTHR,DTHR,s,v)
    hnormy = HMDPen_dist(TTHR,DTHR,s,v)
    vnorm  = VertPen_dist(ZTHR,s.z)
    return sympoly3(hnormx,hnormy,vnorm)

DMODtest = 4000
HMDtest = 4000
ZTHRtest = 450
TTHRtest = 35
stest = Vector(-30000,2000,0)
vtest = Vector(337,0,0)

this_timein = timein(TTHRtest,DMODtest,stest,vtest)

this_tcpa   = tcpa(stest,vtest)

this_tcoa   = tcoa(stest.z,vtest.z)

this_Theta  = Theta_D(DMODtest,stest,vtest,-1)

this_Theta_out  = Theta_D(DMODtest,stest,vtest,1)

this_timeout= Theta_D(DMODtest,stest,vtest,1)

steps = 200

stepsize = (this_timeout-this_timein+4)/200



def testfun(t,justy):
    testfun.counter +=1
    ans = wcv_norm(TTHRtest,0,DMODtest,ZTHRtest,stest+(vtest*t),vtest)
    if justy:
        ans = horizontal_wcv_ydist(TTHRtest,DMODtest,stest+(vtest*t),vtest)
    if testfun.counter==1:
        print("time in is "+str(this_timein))
        print("tca is "+str(this_tcpa))
        print("tcoa is "+str(this_tcoa))
        print("Theta_D_in is "+str(this_Theta))
        print("Theta_D_out is "+str(this_Theta_out))
    return ans

testfun.counter = 0

def testfun_alt(t,justy):
    ans = wcv_norm_alt(TTHRtest,DMODtest,ZTHRtest,stest+(vtest*t),vtest)
    if justy:
        ans = RangePeni_dist(TTHRtest,DMODtest,stest+(vtest*t),vtest)
    return ans

def testfun_alt_orig(t,justy):
    ans = wcv_norm_alt_orig(TTHRtest,DMODtest,ZTHRtest,stest+(vtest*t),vtest)
    if justy:
        ans = RangePeni_dist(TTHRtest,DMODtest,stest+(vtest*t),vtest)
    return ans

def testfun_alt_other(t,justy):
    ans = wcv_norm_alt_other(TTHRtest,DMODtest,ZTHRtest,stest+(vtest*t),vtest)
    if justy:
        ans = RangePeni_dist2(TTHRtest,DMODtest,stest+(vtest*t),vtest)
    return ans

def minind(a):
    ans = 0
    for i in range(0,len(a)):
        if a[i]<a[ans]:
            ans = i
    return ans

import matplotlib.pyplot as plt

import numpy as np

base_range = [this_timein-2+i*stepsize for i in range(0,steps+5)]

usejusty = False

ans = [testfun(t,usejusty) for t in base_range] # main

bans = [testfun_alt(t,usejusty) for t in base_range] # the i

# cans = [testfun_alt_orig(t,usejusty) for t in base_range]

dans = [testfun_alt_other(t,usejusty) for t in base_range] # new one

print("minind is "+str(minind(ans)))

plt.plot(base_range,ans,color="blue")

plt.plot(base_range,bans,color="red")

#plt.plot(base_range,cans)

plt.plot(base_range,dans,color="green")

plt.plot([this_Theta],[testfun(this_Theta,usejusty)], '+', mew=4, ms=8)

plt.plot([this_Theta_out],[testfun(this_Theta_out,usejusty)], '+', mew=4, ms=8)

plt.plot([this_tcpa],[testfun(this_tcpa,usejusty)], '+', mew=4, ms=8)

plt.plot([this_tcpa],[testfun_alt(this_tcpa,usejusty)], '+', mew=4, ms=8)

plt.plot([this_Theta],[testfun_alt(this_Theta,usejusty)], '+', mew=4, ms=8)

plt.plot([this_Theta_out],[testfun_alt(this_Theta_out,usejusty)], '+', mew=4, ms=8)

# plt.plot([this_tcpa],[testfun_alt_orig(this_tcpa,usejusty)], '+', mew=4, ms=8)

# plt.plot([this_Theta],[testfun_alt_orig(this_Theta,usejusty)], '+', mew=4, ms=8)

# plt.plot([this_Theta_out],[testfun_alt_orig(this_Theta_out,usejusty)], '+', mew=4, ms=8)

plt.plot([this_tcpa],[testfun_alt_other(this_tcpa,usejusty)], '+', mew=4, ms=8)

plt.plot([this_Theta],[testfun_alt_other(this_Theta,usejusty)], '+', mew=4, ms=8)

plt.plot([this_Theta_out],[testfun_alt_other(this_Theta_out,usejusty)], '+', mew=4, ms=8)

plt.ylim(ymin = -0.02, ymax = 1.02)

plt.show()
