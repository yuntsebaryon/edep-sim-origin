#ifndef EDepSim_HEPEVTCountFactory_hh_seen
#define EDepSim_HEPEVTCountFactory_hh_seen

#include "kinem/EDepSimVCountFactory.hh"

#include <memory>

namespace EDepSim {class HEPEVTCountFactory;}
class EDepSim::HEPEVTCountFactory : public EDepSim::VCountFactory {
public:
    HEPEVTCountFactory(EDepSim::UserPrimaryGeneratorMessenger* parent);

    /// Return the mean count generator.
    EDepSim::VCountGenerator* GetGenerator();

    /// Set the input file to read.
    virtual void SetInputFile(const G4String& name) {fInputFile=name;}

    /// Get the input file to read.
    virtual const G4String& GetInputFile() const {return fInputFile;}

    /// Handle messages from the UI processor.
    void SetNewValue(G4UIcommand*, G4String);

private:
    /// The text file of HEPEVT records.
    G4String fInputFile;

    /// A command to get the file name to read for the HEPEVT records.
    std::unique_ptr<G4UIcmdWithAString> fInputFileCMD;

};

#endif
