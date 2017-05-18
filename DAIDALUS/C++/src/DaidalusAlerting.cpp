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

#include "Daidalus.h"
#include "DaidalusFileWalker.h"

using namespace larcfm;

int main(int argc, char* argv[]) {

	// Create a Daidalus object for an unbuffered well-clear volume and instantaneous bands
	Daidalus daa;
	std::string input_file = "";
	std::string output_file = "";
	std::ostream* out = &std::cout;

	// A Daidalus object can be configured either programatically or by using a configuration file.
	for (int a=1;a < argc; ++a) {
		std::string arga = argv[a];
		if (arga == "--noma" || arga == "-noma") {
			// Configure DAIDALUS to nominal A parameters: Kinematic Bands, Turn Rate 1.5 [deg/s])
			daa.set_Buffered_WC_SC_228_MOPS(false);
		} else if (arga == "--nomb" || arga == "-nomb") {
			// Configure DAIDALUS to nominal B parameters: Kinematic Bands, Turn Rate 3.0 [deg/s])
			daa.set_Buffered_WC_SC_228_MOPS(true);
		} else if ((startsWith(arga,"--c") || startsWith(arga,"-c"))  && a+1 < argc) {
			// Load configuration file
			arga = argv[++a];
			if (!daa.parameters.loadFromFile(arga)) {
				std::cerr << "File " << arga << "not found" << std::endl;
				exit(0);
			} else {
				std::cout << "Loading configuration file " << arga << std::endl;
			}
		} else if ((startsWith(arga,"--o") || startsWith(arga,"-o")) && a+1 < argc) {
			output_file = argv[++a];
		} else if (startsWith(arga,"--h") || startsWith(arga,"-h")) {
			std::cerr << "Options: [--noma | --nomb | --conf <configuration file> | --output <output file> | --help] <input file>"
					<< std::endl;
			exit(0);
		} else if (startsWith(arga,"-")){
			std::cerr << "Unknown option " << arga << std::endl;
			exit(0);
		} else if (input_file == "") {
			input_file = argv[a];
		} else {
			std::cerr << "Only one input file can be provided (" << a << ")" << std::endl;
			exit(0);
		}
	}
	if (input_file == "") {
		std::cerr << "One input file must be provided" << std::endl;
		exit(0);
	}

	std::cout << "Processing file " << input_file << std::endl;
	DaidalusFileWalker walker(input_file);

	std::ofstream fout;

	if (output_file != "") {
		fout.open(output_file.c_str());
		out = &fout;
		std::cout << "Output file: " << output_file << std::endl;
	}

	(*out) << "Time, Alerting" << std::endl;

	while (!walker.atEnd()) {
		walker.readState(daa);
		// At this point, daa has the state information of ownhsip and traffic for a given time
		int alerting = -1;
		for (int i=1; i <= daa.lastTrafficIndex(); ++i) {
			alerting = std::max(alerting,daa.alerting(i));
		}

		if (alerting > 0) {
			(*out) << daa.getCurrentTime() << "," << alerting << std::endl;
		}
	}
	if (output_file != "") {
		fout.close();
	}

}



