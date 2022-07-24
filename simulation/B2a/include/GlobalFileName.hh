/// \file E75FileName.hh
/// \brief Implementation of the E75FileName class

#ifndef GlobalFileName_h
#define GlobalFileName_h 1

void GlobalSetFileName(char* argv2, char* argv3, char* argv4, char* argv5, char* argv6);
std::string GetfilenameGlo();

int GlobalGettargetOnIGlo();
double GlobalGetmomDGlo();
double GlobalGetthetaDGlo();
double GlobalGetphiDGlo();
int GlobalGetmagneticFieldOnIGlo();

void GlobalSetrandam(char *argv7);
void GlobalSetrandamEmp();
int GlobalGetrandamIGlo();
#endif
