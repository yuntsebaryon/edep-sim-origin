#include "kinem/EDepSimVCountFactory.hh"
#include "kinem/EDepSimHEPEVTCountFactory.hh"
#include "kinem/EDepSimHEPEVTCountGenerator.hh"

EDepSim::HEPEVTCountFactory::HEPEVTCountFactory(
    EDepSim::UserPrimaryGeneratorMessenger* parent) 
    : EDepSim::VCountFactory("hepevt",parent) {

    fInputFileCMD = std::make_unique<G4UIcmdWithAString>(CommandName("input"),this);
    fInputFileCMD->SetGuidance("Set the input file.");
    fInputFileCMD->SetParameterName("name",false);
}

void EDepSim::HEPEVTCountFactory::SetNewValue(G4UIcommand* command,
                                           G4String newValue) {
    if (command == fInputFileCMD.get()) {
        SetInputFile(newValue);
    }
    else {
        EDepSimError("Nothing to set the value.");
        EDepSimThrow("EDepSim::HEPFactory::SetNewValue(): Error");
    }
}

EDepSim::VCountGenerator* EDepSim::HEPEVTCountFactory::GetGenerator() {
    EDepSim::VCountGenerator* counter = new EDepSim::HEPEVTCountGenerator(GetName(),
                                                                GetInputFile(),
                                                                GetIntensity());
    return counter;
}

