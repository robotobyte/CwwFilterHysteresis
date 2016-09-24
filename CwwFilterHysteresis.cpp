// ****************************************************************************
//
// Hysteresis Filter
// -----------------
// Code by W. Witt; V1.00-beta-02; July 2016
//
// This code implemented a configurable hysteresis filter.
//
// ****************************************************************************

#include <limits.h>
#include <Arduino.h>

#include "CwwFilterHysteresis.h"

// ============================================================================
// Constructors, Destructor
// ============================================================================

CwwFilterHysteresis::CwwFilterHysteresis ( uint8_t thresholdCount ) {

  if ( thresholdCount < 1 ) thresholdCount = 1;
  this->thresholdCount = thresholdCount;

  thresholdList = (structThresholdPoint *)
                  malloc ( thresholdCount * sizeof(structThresholdPoint) );

  lastValue = 0;

}

// ----------------------------------------------------------------------------

CwwFilterHysteresis::~CwwFilterHysteresis () {

  free ( thresholdList );

}

// ============================================================================
// Public Functions
// ============================================================================

boolean CwwFilterHysteresis::defineThreshold (
  uint8_t           thresholdIndex,
  CWW_FH_POINT_TYPE thPointCenter,
  CWW_FH_POINT_TYPE offsetAroundCenter
) {

  return defineThreshold (
    thresholdIndex,
    thPointCenter - offsetAroundCenter,
    thPointCenter,
    thPointCenter + offsetAroundCenter
  );

}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

boolean CwwFilterHysteresis::defineThreshold (
  uint8_t           thresholdIndex,
  CWW_FH_POINT_TYPE thPointLow,
  CWW_FH_POINT_TYPE thPointCenter,
  CWW_FH_POINT_TYPE thPointHigh
) {

  boolean pointIsValid;

  pointIsValid = thresholdIndex < thresholdCount;

  if ( pointIsValid ) {
    thresholdList[ thresholdIndex ].high   = thPointHigh;
    thresholdList[ thresholdIndex ].center = thPointCenter;
    thresholdList[ thresholdIndex ].low    = thPointLow;
   }

  return pointIsValid;

}

// ----------------------------------------------------------------------------

void CwwFilterHysteresis::initHistory ( CWW_FH_POINT_TYPE initValue ) {
  
  uint8_t thIndex;
  boolean zoneMatch;

  thIndex = 0;
  zoneMatch = false;
  do {
    zoneMatch = initValue < thresholdList[thIndex].center;
    if ( ! zoneMatch ) thIndex++;
  } while ( ! zoneMatch && thIndex < thresholdCount );

  lastValue = initValue;
  lastZone  = thIndex;  

}

// ----------------------------------------------------------------------------

uint8_t CwwFilterHysteresis::mapValueToZone ( CWW_FH_POINT_TYPE value ) {

  uint8_t           thIndex;
  CWW_FH_POINT_TYPE threshold;
  boolean           zoneMatch;

//if ( value == lastValue ) return lastZone;

  thIndex = 0;
  zoneMatch = false;
  do {
    threshold = thIndex < lastZone 
                ? thresholdList[thIndex].low
                : thresholdList[thIndex].high;
    zoneMatch = lastValue < threshold;
    if ( ! zoneMatch ) thIndex++;
  } while ( ! zoneMatch && thIndex < thresholdCount );

  lastValue = value;
  lastZone  = thIndex;

  return thIndex;

}

// ----------------------------------------------------------------------------

CWW_FH_POINT_TYPE CwwFilterHysteresis::lastMappedValue () {

  return lastValue;

}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

uint8_t CwwFilterHysteresis::lastMappedZone () {

  return lastZone;

}

// ----------------------------------------------------------------------------

uint8_t CwwFilterHysteresis::zoneCount () {

  return thresholdCount + 1;

}

// ****************************************************************************
