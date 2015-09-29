/* 
 * GreatCircle
 *
 * Contact: Jeff Maddalon
 * Organization: NASA/Langley Research Center
 *
 * Copyright (c) 2011-2015 United States Government as represented by
 * the National Aeronautics and Space Administration.  No copyright
 * is claimed in the United States under Title 17, U.S.Code. All Other
 * Rights Reserved.
 */

#include "Util.h"
//#include "UnitSymbols.h"
#include "GreatCircle.h"


#include <cmath>
namespace larcfm {



  static const double EPS = 1.0e-15; // small number, about machine

  const double GreatCircle::spherical_earth_radius = 6366707.0194937070000000000; // Units::from(_m, 1.0 / angle_from_distance(1.0));

  const double GreatCircle::minDt = 1E-5;

  double GreatCircle::decimal_angle(double degrees, double minutes, double seconds, bool north_east) {
    return ((north_east) ? 1.0 : -1.0) * Units::from("deg", (degrees + minutes / 60.0 + seconds / 3600.0));
  }

  double GreatCircle::angle_from_distance(double distance) {
    return Units::to("nm", distance) * Pi / (180.0 * 60.0);
  }

  double GreatCircle::angle_from_distance(double distance, double h) {
    return angle_from_distance(distance * spherical_earth_radius
			       / (spherical_earth_radius + h));
  }

  double GreatCircle::distance_from_angle(double angle, double h) {
	return (spherical_earth_radius+h) * angle;
  }

  double GreatCircle::angular_distance(double lat1, double lon1, double lat2, double lon2) {
//	  if (Util::almost_equals(lat1,lat2) && Util::almost_equals(lon1,lon2)) return 0.0;
    return asin_safe(sqrt_safe(sq(sin((lat1 - lat2) / 2))
				    + cos(lat1) * cos(lat2)
				    * sq(sin((lon1 - lon2) / 2)))) * 2.0;
  }

  double GreatCircle::angular_distance(const LatLonAlt& p1, const LatLonAlt& p2) {
    return angular_distance(p1.lat(), p1.lon(), p2.lat(), p2.lon());
  }

  double GreatCircle::distance(double lat1, double lon1, double lat2, double lon2) {
    return distance_from_angle(angular_distance(lat1, lon1, lat2, lon2), 0.0);
  }

  double GreatCircle::distance(const LatLonAlt& p1, const LatLonAlt& p2) {
//std::cout << p1.toString() << " , " << p2.toString() << std::endl;
	  //return distance_from_angle(angular_distance(p1, p2),(p1.alt() + p2.alt())/2.0);
    return distance_from_angle(angular_distance(p1, p2), 0.0);
  }

  bool GreatCircle::almost_equals(double lat1, double lon1, double lat2, double lon2) {
    return Constants::almost_equals_radian(angular_distance(lat1, lon1, lat2, lon2));
  }
	

  bool GreatCircle::almost_equals(double lat1, double lon1, double lat2,
		     double lon2, double epsilon) {
    return Util::within_epsilon(distance(lat1, lon1, lat2, lon2), epsilon);
  }

