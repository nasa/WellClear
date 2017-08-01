/**

Notices:

Copyright 2016 United States Government as represented by the
Administrator of the National Aeronautics and Space Administration. No
copyright is claimed in the United States under Title 17,
U.S. Code. All Other Rights Reserved.

Disclaimers

No Warranty: THE SUBJECT SOFTWARE IS PROVIDED "AS IS" WITHOUT ANY
WARRANTY OF ANY KIND, EITHER EXPRESSED, IMPLIED, OR STATUTORY,
INCLUDING, BUT NOT LIMITED TO, ANY WARRANTY THAT THE SUBJECT SOFTWARE
WILL CONFORM TO SPECIFICATIONS, ANY IMPLIED WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, OR FREEDOM FROM
INFRINGEMENT, ANY WARRANTY THAT THE SUBJECT SOFTWARE WILL BE ERROR
FREE, OR ANY WARRANTY THAT DOCUMENTATION, IF PROVIDED, WILL CONFORM TO
THE SUBJECT SOFTWARE. THIS AGREEMENT DOES NOT, IN ANY MANNER,
CONSTITUTE AN ENDORSEMENT BY GOVERNMENT AGENCY OR ANY PRIOR RECIPIENT
OF ANY RESULTS, RESULTING DESIGNS, HARDWARE, SOFTWARE PRODUCTS OR ANY
OTHER APPLICATIONS RESULTING FROM USE OF THE SUBJECT SOFTWARE.
FURTHER, GOVERNMENT AGENCY DISCLAIMS ALL WARRANTIES AND LIABILITIES
REGARDING THIRD-PARTY SOFTWARE, IF PRESENT IN THE ORIGINAL SOFTWARE,
AND DISTRIBUTES IT "AS IS."

Waiver and Indemnity: RECIPIENT AGREES TO WAIVE ANY AND ALL CLAIMS
AGAINST THE UNITED STATES GOVERNMENT, ITS CONTRACTORS AND
SUBCONTRACTORS, AS WELL AS ANY PRIOR RECIPIENT.  IF RECIPIENT'S USE OF
THE SUBJECT SOFTWARE RESULTS IN ANY LIABILITIES, DEMANDS, DAMAGES,
EXPENSES OR LOSSES ARISING FROM SUCH USE, INCLUDING ANY DAMAGES FROM
PRODUCTS BASED ON, OR RESULTING FROM, RECIPIENT'S USE OF THE SUBJECT
SOFTWARE, RECIPIENT SHALL INDEMNIFY AND HOLD HARMLESS THE UNITED
STATES GOVERNMENT, ITS CONTRACTORS AND SUBCONTRACTORS, AS WELL AS ANY
PRIOR RECIPIENT, TO THE EXTENT PERMITTED BY LAW.  RECIPIENT'S SOLE
REMEDY FOR ANY SUCH MATTER SHALL BE THE IMMEDIATE, UNILATERAL
TERMINATION OF THIS AGREEMENT.
 **/

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.PrintWriter;
import java.util.Optional;

import gov.nasa.larcfm.ACCoRD.ConflictData;
import gov.nasa.larcfm.ACCoRD.Daidalus;
import gov.nasa.larcfm.ACCoRD.DaidalusFileWalker;
import gov.nasa.larcfm.ACCoRD.Detection3D;
import gov.nasa.larcfm.ACCoRD.Horizontal;
import gov.nasa.larcfm.ACCoRD.Vertical;
import gov.nasa.larcfm.ACCoRD.WCV_tvar;
import gov.nasa.larcfm.Util.Util;
import gov.nasa.larcfm.Util.Units;

public class DaidalusAlerting {

