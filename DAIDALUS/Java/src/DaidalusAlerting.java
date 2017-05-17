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
import java.io.FileWriter;
import java.io.PrintWriter;

import gov.nasa.larcfm.ACCoRD.Daidalus;
import gov.nasa.larcfm.ACCoRD.DaidalusFileWalker;

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
			} else if ((arga.startsWith("--conf") || arga.startsWith("-conf")) && a+1 < args.length) {
				// Load configuration file
				arga = args[++a];
				if (!daa.parameters.loadFromFile(arga)) {
					System.err.println("File "+arga+" not found");
					System.exit(0);
				} else {
					System.out.println("Loading configuration file "+arga);
				}
			} else if ((arga.startsWith("--o") || arga.startsWith("-o")) && a+1 < args.length) {
				output_file = args[++a];
			} else if (arga.startsWith("--h") || arga.startsWith("-h")) {
				System.err.println("Options: [--noma | --nomb | --conf <configuration file> | --output <output file> | --help] <input file>");
				System.exit(0);
			} else if (arga.startsWith("-")){
				System.err.println("Unknown option "+arga);
				System.exit(0);
			} else if (input_file.equals("")) {
				input_file = args[a];
			} else {
				System.err.println("Only one input file can be provided ("+a+")");
				System.exit(0);
			}				
		}
		if (input_file.equals("")) {
			System.err.println("One input file must be provided");
			System.exit(0);
		}

		// Here, have a way to read configuration file and an input file using the graphical interface
		DaidalusFileWalker walker = new DaidalusFileWalker(input_file);

		try {
			if (!output_file.equals("")) {
				out = new PrintWriter(new BufferedWriter(new FileWriter(output_file)),true);
			}
		} catch (Exception e) {
			System.out.println("ERROR: "+e);
		}    

		out.println("Time, Alerting");

		while (!walker.atEnd()) {
			walker.readState(daa);

			int alerting = -1;
			for (int i=1; i <= daa.lastTrafficIndex(); ++i) {
				alerting = Math.max(alerting,daa.alerting(i));
			}

			if (alerting > 0) {
				out.println(daa.getCurrentTime()+","+alerting);
			}
		}
		out.close();
	}
}

