#ifndef EDepSim_HEPEVTCountGenerator_hh_seen
#define EDepSim_HEPEVTCountGenerator_hh_seen

#include <string>
#include <fstream>

#include "kinem/EDepSimVCountGenerator.hh"
#include "kinem/EDepSimNoMoreEvents.hh"


namespace EDepSim {class HEPEVTCountGenerator;}
class EDepSim::HEPEVTCountGenerator : public EDepSim::VCountGenerator {
public:
    HEPEVTCountGenerator(G4String name, const G4String& fileName, double intensity);
    virtual ~HEPEVTCountGenerator();

    /// Return the number of events to generate.
    int GetCount();

private:
    static constexpr int NoEvent = -1; ///< Magic value: invalid event ID.

    /// The name of the input file (used for error messages)
    std::string fFileName;

    /// The input stream to be read.
    std::ifstream fInput;
    
     /// The ID of the next event in the current input file.
    int fNextEvent = NoEvent;

    /// The number of lines read from the input file (used for error messages)
    int fCurrentLine;

    /// Reads one vertex from the current input file and returns its ID.
    /// If no vertex is available, returns `NoEvent`.
    int readOneVertex();

    /// Copied from EDepSim::HEPEVTKinematicsGenerator
    /// Get the next line of "tokens" from the input file.  This returns the
    /// number of tokens that were read from the line, and zero when the file
    /// is empty.  Comments in the file are prefixed by "#", and tokens are
    /// separated by white space.  The expected format is
    /// \code
    /// token1 token2 token3 and so on # comments
    /// \endcode
    /// Lines that are empty, or only have comments are skipped,
    int GetTokens(std::vector<std::string>& tokens);

    /// Parse a token as an integer.  This will throw an error if the string
    /// is not a valid integer.
    int AsInteger(const std::string& token);

};
#endif
