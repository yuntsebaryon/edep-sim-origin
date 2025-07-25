#ifndef EDepSim_NoMoreEvents_hh_Seen
#define EDepSim_NoMoreEvents_hh_Seen

#include "EDepSimException.hh"

/// The exception to be thrown when the generator can't make any more events.
namespace EDepSim {class NoMoreEvents;}
class EDepSim::NoMoreEvents : public EDepSim::Exception {
public:
    NoMoreEvents() {}
    ~NoMoreEvents() throw() {}
    const char* what(void) const throw() {return "EDepSim::NoMoreEvents";}
};

#endif
