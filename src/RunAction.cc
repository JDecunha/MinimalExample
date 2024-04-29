//This project
#include "RunAction.hh"
//Geant4
#include "G4SystemOfUnits.hh"
#include "G4AnalysisManager.hh"
#include "G4Run.hh"
#include "G4Threading.hh"


RunAction::RunAction()
{
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->SetVerboseLevel(1);
  analysisManager->SetNtupleMerging(true);

  //Set scoring geometry parameters (the parallel world pulls these parameters to ensure consistency between histograms and scoring geometry)
  _scoringHalfLength = 5*cm;
  _scoringResolution = 0.1*mm;
  _numBins = (_scoringHalfLength*2.)/_scoringResolution;


  //Creating histograms
  //Dose scorer
  analysisManager->CreateH1("Dose" ,"Dose along phantom profile", _numBins, 0, _numBins);

}

void RunAction::BeginOfRunAction(const G4Run* /*run*/)
{

}

void RunAction::EndOfRunAction(const G4Run*)
{
  auto analysisManager = G4AnalysisManager::Instance();

  analysisManager->Write();
  analysisManager->CloseFile();
}
