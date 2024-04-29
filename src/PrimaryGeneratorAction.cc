//MicroTrackGenerator
#include "DetectorConstruction.hh"
#include "PrimaryGeneratorAction.hh"
//Geant4
#include "G4SystemOfUnits.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction()
{
    //Create the gun and messenger
    gun = new G4ParticleGun();

    //Set default values
    gun->SetParticlePosition(G4ThreeVector(0,0,48*cm)); //Set the primary particle origin position 
    gun->SetParticleMomentumDirection(G4ThreeVector(0,0,-1)); //Set appropriate direction (down in Z axis)
    gun->SetParticleEnergy(80*MeV);
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
    delete gun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* event)
{
    auto particleTable = G4ParticleTable::GetParticleTable();
    G4ParticleDefinition* particle = nullptr;
    particle = particleTable->FindParticle("proton");
    gun->SetParticleDefinition(particle);

    gun->SetParticlePosition(G4ThreeVector(0,0,48*cm)); //Set the primary particle origin position 
    gun->SetParticleMomentumDirection(G4ThreeVector(0,0,-1)); //Set appropriate direction (down in Z axis)
    gun->SetParticleEnergy(80*MeV);

    gun->GeneratePrimaryVertex(event);
}
