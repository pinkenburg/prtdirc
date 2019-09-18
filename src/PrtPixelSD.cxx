#include "PrtPixelSD.h"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4RunManager.hh"
#include <TVector3.h>
#include "G4TransportationManager.hh"

#include "PrtEvent.h"
#include "PrtPrizmHit.h"

#include "PrtRunAction.h"
#include "PrtManager.h"

PrtPixelSD::PrtPixelSD( const G4String& name, 
			const G4String& hitsCollectionName,
			G4int nofCells)
  : G4VSensitiveDetector(name){
  collectionName.insert(hitsCollectionName);
}

PrtPixelSD::~PrtPixelSD(){ 
}

void PrtPixelSD::Initialize(G4HCofThisEvent* hce){
  
  memset(fMultHit, 0, sizeof(fMultHit[0][0])*960);
  
  Double_t qe_m2015[15][64]={{0.63,0.78,0.73,0.69,0.67,0.67,0.69,0.60,0.73,0.82,0.78,0.76,0.75,0.77,0.82,0.78,0.79,0.85,0.78,0.76,0.75,0.77,0.82,0.79,0.81,0.85,0.78,0.76,0.75,0.77,0.83,0.80,0.81,0.84,0.77,0.74,0.74,0.77,0.83,0.79,0.80,0.81,0.76,0.73,0.74,0.76,0.82,0.78,0.80,0.79,0.75,0.74,0.76,0.78,0.83,0.79,0.60,0.76,0.73,0.72,0.73,0.76,0.79,0.70}, //765
			     {0.61,0.69,0.70,0.70,0.70,0.69,0.69,0.61,0.77,0.82,0.83,0.83,0.83,0.82,0.81,0.76,0.77,0.83,0.84,0.84,0.84,0.84,0.82,0.77,0.78,0.84,0.85,0.85,0.85,0.84,0.83,0.78,0.78,0.84,0.85,0.85,0.85,0.84,0.83,0.79,0.77,0.83,0.85,0.85,0.85,0.85,0.83,0.80,0.76,0.83,0.84,0.85,0.85,0.84,0.83,0.81,0.71,0.82,0.84,0.84,0.84,0.84,0.83,0.80}, //1352
			     {0.66,0.66,0.66,0.67,0.66,0.66,0.67,0.62,0.72,0.71,0.72,0.77,0.77,0.77,0.79,0.77,0.72,0.71,0.71,0.74,0.77,0.77,0.79,0.76,0.71,0.71,0.70,0.70,0.75,0.76,0.79,0.75,0.71,0.70,0.70,0.69,0.70,0.73,0.77,0.74,0.71,0.70,0.70,0.69,0.69,0.69,0.72,0.72,0.71,0.71,0.70,0.69,0.69,0.68,0.69,0.68,0.68,0.71,0.70,0.69,0.68,0.68,0.69,0.65}, //1358
			     {0.79,0.86,0.85,0.84,0.84,0.83,0.82,0.77,0.85,0.90,0.90,0.89,0.89,0.89,0.88,0.84,0.85,0.90,0.90,0.90,0.90,0.89,0.88,0.84,0.85,0.90,0.90,0.90,0.89,0.89,0.88,0.84,0.86,0.91,0.91,0.90,0.89,0.89,0.88,0.83,0.86,0.91,0.90,0.90,0.89,0.89,0.87,0.84,0.86,0.90,0.90,0.89,0.89,0.88,0.87,0.83,0.83,0.86,0.86,0.86,0.86,0.85,0.85,0.82}, //1392
			     {0.46,0.52,0.52,0.52,0.52,0.52,0.52,0.48,0.75,0.82,0.83,0.83,0.83,0.83,0.82,0.79,0.76,0.83,0.84,0.84,0.84,0.84,0.83,0.80,0.75,0.83,0.84,0.84,0.84,0.84,0.83,0.80,0.76,0.83,0.84,0.84,0.84,0.84,0.83,0.80,0.74,0.83,0.84,0.84,0.84,0.84,0.83,0.79,0.75,0.83,0.84,0.84,0.84,0.84,0.83,0.80,0.74,0.82,0.83,0.83,0.83,0.83,0.82,0.79}, //1359
			     {0.66,0.70,0.71,0.70,0.71,0.70,0.70,0.56,0.78,0.81,0.81,0.81,0.82,0.82,0.82,0.68,0.78,0.81,0.82,0.81,0.82,0.82,0.82,0.69,0.77,0.81,0.82,0.82,0.82,0.82,0.82,0.69,0.77,0.82,0.82,0.82,0.82,0.82,0.82,0.68,0.77,0.82,0.82,0.82,0.83,0.83,0.82,0.69,0.77,0.82,0.82,0.82,0.83,0.83,0.82,0.71,0.71,0.77,0.75,0.76,0.76,0.76,0.76,0.64}, //1356
			     {0.50,0.63,0.64,0.64,0.64,0.65,0.65,0.56,0.72,0.90,0.92,0.91,0.89,0.90,0.89,0.74,0.72,0.92,0.93,0.93,0.93,0.93,0.91,0.76,0.71,0.92,0.94,0.94,0.94,0.93,0.92,0.77,0.70,0.92,0.94,0.94,0.94,0.93,0.92,0.77,0.69,0.92,0.94,0.94,0.93,0.93,0.91,0.77,0.66,0.89,0.92,0.92,0.92,0.91,0.89,0.76,0.46,0.74,0.75,0.76,0.77,0.77,0.75,0.63}, //1357
			     {0.62,0.68,0.68,0.71,0.71,0.70,0.69,0.52,0.70,0.76,0.77,0.80,0.81,0.80,0.79,0.65,0.71,0.76,0.77,0.81,0.81,0.81,0.80,0.67,0.71,0.76,0.76,0.80,0.82,0.81,0.80,0.69,0.70,0.75,0.76,0.78,0.81,0.81,0.80,0.69,0.70,0.75,0.76,0.76,0.80,0.81,0.80,0.70,0.71,0.75,0.75,0.75,0.76,0.79,0.79,0.70,0.57,0.62,0.62,0.62,0.62,0.62,0.63,0.56}, //1360
			     {0.57,0.69,0.70,0.72,0.73,0.73,0.72,0.66,0.76,0.89,0.90,0.91,0.91,0.91,0.89,0.84,0.77,0.90,0.92,0.93,0.93,0.92,0.91,0.84,0.77,0.91,0.92,0.93,0.93,0.92,0.91,0.85,0.77,0.91,0.92,0.93,0.93,0.92,0.91,0.85,0.76,0.90,0.92,0.93,0.93,0.92,0.91,0.84,0.74,0.89,0.91,0.91,0.92,0.91,0.89,0.83,0.64,0.78,0.79,0.82,0.83,0.83,0.82,0.76}, //1355
			     {0.74,0.77,0.79,0.82,0.78,0.65,0.78,0.74,0.79,0.81,0.80,0.86,0.82,0.75,0.82,0.70,0.80,0.82,0.81,0.83,0.83,0.69,0.82,0.80,0.81,0.82,0.81,0.81,0.79,0.78,0.87,0.84,0.79,0.82,0.81,0.80,0.80,0.79,0.90,0.73,0.79,0.80,0.81,0.80,0.78,0.82,0.97,0.80,0.82,0.81,0.81,0.80,0.80,0.80,0.93,0.83,0.81,0.83,0.83,0.81,0.79,0.80,0.93,0.81}, //1353
			     {0.75,0.91,0.90,0.90,0.89,0.87,0.86,0.75,0.87,0.98,0.99,0.99,0.99,0.99,0.98,0.90,0.89,0.99,1.00,1.00,1.00,1.00,0.99,0.92,0.91,0.99,1.00,1.01,1.01,1.01,1.00,0.92,0.92,0.99,1.00,1.01,1.01,1.01,0.99,0.92,0.91,0.98,1.00,1.00,1.00,1.00,0.99,0.91,0.91,0.97,0.98,0.99,0.99,0.99,0.98,0.89,0.65,0.85,0.88,0.91,0.92,0.94,0.94,0.82}, //1338
			     {0.77,0.85,0.81,0.84,0.83,0.83,0.83,0.72,0.88,0.93,0.92,0.91,0.90,0.90,0.89,0.83,0.87,0.94,0.93,0.91,0.91,0.90,0.90,0.85,0.88,0.94,0.93,0.91,0.91,0.91,0.90,0.86,0.87,0.94,0.93,0.92,0.91,0.91,0.90,0.86,0.87,0.94,0.92,0.91,0.91,0.90,0.90,0.87,0.85,0.93,0.92,0.90,0.90,0.89,0.89,0.86,0.75,0.86,0.84,0.82,0.81,0.80,0.82,0.76}, //411
			     {0.67,0.77,0.78,0.78,0.78,0.78,0.78,0.70,0.80,0.92,0.93,0.93,0.93,0.93,0.92,0.83,0.80,0.93,0.94,0.94,0.94,0.94,0.93,0.84,0.79,0.94,0.95,0.95,0.95,0.94,0.93,0.85,0.77,0.94,0.95,0.95,0.95,0.95,0.93,0.85,0.76,0.94,0.95,0.95,0.95,0.94,0.93,0.85,0.75,0.93,0.94,0.94,0.94,0.93,0.92,0.85,0.67,0.84,0.84,0.84,0.84,0.84,0.83,0.75}, //1339
			     {0.68,0.80,0.79,0.79,0.77,0.76,0.75,0.62,0.76,0.88,0.89,0.89,0.89,0.89,0.88,0.74,0.77,0.89,0.90,0.90,0.90,0.90,0.89,0.75,0.78,0.89,0.90,0.90,0.91,0.90,0.89,0.77,0.78,0.89,0.90,0.90,0.90,0.90,0.89,0.77,0.78,0.88,0.89,0.90,0.90,0.89,0.88,0.78,0.78,0.87,0.88,0.89,0.89,0.88,0.87,0.78,0.64,0.75,0.75,0.76,0.76,0.76,0.75,0.65}, //1336
			     {0.66,0.80,0.80,0.80,0.80,0.79,0.79,0.66,0.76,0.88,0.87,0.88,0.89,0.89,0.90,0.79,0.76,0.88,0.87,0.88,0.89,0.90,0.90,0.79,0.76,0.87,0.87,0.87,0.89,0.90,0.90,0.79,0.77,0.86,0.86,0.87,0.88,0.89,0.90,0.79,0.75,0.86,0.85,0.86,0.87,0.88,0.90,0.79,0.76,0.85,0.84,0.84,0.86,0.87,0.89,0.78,0.54,0.64,0.64,0.70,0.74,0.76,0.78,0.63}  //766
  };
  
  Double_t qe_m2016[9][64]={{0.75,0.91,0.90,0.90,0.89,0.87,0.86,0.75,0.87,0.98,0.99,0.99,0.99,0.99,0.98,0.90,0.89,0.99,1.00,1.00,1.00,1.00,0.99,0.92,0.91,0.99,1.00,1.01,1.01,1.01,1.00,0.92,0.92,0.99,1.00,1.01,1.01,1.01,0.99,0.92,0.91,0.98,1.00,1.00,1.00,1.00,0.99,0.91,0.91,0.97,0.98,0.99,0.99,0.99,0.98,0.89,0.65,0.85,0.88,0.91,0.92,0.94,0.94,0.82}, //1338
			    {0.74,0.77,0.79,0.82,0.78,0.65,0.78,0.74,0.79,0.81,0.80,0.86,0.82,0.75,0.82,0.70,0.80,0.82,0.81,0.83,0.83,0.69,0.82,0.80,0.81,0.82,0.81,0.81,0.79,0.78,0.87,0.84,0.79,0.82,0.81,0.80,0.80,0.79,0.90,0.73,0.79,0.80,0.81,0.80,0.78,0.82,0.97,0.80,0.82,0.81,0.81,0.80,0.80,0.80,0.93,0.83,0.81,0.83,0.83,0.81,0.79,0.80,0.93,0.81}, //1353
			    {0.50,0.63,0.64,0.64,0.64,0.65,0.65,0.56,0.72,0.90,0.92,0.91,0.89,0.90,0.89,0.74,0.72,0.92,0.93,0.93,0.93,0.93,0.91,0.76,0.71,0.92,0.94,0.94,0.94,0.93,0.92,0.77,0.70,0.92,0.94,0.94,0.94,0.93,0.92,0.77,0.69,0.92,0.94,0.94,0.93,0.93,0.91,0.77,0.66,0.89,0.92,0.92,0.92,0.91,0.89,0.76,0.46,0.74,0.75,0.76,0.77,0.77,0.75,0.63}, //1357
			    {0.61,0.69,0.70,0.70,0.70,0.69,0.69,0.61,0.77,0.82,0.83,0.83,0.83,0.82,0.81,0.76,0.77,0.83,0.84,0.84,0.84,0.84,0.82,0.77,0.78,0.84,0.85,0.85,0.85,0.84,0.83,0.78,0.78,0.84,0.85,0.85,0.85,0.84,0.83,0.79,0.77,0.83,0.85,0.85,0.85,0.85,0.83,0.80,0.76,0.83,0.84,0.85,0.85,0.84,0.83,0.81,0.71,0.82,0.84,0.84,0.84,0.84,0.83,0.80}, //1352
			    {0.57,0.69,0.70,0.72,0.73,0.73,0.72,0.66,0.76,0.89,0.90,0.91,0.91,0.91,0.89,0.84,0.77,0.90,0.92,0.93,0.93,0.92,0.91,0.84,0.77,0.91,0.92,0.93,0.93,0.92,0.91,0.85,0.77,0.91,0.92,0.93,0.93,0.92,0.91,0.85,0.76,0.90,0.92,0.93,0.93,0.92,0.91,0.84,0.74,0.89,0.91,0.91,0.92,0.91,0.89,0.83,0.64,0.78,0.79,0.82,0.83,0.83,0.82,0.76}, //1355
			    {0.79,0.86,0.85,0.84,0.84,0.83,0.82,0.77,0.85,0.90,0.90,0.89,0.89,0.89,0.88,0.84,0.85,0.90,0.90,0.90,0.90,0.89,0.88,0.84,0.85,0.90,0.90,0.90,0.89,0.89,0.88,0.84,0.86,0.91,0.91,0.90,0.89,0.89,0.88,0.83,0.86,0.91,0.90,0.90,0.89,0.89,0.87,0.84,0.86,0.90,0.90,0.89,0.89,0.88,0.87,0.83,0.83,0.86,0.86,0.86,0.86,0.85,0.85,0.82}, //1356
			    {0.62,0.68,0.68,0.71,0.71,0.70,0.69,0.52,0.70,0.76,0.77,0.80,0.81,0.80,0.79,0.65,0.71,0.76,0.77,0.81,0.81,0.81,0.80,0.67,0.71,0.76,0.76,0.80,0.82,0.81,0.80,0.69,0.70,0.75,0.76,0.78,0.81,0.81,0.80,0.69,0.70,0.75,0.76,0.76,0.80,0.81,0.80,0.70,0.71,0.75,0.75,0.75,0.76,0.79,0.79,0.70,0.57,0.62,0.62,0.62,0.62,0.62,0.63,0.56}, //1360
			    {0.46,0.52,0.52,0.52,0.52,0.52,0.52,0.48,0.75,0.82,0.83,0.83,0.83,0.83,0.82,0.79,0.76,0.83,0.84,0.84,0.84,0.84,0.83,0.80,0.75,0.83,0.84,0.84,0.84,0.84,0.83,0.80,0.76,0.83,0.84,0.84,0.84,0.84,0.83,0.80,0.74,0.83,0.84,0.84,0.84,0.84,0.83,0.79,0.75,0.83,0.84,0.84,0.84,0.84,0.83,0.80,0.74,0.82,0.83,0.83,0.83,0.83,0.82,0.79}, //1359
			    {0.66,0.80,0.80,0.80,0.80,0.79,0.79,0.66,0.76,0.88,0.87,0.88,0.89,0.89,0.90,0.79,0.76,0.88,0.87,0.88,0.89,0.90,0.90,0.79,0.76,0.87,0.87,0.87,0.89,0.90,0.90,0.79,0.77,0.86,0.86,0.87,0.88,0.89,0.90,0.79,0.75,0.86,0.85,0.86,0.87,0.88,0.90,0.79,0.76,0.85,0.84,0.84,0.86,0.87,0.89,0.78,0.54,0.64,0.64,0.70,0.74,0.76,0.78,0.63}  //1339
  };

  // not corrected for rotation
  // Double_t qe_m2017[14][64]={
  //   // Photonis XP85012  9001357
  //   {0.42,0.55,0.56,0.56,0.55,0.57,0.56,0.42,0.66,0.83,0.85,0.84,0.83,0.84,0.82,0.63,0.65,0.85,0.87,0.87,0.87,0.87,0.85,0.65,0.62,0.85,0.87,0.88,0.87,0.87,0.85,0.66,0.60,0.85,0.87,0.87,0.87,0.86,0.85,0.62,0.59,0.85,0.86,0.86,0.86,0.86,0.84,0.61,0.58,0.81,0.84,0.84,0.84,0.83,0.81,0.61,0.37,0.63,0.65,0.65,0.66,0.66,0.63,0.46},
  //   // Photonis XP85012  9001352
  //   {0.65,0.77,0.78,0.79,0.79,0.78,0.76,0.67,0.72,0.83,0.84,0.84,0.84,0.83,0.82,0.77,0.75,0.84,0.85,0.86,0.86,0.85,0.84,0.79,0.77,0.85,0.86,0.87,0.87,0.86,0.85,0.81,0.78,0.85,0.86,0.87,0.87,0.86,0.85,0.80,0.76,0.85,0.86,0.87,0.86,0.86,0.84,0.78,0.77,0.84,0.85,0.85,0.85,0.85,0.83,0.77,0.63,0.75,0.80,0.81,0.82,0.82,0.81,0.71},
  //   // Photonis XP85012  9001358
  //   {0.63,0.67,0.69,0.72,0.73,0.74,0.73,0.62,0.71,0.74,0.75,0.82,0.82,0.81,0.84,0.75,0.71,0.73,0.75,0.76,0.81,0.81,0.84,0.75,0.71,0.73,0.74,0.76,0.80,0.80,0.84,0.77,0.71,0.72,0.74,0.74,0.75,0.77,0.81,0.77,0.71,0.72,0.74,0.74,0.73,0.73,0.76,0.72,0.70,0.72,0.74,0.74,0.73,0.72,0.73,0.67,0.66,0.71,0.72,0.72,0.71,0.70,0.71,0.63},
  //   // Photonis XP85012  9001356
  //   {0.61,0.72,0.72,0.71,0.69,0.68,0.70,0.66,0.76,0.89,0.88,0.88,0.89,0.89,0.88,0.78,0.78,0.90,0.90,0.89,0.90,0.90,0.89,0.79,0.79,0.90,0.90,0.90,0.91,0.91,0.90,0.76,0.80,0.91,0.91,0.91,0.91,0.91,0.90,0.75,0.80,0.91,0.91,0.91,0.91,0.91,0.90,0.76,0.80,0.91,0.91,0.91,0.91,0.91,0.90,0.79,0.65,0.80,0.80,0.81,0.81,0.78,0.77,0.67},
  //   // Photonis XP85012  9001336
  //   {0.58,0.65,0.65,0.64,0.65,0.66,0.65,0.57,0.71,0.81,0.82,0.82,0.82,0.81,0.80,0.71,0.72,0.82,0.83,0.83,0.83,0.83,0.82,0.70,0.74,0.82,0.83,0.83,0.83,0.83,0.82,0.71,0.76,0.82,0.83,0.83,0.83,0.83,0.82,0.72,0.77,0.81,0.82,0.83,0.83,0.83,0.82,0.73,0.77,0.80,0.81,0.82,0.82,0.82,0.81,0.72,0.69,0.75,0.76,0.77,0.77,0.76,0.76,0.66},
  //   // Photonis XP85012  9001339
  //   {0.61,0.72,0.75,0.76,0.77,0.77,0.76,0.66,0.73,0.83,0.83,0.84,0.84,0.84,0.83,0.75,0.75,0.84,0.85,0.85,0.86,0.86,0.85,0.76,0.75,0.85,0.86,0.86,0.86,0.86,0.85,0.76,0.75,0.85,0.86,0.86,0.87,0.86,0.85,0.76,0.74,0.85,0.86,0.86,0.87,0.86,0.85,0.77,0.70,0.84,0.85,0.85,0.85,0.85,0.85,0.77,0.62,0.75,0.75,0.76,0.77,0.77,0.75,0.65},
  //   // Photonis XP85012  9001355
  //   {0.51,0.60,0.61,0.66,0.71,0.71,0.71,0.66,0.72,0.85,0.86,0.87,0.87,0.87,0.85,0.77,0.72,0.86,0.88,0.88,0.89,0.88,0.86,0.76,0.73,0.87,0.88,0.89,0.89,0.88,0.87,0.77,0.76,0.87,0.88,0.89,0.89,0.88,0.87,0.78,0.76,0.86,0.88,0.88,0.88,0.88,0.86,0.77,0.76,0.85,0.86,0.87,0.87,0.86,0.85,0.72,0.63,0.71,0.72,0.76,0.75,0.75,0.76,0.64},
  //   // Photonis XP85012  9001359
  //   {0.57,0.70,0.74,0.75,0.75,0.75,0.75,0.63,0.68,0.82,0.83,0.83,0.83,0.83,0.82,0.71,0.68,0.83,0.84,0.84,0.84,0.84,0.83,0.73,0.69,0.83,0.84,0.85,0.85,0.84,0.84,0.76,0.70,0.83,0.84,0.85,0.85,0.84,0.83,0.74,0.69,0.83,0.84,0.84,0.84,0.84,0.83,0.74,0.68,0.82,0.83,0.83,0.83,0.83,0.82,0.74,0.55,0.69,0.69,0.69,0.69,0.72,0.73,0.63},
  //   // Photonis XP85012  9001338
  //   {0.69,0.81,0.82,0.80,0.78,0.77,0.76,0.66,0.80,0.86,0.87,0.87,0.87,0.87,0.86,0.78,0.84,0.88,0.89,0.89,0.89,0.89,0.87,0.79,0.85,0.88,0.90,0.90,0.90,0.89,0.88,0.77,0.85,0.88,0.90,0.90,0.90,0.89,0.88,0.77,0.84,0.88,0.89,0.89,0.90,0.89,0.88,0.77,0.82,0.86,0.88,0.88,0.88,0.88,0.87,0.77,0.66,0.81,0.83,0.84,0.85,0.85,0.84,0.71},
  //   // Photonis XP85012  9001360
  //   {0.50,0.62,0.63,0.68,0.69,0.67,0.65,0.47,0.64,0.76,0.77,0.82,0.83,0.83,0.82,0.68,0.66,0.76,0.77,0.83,0.84,0.85,0.83,0.71,0.67,0.77,0.77,0.83,0.85,0.85,0.84,0.72,0.66,0.76,0.77,0.80,0.85,0.85,0.83,0.72,0.66,0.76,0.77,0.77,0.83,0.85,0.83,0.72,0.64,0.76,0.76,0.76,0.78,0.83,0.82,0.72,0.56,0.68,0.69,0.70,0.71,0.71,0.72,0.62},
  //   // Photonis XP85012  9001359
  //   {0.57,0.70,0.74,0.75,0.75,0.75,0.75,0.63,0.68,0.82,0.83,0.83,0.83,0.83,0.82,0.71,0.68,0.83,0.84,0.84,0.84,0.84,0.83,0.73,0.69,0.83,0.84,0.85,0.85,0.84,0.84,0.76,0.70,0.83,0.84,0.85,0.85,0.84,0.83,0.74,0.69,0.83,0.84,0.84,0.84,0.84,0.83,0.74,0.68,0.82,0.83,0.83,0.83,0.83,0.82,0.74,0.55,0.69,0.69,0.69,0.69,0.72,0.73,0.63},
  //   // Photonis XP85012  9001359
  //   {0.57,0.70,0.74,0.75,0.75,0.75,0.75,0.63,0.68,0.82,0.83,0.83,0.83,0.83,0.82,0.71,0.68,0.83,0.84,0.84,0.84,0.84,0.83,0.73,0.69,0.83,0.84,0.85,0.85,0.84,0.84,0.76,0.70,0.83,0.84,0.85,0.85,0.84,0.83,0.74,0.69,0.83,0.84,0.84,0.84,0.84,0.83,0.74,0.68,0.82,0.83,0.83,0.83,0.83,0.82,0.74,0.55,0.69,0.69,0.69,0.69,0.72,0.73,0.63}
  // };

  Double_t qe_m2018[8][64]={ 
    // Photonis XP85012  9001352
    {0.65,0.72,0.75,0.77,0.78,0.76,0.77,0.63,0.77,0.83,0.84,0.85,0.85,0.85,0.84,0.75,0.78,0.84,0.85,0.86,0.86,0.86,0.85,0.80,0.79,0.84,0.86,0.87,0.87,0.87,0.85,0.81,0.79,0.84,0.86,0.87,0.87,0.86,0.85,0.82,0.78,0.83,0.85,0.86,0.86,0.86,0.85,0.82,0.76,0.82,0.84,0.85,0.85,0.84,0.83,0.81,0.67,0.77,0.79,0.81,0.80,0.78,0.77,0.71},
    // Photonis XP85012  9001358
    {0.63,0.71,0.71,0.71,0.71,0.71,0.70,0.66,0.67,0.74,0.73,0.73,0.72,0.72,0.72,0.71,0.69,0.75,0.75,0.74,0.74,0.74,0.74,0.72,0.72,0.82,0.76,0.76,0.74,0.74,0.74,0.72,0.73,0.82,0.81,0.80,0.75,0.73,0.73,0.71,0.74,0.81,0.81,0.80,0.77,0.73,0.72,0.70,0.73,0.84,0.84,0.84,0.81,0.76,0.73,0.71,0.62,0.75,0.75,0.77,0.77,0.72,0.67,0.63},
    // Photonis XP85012  9001356
    {0.61,0.76,0.78,0.79,0.80,0.80,0.80,0.65,0.72,0.89,0.90,0.90,0.91,0.91,0.91,0.80,0.72,0.88,0.90,0.90,0.91,0.91,0.91,0.80,0.71,0.88,0.89,0.90,0.91,0.91,0.91,0.81,0.69,0.89,0.90,0.91,0.91,0.91,0.91,0.81,0.68,0.89,0.90,0.91,0.91,0.91,0.91,0.78,0.70,0.88,0.89,0.90,0.90,0.90,0.90,0.77,0.66,0.78,0.79,0.76,0.75,0.76,0.79,0.67},
    // Photonis XP85012  9001336
    {0.58,0.71,0.72,0.74,0.76,0.77,0.77,0.69,0.65,0.81,0.82,0.82,0.82,0.81,0.80,0.75,0.65,0.82,0.83,0.83,0.83,0.82,0.81,0.76,0.64,0.82,0.83,0.83,0.83,0.83,0.82,0.77,0.65,0.82,0.83,0.83,0.83,0.83,0.82,0.77,0.66,0.81,0.83,0.83,0.83,0.83,0.82,0.76,0.65,0.80,0.82,0.82,0.82,0.82,0.81,0.76,0.57,0.71,0.70,0.71,0.72,0.73,0.72,0.66},
    // Photonis XP85012  9001355
    {0.51,0.72,0.72,0.73,0.76,0.76,0.76,0.63,0.60,0.85,0.86,0.87,0.87,0.86,0.85,0.71,0.61,0.86,0.88,0.88,0.88,0.88,0.86,0.72,0.66,0.87,0.88,0.89,0.89,0.88,0.87,0.76,0.71,0.87,0.89,0.89,0.89,0.88,0.87,0.75,0.71,0.87,0.88,0.88,0.88,0.88,0.86,0.75,0.71,0.85,0.86,0.87,0.87,0.86,0.85,0.76,0.66,0.77,0.76,0.77,0.78,0.77,0.72,0.64},    
    // Photonis XP85012  9001359
    {0.57,0.68,0.68,0.69,0.70,0.69,0.68,0.55,0.70,0.82,0.83,0.83,0.83,0.83,0.82,0.69,0.74,0.83,0.84,0.84,0.84,0.84,0.83,0.69,0.75,0.83,0.84,0.85,0.85,0.84,0.83,0.69,0.75,0.83,0.84,0.85,0.85,0.84,0.83,0.69,0.75,0.83,0.84,0.84,0.84,0.84,0.83,0.72,0.75,0.82,0.83,0.84,0.83,0.83,0.82,0.73,0.63,0.71,0.73,0.76,0.74,0.74,0.74,0.63},
    // Photonis XP85012  9001360
    {0.50,0.64,0.66,0.67,0.66,0.66,0.64,0.56,0.62,0.76,0.76,0.77,0.76,0.76,0.76,0.68,0.63,0.77,0.77,0.77,0.77,0.77,0.76,0.69,0.68,0.82,0.83,0.83,0.80,0.77,0.76,0.70,0.69,0.83,0.84,0.85,0.85,0.83,0.78,0.71,0.67,0.83,0.85,0.85,0.85,0.85,0.83,0.71,0.65,0.82,0.83,0.84,0.83,0.83,0.82,0.72,0.47,0.68,0.71,0.72,0.72,0.72,0.72,0.62},
   // Photonis XP85012  9001336 // placeholder for 1306
    {0.58,0.71,0.72,0.74,0.76,0.77,0.77,0.69,0.65,0.81,0.82,0.82,0.82,0.81,0.80,0.75,0.65,0.82,0.83,0.83,0.83,0.82,0.81,0.76,0.64,0.82,0.83,0.83,0.83,0.83,0.82,0.77,0.65,0.82,0.83,0.83,0.83,0.83,0.82,0.77,0.66,0.81,0.83,0.83,0.83,0.83,0.82,0.76,0.65,0.80,0.82,0.82,0.82,0.82,0.81,0.76,0.57,0.71,0.70,0.71,0.72,0.73,0.72,0.66}
  };

  Double_t qe_m2017[14][64]={ 
    // Photonis XP85012  9001357
    {0.42,0.66,0.65,0.62,0.60,0.59,0.58,0.37,0.55,0.83,0.85,0.85,0.85,0.85,0.81,0.63,0.56,0.85,0.87,0.87,0.87,0.86,0.84,0.65,0.56,0.84,0.87,0.88,0.87,0.86,0.84,0.65,0.55,0.83,0.87,0.87,0.87,0.86,0.84,0.66,0.57,0.84,0.87,0.87,0.86,0.86,0.83,0.66,0.56,0.82,0.85,0.85,0.85,0.84,0.81,0.63,0.42,0.63,0.65,0.66,0.62,0.61,0.61,0.46},
    // Photonis XP85012  9001352
    {0.65,0.72,0.75,0.77,0.78,0.76,0.77,0.63,0.77,0.83,0.84,0.85,0.85,0.85,0.84,0.75,0.78,0.84,0.85,0.86,0.86,0.86,0.85,0.80,0.79,0.84,0.86,0.87,0.87,0.87,0.85,0.81,0.79,0.84,0.86,0.87,0.87,0.86,0.85,0.82,0.78,0.83,0.85,0.86,0.86,0.86,0.85,0.82,0.76,0.82,0.84,0.85,0.85,0.84,0.83,0.81,0.67,0.77,0.79,0.81,0.80,0.78,0.77,0.71},
    // Photonis XP85012  9001358
    {0.63,0.71,0.71,0.71,0.71,0.71,0.70,0.66,0.67,0.74,0.73,0.73,0.72,0.72,0.72,0.71,0.69,0.75,0.75,0.74,0.74,0.74,0.74,0.72,0.72,0.82,0.76,0.76,0.74,0.74,0.74,0.72,0.73,0.82,0.81,0.80,0.75,0.73,0.73,0.71,0.74,0.81,0.81,0.80,0.77,0.73,0.72,0.70,0.73,0.84,0.84,0.84,0.81,0.76,0.73,0.71,0.62,0.75,0.75,0.77,0.77,0.72,0.67,0.63},
    // Photonis XP85012  9001356
    {0.61,0.76,0.78,0.79,0.80,0.80,0.80,0.65,0.72,0.89,0.90,0.90,0.91,0.91,0.91,0.80,0.72,0.88,0.90,0.90,0.91,0.91,0.91,0.80,0.71,0.88,0.89,0.90,0.91,0.91,0.91,0.81,0.69,0.89,0.90,0.91,0.91,0.91,0.91,0.81,0.68,0.89,0.90,0.91,0.91,0.91,0.91,0.78,0.70,0.88,0.89,0.90,0.90,0.90,0.90,0.77,0.66,0.78,0.79,0.76,0.75,0.76,0.79,0.67},
    // Photonis XP85012  9001336
    {0.58,0.71,0.72,0.74,0.76,0.77,0.77,0.69,0.65,0.81,0.82,0.82,0.82,0.81,0.80,0.75,0.65,0.82,0.83,0.83,0.83,0.82,0.81,0.76,0.64,0.82,0.83,0.83,0.83,0.83,0.82,0.77,0.65,0.82,0.83,0.83,0.83,0.83,0.82,0.77,0.66,0.81,0.83,0.83,0.83,0.83,0.82,0.76,0.65,0.80,0.82,0.82,0.82,0.82,0.81,0.76,0.57,0.71,0.70,0.71,0.72,0.73,0.72,0.66},
    // Photonis XP85012  9001339
    {0.61,0.73,0.75,0.75,0.75,0.74,0.70,0.62,0.72,0.83,0.84,0.85,0.85,0.85,0.84,0.75,0.75,0.83,0.85,0.86,0.86,0.86,0.85,0.75,0.76,0.84,0.85,0.86,0.86,0.86,0.85,0.76,0.77,0.84,0.86,0.86,0.87,0.87,0.85,0.77,0.77,0.84,0.86,0.86,0.86,0.86,0.85,0.77,0.76,0.83,0.85,0.85,0.85,0.85,0.85,0.75,0.66,0.75,0.76,0.76,0.76,0.77,0.77,0.65},
    // Photonis XP85012  9001355
    {0.51,0.72,0.72,0.73,0.76,0.76,0.76,0.63,0.60,0.85,0.86,0.87,0.87,0.86,0.85,0.71,0.61,0.86,0.88,0.88,0.88,0.88,0.86,0.72,0.66,0.87,0.88,0.89,0.89,0.88,0.87,0.76,0.71,0.87,0.89,0.89,0.89,0.88,0.87,0.75,0.71,0.87,0.88,0.88,0.88,0.88,0.86,0.75,0.71,0.85,0.86,0.87,0.87,0.86,0.85,0.76,0.66,0.77,0.76,0.77,0.78,0.77,0.72,0.64},
    // Photonis XP85012  9001359
    {0.57,0.68,0.68,0.69,0.70,0.69,0.68,0.55,0.70,0.82,0.83,0.83,0.83,0.83,0.82,0.69,0.74,0.83,0.84,0.84,0.84,0.84,0.83,0.69,0.75,0.83,0.84,0.85,0.85,0.84,0.83,0.69,0.75,0.83,0.84,0.85,0.85,0.84,0.83,0.69,0.75,0.83,0.84,0.84,0.84,0.84,0.83,0.72,0.75,0.82,0.83,0.84,0.83,0.83,0.82,0.73,0.63,0.71,0.73,0.76,0.74,0.74,0.74,0.63},
    // Photonis XP85012  9001338
    {0.69,0.80,0.84,0.85,0.85,0.84,0.82,0.66,0.81,0.86,0.88,0.88,0.88,0.88,0.86,0.81,0.82,0.87,0.89,0.90,0.90,0.89,0.88,0.83,0.80,0.87,0.89,0.90,0.90,0.89,0.88,0.84,0.78,0.87,0.89,0.90,0.90,0.90,0.88,0.85,0.77,0.87,0.89,0.89,0.89,0.89,0.88,0.85,0.76,0.86,0.87,0.88,0.88,0.88,0.87,0.84,0.66,0.78,0.79,0.77,0.77,0.77,0.77,0.71},
    // Photonis XP85012  9001360
    {0.50,0.64,0.66,0.67,0.66,0.66,0.64,0.56,0.62,0.76,0.76,0.77,0.76,0.76,0.76,0.68,0.63,0.77,0.77,0.77,0.77,0.77,0.76,0.69,0.68,0.82,0.83,0.83,0.80,0.77,0.76,0.70,0.69,0.83,0.84,0.85,0.85,0.83,0.78,0.71,0.67,0.83,0.85,0.85,0.85,0.85,0.83,0.71,0.65,0.82,0.83,0.84,0.83,0.83,0.82,0.72,0.47,0.68,0.71,0.72,0.72,0.72,0.72,0.62},
    // Photonis XP85012  9001359
    {0.57,0.68,0.68,0.69,0.70,0.69,0.68,0.55,0.70,0.82,0.83,0.83,0.83,0.83,0.82,0.69,0.74,0.83,0.84,0.84,0.84,0.84,0.83,0.69,0.75,0.83,0.84,0.85,0.85,0.84,0.83,0.69,0.75,0.83,0.84,0.85,0.85,0.84,0.83,0.69,0.75,0.83,0.84,0.84,0.84,0.84,0.83,0.72,0.75,0.82,0.83,0.84,0.83,0.83,0.82,0.73,0.63,0.71,0.73,0.76,0.74,0.74,0.74,0.63},
    // Photonis XP85012  9001359
    {0.57,0.68,0.68,0.69,0.70,0.69,0.68,0.55,0.70,0.82,0.83,0.83,0.83,0.83,0.82,0.69,0.74,0.83,0.84,0.84,0.84,0.84,0.83,0.69,0.75,0.83,0.84,0.85,0.85,0.84,0.83,0.69,0.75,0.83,0.84,0.85,0.85,0.84,0.83,0.69,0.75,0.83,0.84,0.84,0.84,0.84,0.83,0.72,0.75,0.82,0.83,0.84,0.83,0.83,0.82,0.73,0.63,0.71,0.73,0.76,0.74,0.74,0.74,0.63},
  };

  //all
  // Double_t qe_m2017[14][64]={
  //   Photonis XP85012  9000411 {0.69,0.81,0.82,0.84,0.85,0.83,0.83,0.77,0.72,0.85,0.87,0.88,0.89,0.88,0.86,0.81,0.72,0.86,0.88,0.89,0.89,0.88,0.87,0.83,0.76,0.86,0.88,0.89,0.89,0.88,0.87,0.81,0.78,0.86,0.88,0.89,0.89,0.89,0.87,0.80,0.79,0.86,0.88,0.89,0.89,0.88,0.87,0.81,0.78,0.85,0.87,0.88,0.89,0.88,0.86,0.82,0.59,0.68,0.74,0.76,0.78,0.76,0.75,0.69},
  //   Photonis XP85012  9000412 {0.72,0.81,0.79,0.78,0.79,0.80,0.83,0.75,0.85,0.87,0.87,0.87,0.87,0.87,0.87,0.84,0.83,0.88,0.88,0.87,0.87,0.87,0.87,0.85,0.78,0.88,0.88,0.88,0.88,0.86,0.87,0.86,0.82,0.88,0.88,0.88,0.88,0.87,0.87,0.86,0.83,0.89,0.88,0.87,0.87,0.87,0.87,0.86,0.82,0.89,0.89,0.88,0.87,0.87,0.88,0.85,0.73,0.86,0.87,0.87,0.85,0.87,0.84,0.79},
  //   Photonis XP85012  9000765 {0.64,0.73,0.77,0.78,0.79,0.81,0.81,0.70,0.72,0.80,0.82,0.82,0.81,0.79,0.77,0.71,0.68,0.77,0.77,0.77,0.76,0.75,0.74,0.70,0.65,0.75,0.76,0.75,0.74,0.73,0.73,0.70,0.68,0.74,0.75,0.75,0.74,0.74,0.75,0.69,0.72,0.77,0.76,0.77,0.77,0.76,0.77,0.69,0.73,0.80,0.80,0.81,0.81,0.80,0.80,0.74,0.63,0.73,0.73,0.74,0.75,0.75,0.74,0.64},
  //   Photonis XP85012  9000766 {0.64,0.81,0.79,0.79,0.81,0.82,0.83,0.65,0.76,0.91,0.92,0.92,0.91,0.90,0.89,0.73,0.77,0.90,0.92,0.92,0.91,0.90,0.88,0.74,0.77,0.91,0.92,0.93,0.92,0.91,0.89,0.79,0.77,0.91,0.93,0.93,0.93,0.92,0.90,0.82,0.76,0.91,0.93,0.94,0.94,0.93,0.92,0.84,0.74,0.90,0.92,0.93,0.94,0.94,0.93,0.85,0.62,0.78,0.79,0.82,0.81,0.78,0.78,0.67},
  //   Photonis XP85012  9001336 {0.58,0.71,0.72,0.74,0.76,0.77,0.77,0.69,0.65,0.81,0.82,0.82,0.82,0.81,0.80,0.75,0.65,0.82,0.83,0.83,0.83,0.82,0.81,0.76,0.64,0.82,0.83,0.83,0.83,0.83,0.82,0.77,0.65,0.82,0.83,0.83,0.83,0.83,0.82,0.77,0.66,0.81,0.83,0.83,0.83,0.83,0.82,0.76,0.65,0.80,0.82,0.82,0.82,0.82,0.81,0.76,0.57,0.71,0.70,0.71,0.72,0.73,0.72,0.66},
  //   Photonis XP85012  9001338 {0.69,0.80,0.84,0.85,0.85,0.84,0.82,0.66,0.81,0.86,0.88,0.88,0.88,0.88,0.86,0.81,0.82,0.87,0.89,0.90,0.90,0.89,0.88,0.83,0.80,0.87,0.89,0.90,0.90,0.89,0.88,0.84,0.78,0.87,0.89,0.90,0.90,0.90,0.88,0.85,0.77,0.87,0.89,0.89,0.89,0.89,0.88,0.85,0.76,0.86,0.87,0.88,0.88,0.88,0.87,0.84,0.66,0.78,0.79,0.77,0.77,0.77,0.77,0.71},
  //   Photonis XP85012  9001339 {0.61,0.73,0.75,0.75,0.75,0.74,0.70,0.62,0.72,0.83,0.84,0.85,0.85,0.85,0.84,0.75,0.75,0.83,0.85,0.86,0.86,0.86,0.85,0.75,0.76,0.84,0.85,0.86,0.86,0.86,0.85,0.76,0.77,0.84,0.86,0.86,0.87,0.87,0.85,0.77,0.77,0.84,0.86,0.86,0.86,0.86,0.85,0.77,0.76,0.83,0.85,0.85,0.85,0.85,0.85,0.75,0.66,0.75,0.76,0.76,0.76,0.77,0.77,0.65},
  //   Photonis XP85012  9001352 {0.65,0.72,0.75,0.77,0.78,0.76,0.77,0.63,0.77,0.83,0.84,0.85,0.85,0.85,0.84,0.75,0.78,0.84,0.85,0.86,0.86,0.86,0.85,0.80,0.79,0.84,0.86,0.87,0.87,0.87,0.85,0.81,0.79,0.84,0.86,0.87,0.87,0.86,0.85,0.82,0.78,0.83,0.85,0.86,0.86,0.86,0.85,0.82,0.76,0.82,0.84,0.85,0.85,0.84,0.83,0.81,0.67,0.77,0.79,0.81,0.80,0.78,0.77,0.71},
  //   Photonis XP85012  9001353 {0.63,0.69,0.71,0.72,0.73,0.74,0.74,0.66,0.71,0.74,0.75,0.75,0.75,0.75,0.76,0.71,0.72,0.75,0.75,0.75,0.75,0.75,0.75,0.70,0.74,0.77,0.77,0.77,0.76,0.75,0.75,0.71,0.75,0.78,0.79,0.79,0.78,0.76,0.76,0.73,0.75,0.78,0.79,0.79,0.79,0.78,0.77,0.75,0.75,0.78,0.79,0.79,0.79,0.79,0.78,0.76,0.68,0.73,0.74,0.75,0.74,0.72,0.71,0.67},
  //   Photonis XP85012  9001355 {0.51,0.72,0.72,0.73,0.76,0.76,0.76,0.63,0.60,0.85,0.86,0.87,0.87,0.86,0.85,0.71,0.61,0.86,0.88,0.88,0.88,0.88,0.86,0.72,0.66,0.87,0.88,0.89,0.89,0.88,0.87,0.76,0.71,0.87,0.89,0.89,0.89,0.88,0.87,0.75,0.71,0.87,0.88,0.88,0.88,0.88,0.86,0.75,0.71,0.85,0.86,0.87,0.87,0.86,0.85,0.76,0.66,0.77,0.76,0.77,0.78,0.77,0.72,0.64},
  //   Photonis XP85012  9001356 {0.61,0.76,0.78,0.79,0.80,0.80,0.80,0.65,0.72,0.89,0.90,0.90,0.91,0.91,0.91,0.80,0.72,0.88,0.90,0.90,0.91,0.91,0.91,0.80,0.71,0.88,0.89,0.90,0.91,0.91,0.91,0.81,0.69,0.89,0.90,0.91,0.91,0.91,0.91,0.81,0.68,0.89,0.90,0.91,0.91,0.91,0.91,0.78,0.70,0.88,0.89,0.90,0.90,0.90,0.90,0.77,0.66,0.78,0.79,0.76,0.75,0.76,0.79,0.67},
  //   Photonis XP85012  9001357 {0.42,0.66,0.65,0.62,0.60,0.59,0.58,0.37,0.55,0.83,0.85,0.85,0.85,0.85,0.81,0.63,0.56,0.85,0.87,0.87,0.87,0.86,0.84,0.65,0.56,0.84,0.87,0.88,0.87,0.86,0.84,0.65,0.55,0.83,0.87,0.87,0.87,0.86,0.84,0.66,0.57,0.84,0.87,0.87,0.86,0.86,0.83,0.66,0.56,0.82,0.85,0.85,0.85,0.84,0.81,0.63,0.42,0.63,0.65,0.66,0.62,0.61,0.61,0.46},
  //   Photonis XP85012  9001358 {0.63,0.71,0.71,0.71,0.71,0.71,0.70,0.66,0.67,0.74,0.73,0.73,0.72,0.72,0.72,0.71,0.69,0.75,0.75,0.74,0.74,0.74,0.74,0.72,0.72,0.82,0.76,0.76,0.74,0.74,0.74,0.72,0.73,0.82,0.81,0.80,0.75,0.73,0.73,0.71,0.74,0.81,0.81,0.80,0.77,0.73,0.72,0.70,0.73,0.84,0.84,0.84,0.81,0.76,0.73,0.71,0.62,0.75,0.75,0.77,0.77,0.72,0.67,0.63},
  //   Photonis XP85012  9001359 {0.57,0.68,0.68,0.69,0.70,0.69,0.68,0.55,0.70,0.82,0.83,0.83,0.83,0.83,0.82,0.69,0.74,0.83,0.84,0.84,0.84,0.84,0.83,0.69,0.75,0.83,0.84,0.85,0.85,0.84,0.83,0.69,0.75,0.83,0.84,0.85,0.85,0.84,0.83,0.69,0.75,0.83,0.84,0.84,0.84,0.84,0.83,0.72,0.75,0.82,0.83,0.84,0.83,0.83,0.82,0.73,0.63,0.71,0.73,0.76,0.74,0.74,0.74,0.63},
  //   Photonis XP85012  9001360 {0.50,0.64,0.66,0.67,0.66,0.66,0.64,0.56,0.62,0.76,0.76,0.77,0.76,0.76,0.76,0.68,0.63,0.77,0.77,0.77,0.77,0.77,0.76,0.69,0.68,0.82,0.83,0.83,0.80,0.77,0.76,0.70,0.69,0.83,0.84,0.85,0.85,0.83,0.78,0.71,0.67,0.83,0.85,0.85,0.85,0.85,0.83,0.71,0.65,0.82,0.83,0.84,0.83,0.83,0.82,0.72,0.47,0.68,0.71,0.72,0.72,0.72,0.72,0.62}
  //   Photonis XP85012  9001392 {0.66,0.77,0.79,0.79,0.78,0.77,0.77,0.69,0.87,0.93,0.93,0.93,0.93,0.92,0.92,0.81,0.87,0.93,0.93,0.93,0.93,0.93,0.92,0.82,0.86,0.92,0.93,0.93,0.93,0.93,0.91,0.83,0.85,0.93,0.93,0.93,0.93,0.92,0.91,0.84,0.84,0.93,0.93,0.93,0.92,0.92,0.90,0.84,0.82,0.92,0.92,0.92,0.91,0.91,0.89,0.83,0.72,0.84,0.86,0.86,0.84,0.83,0.80,0.74},
  // }
		

  
  Int_t layout = PrtManager::Instance()->GetMcpLayout();   
  if(layout==2015){
    for(auto m=0; m<15; m++){
      for(auto i=0; i<64; i++){
	fQe_space[m][i]=qe_m2015[m][i];
      }
    }
  }else if(layout==2016){
    for(auto m=0; m<9; m++){
      for(auto i=0; i<64; i++){
	fQe_space[m][i]=qe_m2016[m][i];
      }
    }
  }else if(layout==2018){
    for(auto m=0; m<8; m++){
      for(auto i=0; i<64; i++){
	fQe_space[m][i]=qe_m2018[m][i];
      }
    } 
  }else{
    for(auto m=0; m<14; m++){
      for(auto i=0; i<64; i++){
	fQe_space[m][i]=qe_m2017[m][i];
      }
    }
  }
  
  
 // TTree *gTree = new TTree("Prt","Prototype hits tree");
 // Event *fHit = 0;
 // gTree->Branch("event", "Event", &event, 64000, 0);

  // // Create hits collection
  // fHitsCollection 
  //   = new B4cCalorHitsCollection(SensitiveDetectorName, collectionName[0]); 

  // // Add this collection in hce
  // G4int hcID 
  //   = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  // hce->AddHitsCollection( hcID, fHitsCollection ); 

  // // Create hits
  // // fNofCells for cells + one more for total sums 
  // for (G4int i=0; i<fNofCells+1; i++ ) {
  //   fHitsCollection->insert(new B4cCalorHit());
  // }
 
  //PrtManager::Instance()->AddEvent(PrtEvent());
}