  // parameter d is the angular distance between lat/lon #1 and lat/lon #2
  static double initial_course_impl(const LatLonAlt& p1, const LatLonAlt& p2, double d) {
    double lat1 = p1.lat();
    double lon1 = p1.lon();
    double lat2 = p2.lat();
    double lon2 = p2.lon();
    
    if (cos(lat1) < EPS) { // EPS a small number, about machine precision
      if (lat1 > 0) {
	return Pi; // starting from North pole, all directions are south
      } else {
	return 2.0 * Pi; // starting from South pole, all directions are
	// north. JMM: why not 0?
      }
    }
    
    if (Constants::almost_equals_radian(d)) {
      return 0.0;
      // if the two points are almost the same, then any course is valid
      // returning 0.0 here avoids a 0/0 division (NaN) in the
      // calculations below.
    }
    
    double tc1;
    double acos1 = (sin(lat2) - sin(lat1) * cos(d)) / (sin(d) * cos(lat1));
    //std::cout << "GCICI num=" << (sin(lat2) - sin(lat1) * cos(d)) << std::endl;
    //std::cout << "GCICI denom=" << (sin(d)*cos(lat1)) << std::endl;
    //std::cout << "GCICI sin(d)=" << sin(d) << std::endl;
    //std::cout << "GCICI lat1=" << lat1 << std::endl;
    //std::cout << "GCICI cos(lat1)=" << cos(lat1) << std::endl;
    //std::cout << "GCICI " << acos1 << std::endl;
    if (Util::almost_equals(lon2, lon1, PRECISION13)) {
      tc1 = acos_safe(acos1);
    } else {		
      if (sin(lon2 - lon1) > 0) { 
	// slightly different than av
	// formulary because of +East convention that I use
	tc1 = acos_safe(acos1);
      } else {
	tc1 = 2 * Pi - acos_safe(acos1);
      }
    }
    //std::cout << "GCICI crs=" << tc1 << "  " << (tc1-Pi) << std::endl;
    return tc1;
  }
  
  double GreatCircle::initial_course(double lat1, double lon1, double lat2, double lon2) {
    LatLonAlt p1 = LatLonAlt::mk(lat1, lon1, 0.0);
    LatLonAlt p2 = LatLonAlt::mk(lat2, lon2, 0.0);
    double d = angular_distance(lat1, lon1, lat2, lon2);
    return initial_course_impl(p1, p2, d);
  }

  double GreatCircle::initial_course(LatLonAlt p1, LatLonAlt p2) {
     	    return initial_course(p1.lat(),p1.lon(),
                                  p2.lat(),p2.lon());
  }


  // parameter d is the angular distance between lat/long #1 and #2
  static LatLonAlt interpolate_impl(const LatLonAlt& p1, const LatLonAlt& p2, double d, double f, double alt) {
    if (Constants::almost_equals_radian(d) ) {
      return p1.mkAlt(alt);
      // if the two points are almost the same, then consider the two
      // points the same and arbitrarily return one of them (in this case
      // p1)
      // with the altitude that was provided
    }
    
    double lat1 = p1.lat();
    double lon1 = p1.lon();
    double lat2 = p2.lat();
    double lon2 = p2.lon();
    
    double a = sin((1 - f) * d) / sin(d);
    double b = sin(f * d) / sin(d);
    double x = a * cos(lat1) * cos(lon1) + b * cos(lat2) * cos(lon2);
    double y = a * cos(lat1) * sin(lon1) + b * cos(lat2) * sin(lon2);
    double z = a * sin(lat1) + b * sin(lat2);
    return LatLonAlt::mk(atan2_safe(z, sqrt(x * x + y * y)), // lat
				  atan2_safe(y, x), // longitude
				  alt); // long
  }
  
  double GreatCircle::representative_course(double lat1, double lon1, double lat2, double lon2) {
    LatLonAlt p1 = LatLonAlt::mk(lat1, lon1, 0.0);
    LatLonAlt p2 = LatLonAlt::mk(lat2, lon2, 0.0);
    double d = angular_distance(lat1, lon1, lat2, lon2);
    LatLonAlt midPt = interpolate_impl(p1, p2, d, 0.5, 0.0);
    return initial_course_impl(midPt, p2, d / 2.0);
  }
  

  double GreatCircle::representative_course(const LatLonAlt& p1, const LatLonAlt& p2) {
	  return representative_course(p1.lat(),p1.lon(),p2.lat(),p2.lon());
  }

  LatLonAlt GreatCircle::interpolate(const LatLonAlt& p1, const LatLonAlt& p2, double f) {
    double d = angular_distance(p1, p2);
    return interpolate_impl(p1, p2, d, f, (p2.alt() - p1.alt())*f + p1.alt());
  }