	public static void main(String[] args) {

		// Create a Daidalus object for an unbuffered well-clear volume and instantaneous bands
		Daidalus daa = new Daidalus();
		String input_file = "";
		String output_file = "";
		PrintWriter out = new PrintWriter(System.out);

		// A Daidalus object can be configured either programatically or by using a configuration file.
		for (int a=0;a < args.length; ++a) {
			String arga = args[a];
			if (arga.equals("--noma") || arga.equals("-noma")) {
				// Configure DAIDALUS to nominal A parameters: Kinematic Bands, Turn Rate 1.5 [deg/s])
				daa.set_Buffered_WC_SC_228_MOPS(false);
			} else if (arga.equals("--nomb") || arga.equals("-nomb")) {
				// Configure DAIDALUS to nominal B parameters: Kinematic Bands, Turn Rate 3.0 [deg/s])
				daa.set_Buffered_WC_SC_228_MOPS(true);
			} else if ((arga.startsWith("--c") || arga.startsWith("-c")) && a+1 < args.length) {
				// Load configuration file
				arga = args[++a];
				if (!daa.parameters.loadFromFile(arga)) {
					System.err.println("File "+arga+" not found");
					System.exit(1);
				} else {
					System.out.println("** Error: Loading configuration file "+arga);
				}
			} else if ((arga.startsWith("--o") || arga.startsWith("-o")) && a+1 < args.length) {
				output_file = args[++a];
			} else if (arga.startsWith("--h") || arga.startsWith("-h")) {
				System.err.println("** Error: Options: [--noma | --nomb | --conf <configuration file> | --output <output file> | --help] <input file>");
				System.exit(1);
			} else if (arga.startsWith("-")){
				System.err.println("** Error: Unknown option "+arga);
				System.exit(1);
			} else if (input_file.equals("")) {
				input_file = args[a];
			} else {
				System.err.println("** Error: Only one input file can be provided ("+a+")");
				System.exit(1);
			}				
		}
		if (input_file.equals("")) {
			System.err.println("** Error: One input file must be provided");
			System.exit(1);
		}
		File file = new File(input_file);
		if (!file.exists() || !file.canRead()) {
			System.err.println("** Error: File "+input_file+" cannot be read");
			System.exit(1);
		}
		try {
			String name = file.getName();
			String scenario = name.contains(".") ? name.substring(0, name.lastIndexOf('.')):name;
			if (output_file.equals("")) {
				output_file = scenario+".csv";
			} 
			out = new PrintWriter(new BufferedWriter(new FileWriter(output_file)),true);
		} catch (Exception e) {
			System.err.println("** Error: "+e);
			System.exit(1);
		}

		System.out.println("Processing DAIDALUS file "+input_file);
		System.out.println("Generating CSV file "+output_file);
		DaidalusFileWalker walker = new DaidalusFileWalker(input_file);
		Optional<Detection3D> detector = daa.parameters.alertor.conflictDetector();
		String uhor = daa.parameters.getUnits("min_horizontal_recovery");
		String uver = daa.parameters.getUnits("min_vertical_recovery");
		String ugs = daa.parameters.getUnits("gs_step");
		String uvs = daa.parameters.getUnits("vs_step");

		out.print(" Time, Ownship, Traffic, Alert Level");
		String line_units = "[s],,,";
		for (int i=1; i <= daa.parameters.alertor.mostSevereAlertLevel();++i) {
			out.print(", Time to Volume of Alert("+i+")");
			line_units += ", [s]";
		}
		out.print(", Horizontal Separation, Vertical Separation, Horizontal Closure Rate, Vertical Closure Rate, Projected HMD, Projected VMD, Projected TCPA, Projected DCPA, Projected TCOA");
		line_units += ", ["+uhor+"], ["+uver+"], ["+ugs+"], ["+uvs+"], ["+uhor+"], ["+uver+"], [s], ["+uhor+"], [s]";
		if (detector.isPresent() && detector.get() instanceof WCV_tvar) {
			out.print(", Projected TAUMOD (WCV*)");
			line_units += ", [s]";
		}
		out.println();
		out.println(line_units);

		while (!walker.atEnd()) {
			walker.readState(daa);
			// At this point, daa has the state information of ownhsip and traffic for a given time
			for (int ac=1; ac <= daa.lastTrafficIndex(); ++ac) {
				out.print(daa.getCurrentTime());
				out.print(", "+daa.getOwnshipState().getId());
				out.print(", "+daa.getAircraftState(ac).getId());
				int alert = daa.alerting(ac);
				out.print(", "+alert);
				ConflictData det=null;
				for (int l=1; l <= daa.parameters.alertor.mostSevereAlertLevel(); ++l) {
					det = daa.detection(ac,l);
					out.print(", ");
					out.print(det.getTimeIn());
				}
				if (det != null) {
					out.print(", "+Units.to(uhor,det.get_s().norm2D()));
					out.print(", "+Units.to(uver,det.get_s().z));
					out.print(", "+Units.to(ugs,det.get_v().norm2D()));
					out.print(", "+Units.to(uvs,det.get_v().z));
					out.print(", "+Units.to(uhor,det.HMD(daa.parameters.getLookaheadTime())));
					out.print(", "+Units.to(uver,det.VMD(daa.parameters.getLookaheadTime())));
					double tcpa  = Horizontal.tcpa(det.get_s().vect2(),det.get_v().vect2());
					out.print(", "+tcpa);
					double dcpa =  Horizontal.dcpa(det.get_s().vect2(),det.get_v().vect2());
					out.print(", "+Units.to(uhor,dcpa));
					double tcoa = Vertical.time_coalt(det.get_s().z,det.get_v().z);
					out.print(", ");
					if (tcoa >= 0) {
						out.print(tcoa);
					}
					out.print(", ");
					if (detector.isPresent() && detector.get() instanceof WCV_tvar) {
						double tau_mod  = ((WCV_tvar)detector.get()).horizontal_tvar(det.get_s().vect2(),det.get_v().vect2());
						if (tau_mod > 0) {
							out.print(tau_mod);
						}						
					}
				}
				out.println();
			}
		}
		out.close();
	}
}

