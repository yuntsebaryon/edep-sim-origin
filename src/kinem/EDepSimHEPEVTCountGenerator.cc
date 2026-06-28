#include "kinem/EDepSimVCountGenerator.hh"
#include "kinem/EDepSimHEPEVTCountGenerator.hh"

#include <vector>
#include <limits>

constexpr int aBigNumber = std::numeric_limits<int>::max();

EDepSim::HEPEVTCountGenerator::HEPEVTCountGenerator(G4String name,
                                                    const G4String& fileName,
                                                    double intensity) 
    : EDepSim::VCountGenerator(name, intensity), fFileName(fileName) {}

EDepSim::HEPEVTCountGenerator::~HEPEVTCountGenerator() {}

int EDepSim::HEPEVTCountGenerator::GetTokens(
    std::vector<std::string>& tokens) {

    tokens.clear();
    if (!fInput.is_open() || fInput.eof()) {
        EDepSimError("No more events: " << fFileName);
        return 0;
    }

    std::string line;
    while (std::getline(fInput,line)) {
        ++fCurrentLine;
        // Strip out comments.
        line = line.substr(0,line.find_first_of("#"));
        // Break into tokens.
        std::istringstream input(line);
        std::string token;
        tokens.clear();
        while (input >> token) tokens.push_back(token);
        if (!tokens.empty()) return tokens.size();
    }

    EDepSimLog("No more data in " << fFileName);
    return 0;
}

int EDepSim::HEPEVTCountGenerator::AsInteger(const std::string& token) {
    std::istringstream input(token);
    int value;
    input >> value;
    std::string trailer;
    if (input >> trailer) {
        EDepSimThrow(fFileName << ":" << fCurrentLine
                     << " -- Invalid integer: |" << token<< "|");
    }
    return value;
}

int EDepSim::HEPEVTCountGenerator::GetCount() {

    if (!fInput.is_open()) {
        fInput.open(fFileName,std::ifstream::in);
        fCurrentLine = 0;
        if (!fInput.is_open()) {
            EDepSimThrow("File not open: " << fFileName);
        }
        EDepSimNamedLog("HEPEVT", "Open HEPEVT file: " << fFileName);
        fNextEvent = NoEvent;
    }

#if 0
    fNVertices = 0;
    std::vector<std::string> tokens;
    int event_id  = 0;
    int vertex_id = 0;
    int lines     = 0;
    
    
    // Parse the header line for an interaction. Depending on the input
    // flavor, this line may contain any number of the following: event ID,
    // vertex ID, number of particles, and vertex (x,y,z,t). The number
    // of particles is the only required field. Vertex positions should
    // be given in cm, and the time should be in ns.
    if (!GetTokens(tokens)) {
        EDepSimLog("No more events in " << fFileName
                   << " after " << fCurrentLine << " lines");
        throw EDepSim::NoMoreEvents();
    }
    switch (tokens.size()) {
    // For pythia flavor input, the number of particle in the event
    // is the only token.
    case 1: case 2: case 5:
        EDepSimError("Format not supported in " << fFileName << ":" << fCurrentLine);
        throw EDepSim::NoMoreEvents();
    // The next two cases use the marley (15-number) input flavor
    // For multi-vertex events, separate each vertex with a line
    // containing three numbers: eventID, vertexID, and the number
    // of particles in that vertex
    case 3:
        event_id  = AsInteger(tokens[0]);
        vertex_id = AsInteger(tokens[1]);
        lines     = AsInteger(tokens[2]);
        break;
    // Alternative header format for multi-vertex events:
    // event ID, vertex ID, number of particles in that vertex,
    // and vertex (x,y,z,t)
    case 7:
        event_id  = AsInteger(tokens[0]);
        vertex_id = AsInteger(tokens[1]);
        lines     = AsInteger(tokens[2]);
        break;
    default:
        EDepSimError("Syntax error at " << fFileName << ":" << fCurrentLine);
        throw EDepSim::NoMoreEvents();
    }

    for (int line = 0; line < lines; ++line) {
        fInput.ignore(aBigNumber, '\n');
        ++fCurrentLine;
    }
#else
    
    // only for the first event in each input file:
    if (fNextEvent == NoEvent) fNextEvent = readOneVertex();

    if (fNextEvent == NoEvent) { // we have no more event in the input file
        throw EDepSim::NoMoreEvents();
    }

    EDepSimLog("Looking for the end of event " << fNextEvent);

    int nVertices = 0;
    int nextEventID = NoEvent;
    do {
        ++nVertices;
        nextEventID = readOneVertex();
    } while (nextEventID == fNextEvent);

    EDepSimLog("Found " << nVertices << " vertices in event " << fNextEvent);

    // note: the first vertex of the next event is already read
    fNextEvent = nextEventID;

#endif

    return nVertices;
}


int EDepSim::HEPEVTCountGenerator::readOneVertex() {

    std::vector<std::string> tokens;
    int event_id  = 0;
    int vertex_id = 0;
    int lines     = 0;
    
    
    // Parse the header line for an interaction. Depending on the input
    // flavor, this line may contain any number of the following: event ID,
    // vertex ID, number of particles, and vertex (x,y,z,t). The number
    // of particles is the only required field. Vertex positions should
    // be given in cm, and the time should be in ns.
    if (!GetTokens(tokens)) {
        EDepSimLog("No more events in " << fFileName
                   << " after " << fCurrentLine << " lines");
        return NoEvent;
    }
    switch (tokens.size()) {
    // For pythia flavor input, the number of particle in the event
    // is the only token.
    case 1: case 2: case 5:
        EDepSimError("Format not supported in " << fFileName << ":" << fCurrentLine);
        throw EDepSim::NoMoreEvents();
    // The next two cases use the marley (15-number) input flavor
    // For multi-vertex events, separate each vertex with a line
    // containing three numbers: eventID, vertexID, and the number
    // of particles in that vertex
    case 3:
        event_id  = AsInteger(tokens[0]);
        vertex_id = AsInteger(tokens[1]);
        lines     = AsInteger(tokens[2]);
        break;
    // Alternative header format for multi-vertex events:
    // event ID, vertex ID, number of particles in that vertex,
    // and vertex (x,y,z,t)
    case 7:
        event_id  = AsInteger(tokens[0]);
        vertex_id = AsInteger(tokens[1]);
        lines     = AsInteger(tokens[2]);
        break;
    default:
        EDepSimError("Syntax error at " << fFileName << ":" << fCurrentLine);
        throw EDepSim::NoMoreEvents();
    }

    for (int line = 0; line < lines; ++line) {
        fInput.ignore(aBigNumber, '\n');
        if (fInput.eof()) {
            EDepSimError("Event " << fNextEvent << " ended after " << line
                 << " particles (" << lines << " expected) at " << fFileName << ":" << fCurrentLine);
            break;
        }
        ++fCurrentLine;
    }

    return event_id;
} // readOneVertex()