  LatLonAlt GreatCircle::interpolateEst(const LatLonAlt& p1, const LatLonAlt& p2, double f) {
		return LatLonAlt::mk((p2.lat() - p1.lat()) * f + p1.lat(),
		                     (p2.lon() - p1.lon()) * f + p1.lon(),
              	             (p2.alt() - p1.alt()) * f + p1.alt());
  }

  static LatLonAlt linear_initial_impl(const LatLonAlt& s, double track, double d, double vertical) {
    double lat = asin_safe(sin(s.lat())*cos(d)+cos(s.lat())*sin(d)*cos(track));
    double dlon = atan2_safe(sin(track)*sin(d)*cos(s.lat()),cos(d)-sin(s.lat())*sin(lat));
    // slightly different from aviation formulary because I use
    // "east positive" convention
    double lon = to_pi(s.lon() + dlon);

    return LatLonAlt::mk(lat, lon, s.alt()+vertical);
  }

  static LatLonAlt linear_rhumb_impl(const LatLonAlt& s, double track, double d, double vertical) {
	// -- Based on the calculation in the "Rhumb line" section of the
	//    Aviation Formulary v1.44
	// -- Weird things happen to rhumb lines that go to the poles, therefore
	//    force any polar latitudes to be "near" the pole

	static const double eps = 1e-15;
	double s_lat = std::max(std::min(s.lat(), Pi/2-eps), -Pi/2+eps);
	double lat = s_lat + d * cos(track);
	lat = std::max(std::min(lat, Pi/2-eps), -Pi/2+eps);

	double q;
	if ( Constants::almost_equals_radian(lat, s_lat) ) {
		// (std::abs(lat - lat1) < EPS) {
		q = cos(s_lat);
	} else {
		double dphi = log(tan(lat / 2 + Pi / 4)
				/ tan(s_lat / 2 + Pi / 4));
		q = (lat - s_lat) / dphi;
	}
	double dlon = -d * sin(track) / q;

	// slightly different from aviation formulary because I use
	// "east positive" convention
	double lon = to_pi(s.lon() - dlon);
	return LatLonAlt::mk(lat, lon, s.alt()+vertical);
  }

  LatLonAlt GreatCircle::linear_gcgs(const LatLonAlt& p1, const LatLonAlt& p2, const Velocity& v, double t) {
	double d = GreatCircle::angular_distance(p1, p2);
	if ( Constants::almost_equals_radian(d) ) {
		// If the two points are about 1 meter apart, then count them as the
      // same.
      return p1;
    }
    double f = angle_from_distance(v.gs() * t) / d;
    return interpolate_impl(p1, p2, d, f, p1.alt() + v.z*t);
  }
  
  LatLonAlt GreatCircle::linear_gc(LatLonAlt p1, LatLonAlt p2, double d) {
    //return GreatCircle.linear_initial(p1, initial_course(p1,p2), d);
    double dist = angular_distance(p1,p2);
    double f = angle_from_distance(d)/dist;
    return interpolate_impl(p1, p2, dist, f, (p2.alt() - p1.alt())*f + p1.alt());
  }
  
  LatLonAlt GreatCircle::linear_rhumb(const LatLonAlt& s, const Velocity& v, double t) {
    return linear_rhumb_impl(s, v.trk(), GreatCircle::angle_from_distance(v.gs() * t), v.z*t);
  }

  LatLonAlt GreatCircle::linear_rhumb(const LatLonAlt& s, double track, double dist) {
    return linear_rhumb_impl(s, track, GreatCircle::angle_from_distance(dist), 0.0);
  }
	
  LatLonAlt GreatCircle::linear_initial(const LatLonAlt& s, const Velocity& v, double t) {
    return linear_initial_impl(s, v.trk(), GreatCircle::angle_from_distance(v.gs() * t), v.z*t);
  }

  LatLonAlt GreatCircle::linear_initial(const LatLonAlt& s, double track, double dist) {
    return linear_initial_impl(s, track, GreatCircle::angle_from_distance(dist), 0.0);
  }

