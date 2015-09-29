% +----------------------------------------------------------------------+
% |   Filename           : WCBoundary_Tep.m                              |
% |   Description        : Well-clear model based on time to entry point |
% |   Created by         : C. Munoz, J. Upchurch, A. Narkawicz           |
% |   Created on         : 12 December 2013                              |
% |   Date last modified : 2 January 2014                                |
% +----------------------------------------------------------------------+

function [ Rxy, Dcpa, Tep, Rz, Tcoa, WCVxy, WCVz, WCV ] = ...
    WCBoundary_Tep(DTHR,TEP,ZTHR,TCOA, ...
    xo,yo,zo,xodot,yodot,zodot,xi,yi,zi,xidot,yidot,zidot)

% Inputs: 
%  DTHR       : Horizontal distance thresholds
%  TEP        : Time to entry point threshold
%  ZTHR       : Altitude threshold
%  TCOA       : Time to co-altitude threshold
%  (xo,yo,zo) : Ownship eastern, northern, and altitude positions
%  (xodot,yodot,zodot) : Ownship eastern, northern, and altitude speeds
%  (xi,yi,zi) : Intruder eastern, northern, and altitude positions
%  (xidot,yidot,zidot) : Intruder eastern, northern, and altitude speeds

% Outputs: 
%  Rxy    : Horiztonal range
%  Dcpa   : Distance at closest point of approach
%  Tep    : Time to entry point (first violation with respect to DTHR)
%  Rz     : Relative altitude
%  Tcoa   : Time to co-altitude
%  WCVxy  : Horizontal well-clear violation
%  WCVz   : Vertical well-clear violation
%  WCV    : Well-clear violation

% absolute position and velocity of ownship
s_o = [xo;yo]; s_oz = zo;    v_o = [xodot;yodot]; v_oz = zodot;

% absolute position and velocity of intruder
s_i = [xi;yi]; s_iz = zi;    v_i = [xidot;yidot]; v_iz = zidot;

% relative position and velocity 
s = (s_o-s_i); s_z = (s_oz-s_iz);
v = (v_o-v_i); v_z = (v_oz-v_iz);

% horizontal dimension outputs
ss = dot(s,s);
vv = dot(v,v);
sv = dot(s,v);
Rxy = sqrt(ss); % horizontal range
if vv > 0
  tcpa = -sv/vv; % time to closest point of approach
else
  tcpa = 0;
end
s_tcpa = s+tcpa*v; % relative position at closest point of approach
Dcpa = sqrt(dot(s_tcpa,s_tcpa)); % distance at TCPA
c = ss-DTHR^2; 
if sv < 0 % sv < 0 iff aircraft are converging in horizontal plane
    Tep = (-sv-sqrt(sv^2-vv*c))/vv; % time to entry point
else
    Tep = -1;
end

%============= HORIZONTAL WELL CLEAR VIOLATION DEFINITION =================
WCVxy = Rxy <= DTHR || (Sxy <= DTHR && 0 <= Tep && Tep <= TEP);
%==========================================================================

% vertical dimension outputs
Rz = abs(s_z); % relative altitude
cz = s_z*v_z; % Cz < 0 iff aircraft are converging in vertical plane
if cz < 0
    Tcoa = -s_z/v_z; % time to co-altitude
else
    Tcoa = -1;
end

%================= VERTICAL WELL CLEAR VIOLATION DEFINITION ===============
WCVz = Rz <= ZTHR || (0 <= Tcoa && Tcoa <= TCOA);
%==========================================================================

%================= WELL CLEAR VIOLATION DEFINITION ===============
WCV = WCVxy && WCVz;
%==========================================================================

end

