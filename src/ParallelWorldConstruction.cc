//Geant4
#include "G4SystemOfUnits.hh"
//This project
#include "ParallelWorldConstruction.hh"
#include "StraightLineParameterisation.hh"
#include "RunAction.hh"
//This project: Scorers
#include "EdepScorer.hh"
//Geant4
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVParameterised.hh"
#include "G4SDManager.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4RunManager.hh"
#include "G4VisAttributes.hh"

ParallelWorldConstruction::ParallelWorldConstruction(const G4String& parallelWorldName): G4VUserParallelWorld(parallelWorldName) { }

void ParallelWorldConstruction::Construct() 
{
  //Get the world pointers (required for parallel worlds)
  G4VPhysicalVolume* motherWorldPointer = GetWorld();
  G4LogicalVolume* motherWorldLogical = motherWorldPointer->GetLogicalVolume();

  //Get the RunAction pointer so that you can ensure the output histograms have a number of bins that corresponds 
  //to the desired number of voxels
  RunAction* pRunAction = ((RunAction*)G4RunManager::GetRunManager()->GetUserRunAction());

  //Define G4Box that the parameterisation will place many copies of
  G4double xHalfSize = 2*cm; 
  G4double yHalfSize = 2*cm;
  G4double zHalfSize = pRunAction->GetScoringHalfLength();

  //
  // Normal scoring box
  //

  G4Box* scoringBox = new G4Box("scoringBox", xHalfSize, yHalfSize, zHalfSize);
  G4LogicalVolume * scoringBox_logical = new G4LogicalVolume(scoringBox, 0, "scoringBox_logical", 0, 0, 0);
  G4ThreeVector phantomOffset = G4ThreeVector(0, 0, ((-5*cm)));
  G4VPhysicalVolume* scoringBox_physical = new G4PVPlacement(0, phantomOffset, scoringBox_logical, "scoringBox_logical", motherWorldLogical, false, 0, false);

  //Visualize scoring box
  G4VisAttributes* scoring_vis = new G4VisAttributes();
  scoring_vis->SetColor(1, 0, 0);
  scoring_vis->SetVisibility(true);
  scoringBox_logical->SetVisAttributes(scoring_vis);

  //Slice up the scoring box
  G4double zAxisResolution = pRunAction->GetScoringResolution();
  G4int zIncrements = (zHalfSize*2.)/zAxisResolution; //Set zIncrements according to desired resolution 
  G4int xIncrements = 1; G4int yIncrements = 1;
  G4int numVoxels = xIncrements*yIncrements*zIncrements;

  G4Box* ScoringZCuts = new G4Box("ScoringZCuts", 1*cm, 1*cm, (zAxisResolution/2.));
  G4LogicalVolume* ScoringZCuts_log = new G4LogicalVolume(ScoringZCuts, 0,"ScoringZCutsLog");
  G4VPhysicalVolume* ScoringZCuts_physical = new G4PVReplica("ScoringZCutsPhysical",
                                                              ScoringZCuts_log,
                                                              scoringBox_physical,
                                                              kZAxis,
                                                              zIncrements,
                                                              zAxisResolution);

}

void ParallelWorldConstruction::ConstructSD() 
{
  //To pull the number of bins
  RunAction* pRunAction = ((RunAction*)G4RunManager::GetRunManager()->GetUserRunAction());

  //Set up the scoring
  G4SDManager* SDManager = G4SDManager::GetSDMpointer();
  G4MultiFunctionalDetector* MultiFuncDetector = new G4MultiFunctionalDetector("multifuncdetector1");

  //Make the scorers
  G4VPrimitiveScorer* edepScorer;
  edepScorer = new EdepScorer("edep",0);
  MultiFuncDetector->RegisterPrimitive(edepScorer);

  //Register sensitive detector with SDManager, and register SD with logical volume
  SDManager->AddNewDetector(MultiFuncDetector);
  SetSensitiveDetector("ScoringZCutsLog",MultiFuncDetector);
}