  double GreatCircle::cross_track_distance(const LatLonAlt& p1, const LatLonAlt& p2, const LatLonAlt& offCircle) {
  	double dist_p1oc = angular_distance(p1,offCircle);
  	double trk_p1oc = initial_course_impl(p1,offCircle,dist_p1oc);
  	double trk_p1p2 = initial_course(p1,p2);
  	// This is a direct application of the "spherical law of sines"
  	return distance_from_angle(Util::asin_safe(sin(dist_p1oc)*sin(trk_p1oc-trk_p1p2)), (p1.alt()+p2.alt()+offCircle.alt())/3.0);
  }

  bool GreatCircle::collinear(const LatLonAlt& p1, const LatLonAlt& p2, const LatLonAlt& p3) {
    	double epsilon = 1E-7;
    	return Util::within_epsilon(cross_track_distance(p1,p2,p3),epsilon);
    }

  LatLonAlt GreatCircle::closest_point_circle(const LatLonAlt& p1, const LatLonAlt& p2, const LatLonAlt& x) {
    	double p1p2_dist = angular_distance(p1,p2);
    	if (Util::almost_equals(p1p2_dist,0.0)) {
    		return x;   // if p1==p2, every great circle runs through them, thus x is on one of these great circles  
    	}
    	if (p1.almostEquals(x)) {
    		return p1.mkAlt(x.alt());
    	}
    	if (p2.almostEquals(x)) {
    		return p2.mkAlt(x.alt());
    	}


    	// general case
    	double c = angular_distance(p1,x); // angular distance from p1 to x, 0 is ruled out above
        double p1p2_trk = initial_course_impl(p1,p2,p1p2_dist);
	double p1x_trk = initial_course_impl(p1,x,c);
	double A = std::abs(Util::to_pi(p1p2_trk - p1x_trk));
	double a = Util::asin_safe(std::sin(c)*std::sin(A)); // "spherical law of sines"
	if (Util::within_epsilon(a, 1E-7)) return x;  // collinear
	double b = Util::atan2_safe(std::cos(A) * std::sin(c), std::cos(c)); // should be distance along gc p1-p2
	return interpolate_impl(p1,p2,p1p2_dist,(b / p1p2_dist),x.alt());

    }
    
 LatLonAlt GreatCircle::closest_point_segment(const LatLonAlt& p1, const LatLonAlt& p2, const LatLonAlt& x) {
    	LatLonAlt p3 = closest_point_circle(p1,p2,x);
    	double p1p2t = initial_course(p1,p2);
    	double p1p2d = angular_distance(p1,p2);
    	double p1p3d = angular_distance(p1,p3);
   	    	
    	LatLonAlt p3p = linear_initial_impl(p1,p1p2t,p1p3d,p3.alt());
    	if (p3.almostEquals(p3p)) {
    		if (p1p2d > p1p3d) {
    			return p3;
    		} else {
    			return p2;
    		}
    	} else {
        	// At this point, it should be that:
    		//    linear_initial_impl(p1,p1p2t+M_PI,p1p3d,p3.alt()) == p3
    		//
        	//if ( ! linear_initial_impl(p1,p1p2t+M_PI,p1p3d,p3.alt()).almostEquals(p3)) throw new RuntimeException("help!");
    		return p1;
    	}
    }
    