G4bool PrtPixelSD::ProcessHits(G4Step* step, G4TouchableHistory* hist){
  
  // // energy deposit
  // G4double edep = step->GetTotalEnergyDeposit();
  
  // // step length
  // G4double stepLength = 0.;
  // if ( step->GetTrack()->GetDefinition()->GetPDGCharge() != 0. ) {
  //   stepLength = step->GetStepLength();
  // }

  // if ( edep==0. && stepLength == 0. ) return false;     

  if(step == 0) return false;
 
  //G4ThreeVector translation = hist->GetTranslation();
  //G4ThreeVector localpos = step->GetPreStepPoint()->GetPhysicalVolume()->GetObjectTranslation();
  G4TouchableHistory* touchable = (G4TouchableHistory*)(step->GetPostStepPoint()->GetTouchable());

  // Get cell id 
  G4int layerNumber = touchable->GetReplicaNumber(0);
  //G4cout<< " PixelId = "<<layerNumber << G4endl;
  G4Track* track = step->GetTrack();
  const G4DynamicParticle* dynParticle = track->GetDynamicParticle();
  G4ParticleDefinition* particle = dynParticle->GetDefinition();  
  G4String ParticleName = particle->GetParticleName();

  // auto th = track->GetTouchableHandle();
  //   std::ostringstream oss;
  // G4int depth = th->GetHistoryDepth();
  // for (G4int i = depth; i >= 0; --i) {
  //   oss << th->GetVolume(i)->GetName()
  // 	<< ':' << th->GetCopyNumber(i);
  //   if (i != 0) oss << '/';
  // }
  // std::cout<<"oss.str() "<<oss.str()<<std::endl;
  
  G4ThreeVector globalpos = step->GetPostStepPoint()->GetPosition();
  G4ThreeVector localpos = touchable->GetHistory()->GetTopTransform().TransformPoint(globalpos);
  G4ThreeVector translation = touchable->GetHistory()->GetTopTransform().Inverse().TransformPoint(G4ThreeVector(0,0,0));
  G4ThreeVector inPrismpos = touchable->GetHistory()->GetTransform(1).TransformPoint(globalpos);

  G4ThreeVector globalvec = track->GetVertexMomentumDirection();
  G4ThreeVector localvec = touchable->GetHistory()->GetTopTransform().TransformAxis(globalvec);

  G4ThreeVector g4mom = track->GetVertexMomentumDirection(); // track->GetMomentum(); 
  G4ThreeVector g4pos = track->GetVertexPosition();
 
  TVector3 globalPos(inPrismpos.x(),inPrismpos.y(),inPrismpos.z());
  TVector3 localPos(localpos.x(),localpos.y(),localpos.z());
    
  if(PrtManager::Instance()->GetRunType() == 6){ //focal plane scan
    globalPos = TVector3(globalpos.x(),globalpos.y(),globalpos.z());
    localPos = TVector3(g4pos.x(),g4pos.y(),g4pos.z());
  }
  localPos = TVector3(g4pos.x(),g4pos.y(),g4pos.z());  
  
  translation=touchable->GetHistory()->GetTransform(1).TransformPoint(translation);
  TVector3 digiPos(translation.x(),translation.y(),translation.z());
  TVector3 momentum(g4mom.x(),g4mom.y(),g4mom.z());
  G4ThreeVector lp = touchable->GetHistory()->GetTransform(1).TransformPoint(g4pos); //pos in wDirc
  TVector3 position(lp.x(),lp.y(),lp.z());

  // information from prizm
  G4SDManager* fSDM = G4SDManager::GetSDMpointer();
  G4RunManager* fRM = G4RunManager::GetRunManager();
  G4int collectionID = fSDM->GetCollectionID("PrizmHitsCollection");
  const G4Event* currentEvent = fRM->GetCurrentEvent();
  G4HCofThisEvent* HCofEvent = currentEvent->GetHCofThisEvent();
  PrtPrizmHitsCollection* prizmCol = (PrtPrizmHitsCollection*)(HCofEvent->GetHC(collectionID));
 
  Double_t pathId = 0;
  Int_t refl=0;
  for (G4int i=0;i<prizmCol->entries();i++){
    PrtPrizmHit* phit = (*prizmCol)[i];
    if(phit->GetTrackID()==track->GetTrackID()) {
      refl++;
      pathId += phit->GetNormalId()*1000*refl;
    }
  }

    
  PrtHit hit;
  Int_t mcpid=touchable->GetReplicaNumber(1);
  Int_t pixid = touchable->GetReplicaNumber(0);
  
  hit.SetMcpId(mcpid);
  hit.SetPixelId(pixid);
  // hit.SetGlobalPos(globalPos);
  // hit.SetLocalPos(localPos);
  // hit.SetDigiPos(digiPos);
  // hit.SetPosition(globalPos);
  // hit.SetMomentum(momentum);
  if(PrtManager::Instance()->GetRunType()==6){
    G4ThreeVector mominend = step->GetPostStepPoint()->GetMomentum();
    TVector3 mominendv(mominend.x(),mominend.y(),mominend.z());
    //hit.SetMomentum(mominendv);
  }
  hit.SetParticleId(track->GetTrackID());
  hit.SetParentParticleId(track->GetParentID());
  hit.SetNreflectionsInPrizm(refl-1);
  hit.SetPathInPrizm(pathId);
  hit.SetCherenkovMC(PrtManager::Instance()->GetCurrentCherenkov());

  // // get normal to pixel surface --------------------------
  // G4Navigator* theNavigator 
  //   = G4TransportationManager::GetTransportationManager()
  //   ->GetNavigatorForTracking();
  // Double_t normalId = 0;
  // G4bool valid;
  // G4ThreeVector theLocalNormal = theNavigator->GetLocalExitNormal(&valid);
  // if (valid ){
  //   G4ThreeVector pmom = -touchable->GetHistory()->GetTransform(1).TransformAxis(g4mom);
  //   double angle = theLocalNormal.angle(pmom);
  //   hit.SetPosition(TVector3(pmom.x(),pmom.y(),pmom.z()));
  //   hit.SetMultiplicity(1000*angle*TMath::RadToDeg()); // mdeg
  // }
  // // ------------------------------------------------------

  // time since track created
  G4double time = step->GetPreStepPoint()->GetLocalTime();
  if(PrtManager::Instance()->GetRunType()==0) time = G4RandGauss::shoot(time,PrtManager::Instance()->GetTimeRes()); //resolution [ns]
  hit.SetLeadTime(time);
  Double_t wavelength = 1.2398/(track->GetMomentum().mag()*1E6)*1000;
  hit.SetTotTime(wavelength); //set photon wavelength
  // time since event created
  // step->GetPreStepPoint()->GetGlobalTime()*1000

  if(PrtManager::Instance()->GetRunType()==1 || PrtManager::Instance()->GetRunType()==5 || PrtManager::Instance()->GetRunType()==11 || PrtManager::Instance()->GetRunType()==6){
    PrtManager::Instance()->AddHit(hit,localPos,digiPos,position);
    return true;
  }
  
  Bool_t quantum_efficiency(true);
  Bool_t charge_sharing(true);
  Bool_t dead_time(true);
  Bool_t dark_counts(true);
  Bool_t transport_efficiency(true);
  
  if(PrtManager::Instance()->GetGeometry()==2021) charge_sharing=false; //no cs in mfield
  if(PrtManager::Instance()->GetGeometry()==2030) charge_sharing=false;

  if(transport_efficiency){
    Double_t pi(4*atan(1));
    Double_t roughness(1); //nm
    //std::cout<<"vec  "<<localvec.x()<<" "<<localvec.y() << " " <<localvec.z() <<std::endl;
    
    Double_t angleX = localvec.angle(G4ThreeVector(1,0,0));
    Double_t angleY = localvec.angle(G4ThreeVector(0,1,0));
    Double_t length = track->GetTrackLength()-400;
    Double_t lengthx = fabs(length*localvec.x());
    Double_t lengthy = fabs(length*localvec.y());
      
    Int_t nBouncesX=(Int_t)(lengthx)/17.1; // 17 bar height
    Int_t nBouncesY=(Int_t)(lengthy)/35.9; // 36 bar width
  
    //std::cout<<" angleX  "<< angleX <<"   angleY "<< angleY << " nBouncesX " << nBouncesX << " nBouncesY " << nBouncesY  << "  length "<< length<<std::endl;
    
    
    Double_t ll = wavelength*wavelength;
    Double_t n_quartz = sqrt(1. + (0.696*ll/(ll-pow(0.068,2))) + (0.407*ll/(ll-pow(0.116,2))) + 0.897*ll/(ll-pow(9.896,2)));
    Double_t bounce_probX = 1 - pow(4*pi*cos(angleX)*roughness*n_quartz/wavelength,2);
    Double_t bounce_probY = 1 - pow(4*pi*cos(angleY)*roughness*n_quartz/wavelength,2);

    Double_t totalProb = pow(bounce_probX, nBouncesX)*pow(bounce_probY, nBouncesY);	
    if(G4UniformRand() > totalProb) {
      // std::cout<<"photon lost in the radiator. n_bounces = ["<<nBouncesX<<" "<<nBouncesY<<"] with prob= "<<totalProb<<std::endl;      
      return true;
    }
  }

  Bool_t is_hit(true);
  if(PrtManager::Instance()->GetRunType()==0 && PrtManager::Instance()->GetMcpLayout()>=2015 && quantum_efficiency){
    if(fQe_space[mcpid][pixid]>G4UniformRand()) {
      if(fMultHit[mcpid][pixid]==0 || !dead_time) PrtManager::Instance()->AddHit(hit,localPos,digiPos,position);
      //else std::cout<<"fMultHit["<<mcpid<<"]["<<pixid<<"] "<<fMultHit[mcpid][pixid] <<std::endl;      
      fMultHit[mcpid][pixid]++;
    }else is_hit=false;
    
  }else{
    if(fMultHit[mcpid][pixid]==0 || !dead_time || PrtManager::Instance()->GetMcpLayout()==0) PrtManager::Instance()->AddHit(hit,localPos,digiPos,position);
    fMultHit[mcpid][pixid]++;
  }

  if(is_hit && charge_sharing){
    //charge sharing for 8x8 MCP
    Double_t pixdim(53/16.);
    //Double_t chargesig(1),threshold(0.5);
    Double_t chargesig(1.7),threshold(0.7); //high cs / ct  // 1.5/0.9 for 0.5mV // 1.5/0.7 for 1mV
    // if(mcpid==3){
    //   chargesig=3.2;
    //   threshold=1;
    // }
    // if(mcpid==4){
    //   chargesig=1.7;
    //   threshold=1;
    // }
    Double_t x(localPos.x()), y(localPos.y());
    Int_t p(pixid);
    Bool_t ok(false);
    Double_t expd = exp(-(pixdim-fabs(x))/chargesig);
    
    if(x<0 && pixid%8!=1 && expd>G4UniformRand() && expd<threshold){ok=true; p-=1;}
    if(x>0 && pixid%8!=0 && expd>G4UniformRand() && expd<threshold){ok=true; p+=1;}
    expd = exp(-(pixdim-fabs(y))/chargesig);
    if(y<0 && pixid>8    && expd>G4UniformRand() && expd<threshold){ok=true; p-=8;}
    if(y>0 && pixid<57   && expd>G4UniformRand() && expd<threshold){ok=true; p+=8;}
 
    if(ok) {
      hit.SetPixelId(p);
      if(fMultHit[mcpid][p]==0 || !dead_time) PrtManager::Instance()->AddHit(hit,localPos,digiPos,position);
      fMultHit[mcpid][p]++;
    }
  }

  if(dark_counts){
  }
      
  return true;
}

void PrtPixelSD::EndOfEvent(G4HCofThisEvent*){
  memset(fMultHit, 0, sizeof(fMultHit[0][0])*960);
  G4int eventNumber = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
  if(eventNumber%1==0 && PrtManager::Instance()->GetRunType()==0) std::cout<<"Event # "<<eventNumber <<std::endl;
  if(eventNumber%1000==0 && PrtManager::Instance()->GetRunType()!=0) std::cout<<"Event # "<<eventNumber <<std::endl;
  PrtManager::Instance()->Fill();
}