 /**
  * Given two great circles defined by a1,a2 and b1,b2, return the intersection poin that is closest a1.  Use LatLonAlt.antipode() to get the other value.
  * This assumes that the arc distance between a1,a2 < 90 and b1,b2 < 90
  * This returns an INVALID value if both segments are collinear
  * EXPERIMENTAL
  */
 LatLonAlt GreatCircle::intersection(const LatLonAlt& a1, const LatLonAlt& a2, const LatLonAlt& b1, const LatLonAlt& b2) {
 	Vect3 va = spherical2xyz(a1.lat(), a1.lon()).cross(spherical2xyz(a2.lat(), a2.lon()));
 	Vect3 vb = spherical2xyz(b1.lat(), b1.lon()).cross(spherical2xyz(b2.lat(), b2.lon()));
 	double r = GreatCircle::spherical_earth_radius;
 	Vect3 vavb = va.cross(vb);
 	if (vavb.almostEquals(Vect3::ZERO)) {
 		return LatLonAlt::INVALID;
 	}
 	Vect3 v1 = vavb.Scal(r / vavb.norm());
 	Vect3 v2 = vavb.Scal(-r / vavb.norm());
 	LatLonAlt x1 = xyz2spherical(v1).mkAlt(a1.alt());
 	LatLonAlt x2 = xyz2spherical(v2).mkAlt(a1.alt());
 	if (distance(a1,x1) < distance(a1,x2)) {
 		return x1;
 	} else {
 		return x2;
 	}
 }

 /**
  * Given two great circles defined by so, so2 and si, si2 return the intersection point that is closest to so.
  * Calculate altitude of intersection using time dt = delta time from "so" to "so2"
  *
  * @param so
  * @param so2
  * @param dto
  * @param si
  * @param si2
  * @return a pair: intersection point and the delta time from point "so" to the intersection, can be negative if intersect pt. in the past
  *              if intersection point is invalid then the returned delta time is -1
  */
 std::pair<LatLonAlt,double>  GreatCircle::intersection(const LatLonAlt& so, const LatLonAlt& so2, double dto, const LatLonAlt& si, const LatLonAlt& si2) {
 	LatLonAlt lgc = GreatCircle::intersection(so, so2, si, si2);
    	//f.pln(" %%% GreatCircle.intersection: lgc = "+lgc.toString(15));
 	if (lgc.isInvalid()) return std::pair<LatLonAlt,double>(lgc,-1.0);
 	double gso = distance(so,so2)/dto;
 	double intTm = distance(so,lgc)/gso;  // relative to so
 	//f.pln(" ## gso = "+Units.str("kn", gso)+" distance(so,lgc) = "+Units.str("nm",distance(so,lgc)));
 	bool behind = GreatCircle::behind(lgc, so, GreatCircle::velocity_average(so, so2, 1.0));
 	if (behind) intTm = -intTm;
 	// compute a better altitude
 	double vs = (so2.alt() - so.alt())/dto;
 	double nAlt = so.alt() + vs*(intTm);
 	//f.pln(" $$ LatLonAlt.intersection: intTm = "+intTm+" vs = "+Units.str("fpm",vs)+" nAlt = "+Units.str("ft",nAlt)+" "+behind);
 	LatLonAlt pgc = LatLonAlt::mk(lgc.lat(),lgc.lon(),nAlt);
 	return std::pair<LatLonAlt,double>(pgc,intTm);
 }


 std::pair<LatLonAlt,double> GreatCircle::intersection(const LatLonAlt& so, const Velocity& vo, const LatLonAlt& si, const Velocity& vi, bool checkBehind) {
 	LatLonAlt so2 = linear_initial(so, vo, 1000);
 	LatLonAlt si2 = linear_initial(si, vi, 1000);
 	LatLonAlt i = intersection(so, so2, si, si2);
 	if (checkBehind) {
    	if (i.isInvalid() || behind(i, so,vo) || behind(i, si,vi)) return std::pair<LatLonAlt,double>(LatLonAlt::INVALID,-1.0); // collinear or (nearly) same position or cross in the past
 	}
 	double dt = distance(so,i)/vo.gs();
 	if (behind(i, so, vo)) dt = -dt;   // ??? RWB ???
 	return std::pair<LatLonAlt,double>(i,dt);
}

double GreatCircle::angleBetween(const LatLonAlt& a1, const LatLonAlt& a2, const LatLonAlt& b1, const LatLonAlt& b2) {
	Vect3 va = spherical2xyz(a1.lat(), a1.lon()).cross(spherical2xyz(a2.lat(), a2.lon())).Hat(); // normal 1
	Vect3 vb = spherical2xyz(b1.lat(), b1.lon()).cross(spherical2xyz(b2.lat(), b2.lon())).Hat(); // normal 2
	double cosx = va.dot(vb);
	return std::acos(cosx);
}


 /**
  * Return true if x is "behind" ll, considering its current direction of travel, v.
  * "Behind" here refers to the hemisphere aft of ll.
  * That is, x is within the region behind the perpendicular line to v through ll.
  * @param ll aircraft position
  * @param v aircraft velocity
  * @param x intruder positino
  * @return
  */
 bool GreatCircle::behind(const LatLonAlt& x, const LatLonAlt& ll, const Velocity& v) {
 	Velocity v2 = velocity_initial(ll, x, 100);
 	return Util::turnDelta(v.trk(), v2.trk()) > M_PI/2.0;
 }

	/**
	 * Returns values describing if the ownship state will pass in front of or behind the intruder (from a horizontal perspective)
	 * @param so ownship position
	 * @param vo ownship velocity
	 * @param si intruder position
	 * @param vi intruder velocity
	 * @return 1 if ownship will pass in front (or collide, from a horizontal sense), -1 if ownship will pass behind, 0 if collinear or parallel or closest intersection is behind you
	 */
 int GreatCircle::passingDirection(const LatLonAlt& so, const Velocity& vo, const LatLonAlt& si, const Velocity& vi) {
 	std::pair<LatLonAlt,double> p = intersection(so,vo,si,vi,true);
 	if (p.second < 0) return 0;
 	LatLonAlt si3 = linear_initial(si,vi,p.second); // intruder position at time of intersection
 	if (behind(p.first, si3, vi)) return -1;
 	return 1;
 }

//    int dirForBehind(LatLonAlt so, Velocity vo, LatLonAlt si, Velocity vi) {
//    	LatLonAlt so2 = linear_initial(so, vo, 1000);
//    	LatLonAlt si2 = linear_initial(si, vi, 1000);
//    	LatLonAlt i = intersection(so, so2, si, si2);
//    	if (i.isInvalid() || behind(so,vo,i) || behind(si,vi,i)) return 0; // collinear or (nearly) same position or cross in the past
//    	double tso = distance(so,i)/vo.gs();
//    	if (behind(so,vo,i)) tso = -tso;
//    	LatLonAlt siXP = linear_initial(si,vi,tso);
//    	int ahead = (behind(siXP,vi,i) ? -1 : 1);
//    	int onRight = Util::sign(cross_track_distance(so,i,siXP));
//fpln("ahead="+ahead+"  onRight="+onRight+" siXP="+siXP.toStringNP(8)+" i="+i.toStringNP(8));
//    	return ahead*onRight;
//    }


 int GreatCircle::dirForBehind(const LatLonAlt& so, const Velocity& vo, const LatLonAlt& si, const Velocity& vi) {
 	LatLonAlt so2 = linear_initial(so, vo, 1000);
 	LatLonAlt si2 = linear_initial(si, vi, 1000);
 	LatLonAlt i = intersection(so, so2, si, si2);
 	if (i.isInvalid() || behind(i,so,vo) || behind(i,si,vi)) return 0; // collinear or (nearly) same position or cross in the past
 	int onRight = Util::sign(cross_track_distance(si,si2,so));
 	return -onRight;
 }



  Velocity GreatCircle::velocity_initial(const LatLonAlt& p1, const LatLonAlt& p2, double t) {
    // p1 is the source position, p2 is another point to form a great circle
    // positive time is moving from p1 toward p2
    // negative time is moving from p1 away from p2
    if (std::abs(t) < minDt || Util::almost_equals(std::abs(t) + minDt, minDt,
			      PRECISION7)) {
      // time is negative or very small (less than 1 ms)
      return Velocity::ZEROV;
    }
    double d = angular_distance(p1, p2);
    if (Constants::almost_equals_radian(d)) {
      if (Constants::almost_equals_alt(p1.alt(), p2.alt())) {
	// If the two points are about 1 meter apart, then count them as
	// the same.
	return Velocity::ZEROV;
      } else {
	return Velocity::ZEROV.mkVs((p2.alt() - p1.alt()) / t);
      }
    }
    double gs = GreatCircle::distance_from_angle(d, 0.0) / t;
    double crs = initial_course_impl(p1, p2, d);
    return Velocity::mkTrkGsVs(crs, gs, (p2.alt() - p1.alt()) / t);
  }

  Velocity GreatCircle::velocity_average(const LatLonAlt& p1, const LatLonAlt& p2, double t) {
    // p1 is the source position, p2 is another point on that circle
    // positive time is moving from p1 toward p2
    // negative time is moving from p1 away from p2
    if (t >= 0.0) {
      return GreatCircle::velocity_initial(GreatCircle::interpolate(p1, p2, 0.5), p2, t / 2.0);
    } else {
      return GreatCircle::velocity_average(p1, GreatCircle::interpolate(p1, p2, -1.0), -t);
    }
  }

  Velocity GreatCircle::velocity_average_speed(const LatLonAlt& s1, const LatLonAlt& s2, double speed) {
	  double dist = GreatCircle::distance(s1, s2);
	  double dt = dist/speed;
	  return GreatCircle::velocity_average(s1, s2, dt);
  }


  Velocity GreatCircle::velocity_final(const LatLonAlt& p1, const LatLonAlt& p2, double t) {
    // p1 is the source position, p2 is another point on that circle
    // positive time is moving from p1 toward p2
    // negative time is moving from p1 away from p2 (final velocity is the
    // velocity at that time)
    if (t >= 0.0) {
      return GreatCircle::velocity_initial(p2, p1, -t);
    } else {
      return GreatCircle::velocity_initial(GreatCircle::interpolate(p1, p2, -1.0), p1, t);
    }
  }


  /**
   * Transforms a lat/lon position to a point on in R3 (on a sphere)
   * This is an Earth-Centered, Earth-Fixed translation (assuming earth-surface altitude).
   * From Wikipedia http://en.wikipedia.org/wiki/Curvilinear_coordinates (contents apparently moved to Geodetic datum entry)
   * We take a standard radius of the earth as defined in GreatCircle, and treat altitude as 0.
   * @param lat Latitude
   * @param lon Longitude
   * @return point in R3 on surface of the earth
   */
  Vect3 GreatCircle::spherical2xyz(double lat, double lon) {
  	double r = GreatCircle::spherical_earth_radius;
  	// convert latitude to 0-PI
  	double theta = M_PI/2 - lat;
  	double phi = M_PI - lon;
  	double x = r*std::sin(theta)*std::cos(phi);
  	double y = r*std::sin(theta)*std::sin(phi);
  	double z = r*std::cos(theta);
  	return Vect3(x,y,z);
  }

  /**
   * Transforms a R3 position on the earth surface into lat/lon coordinates
   * This is an Earth-Centered, Earth-Fixed translation (assuming earth-surface altitude).
   * From Wikipedia http://en.wikipedia.org/wiki/Curvilinear_coordinates (contents apparently moved to Geodetic datum entry)
   * We take a standard radius of the earth as defined in GreatCircle, and treat altitude as 0.
   * @param v position in R3, with ECEF origin
   * @return LatLonAlt point on surface of the earth (zero altitude)
   */
  LatLonAlt GreatCircle::xyz2spherical(const Vect3& v) {
  	double r = GreatCircle::spherical_earth_radius;
  	double theta = Util::acos_safe(v.z/r);
  	double phi = Util::atan2_safe(v.y, v.x);
  	double lat = M_PI/2 - theta;
  	double lon = Util::to_pi(M_PI - phi);
  	return LatLonAlt::mk(lat, lon, 0);
  }


}
