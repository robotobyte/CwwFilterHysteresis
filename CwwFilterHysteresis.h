// *****************************************************************************
//
// Hysteresis Filter
// -----------------
// Code by W. Witt; V1.00-beta-02; July 2016
//
// This code defines a configurable hysteresis filter.
//
// Each filter instance may be configured with a different number of
// thresholds, but the threshold count must be at least 1. n thresholds
// create m=n+1 zones, and the filter then compares input values to
// thresholds and accordingly maps each input value to a zone.
//
// Each threshold must be given three values: low, center, high.
// The filter requires that:
// 1) threshold 0 center < threeshold 1 center, etc.
// 2) for each threshold, low < center < high
//
// The filter then maps input values to zones as follows:
//
//           threshold 0 low                 threshold 1 low
//           |   threshold 0 center          |     threshold 1 center
//           |   |   threshold 0 high        |     |     threshold 1 high
//           |   |   |                       |     |     |
//           v   v   v                       v     v     v
//  
//           .   |   .                       .     |     .
//           .   |   .                       .     |     .
//           .   |   .                       .     |     .
//    <==================================================================>
//   low                           Input Value                         high
//               |                                 |
//    <<-------->|<------------------------------->|<------------------->>
//       zone 0                zone 1                       zone 2
//
//       ------------|-----------------------------------|------------>
//       increasing input values are checked against high thresholds
//       to transition from zone m to m+1, the high threshold must be crossed
//
//       <---|-------------------------------|-------------------------
//           decreasing input values are checked against low thresholds
//           from zone m to m-1, the low threshold must be crossed
//
//       center thresholds are used during initialization
//
// *****************************************************************************

#ifndef CwwFilterHysteresis_h
#define CwwFilterHysteresis_h

// *****************************************************************************

#include <Arduino.h>

// =============================================================================

// Define type of values that will be input to the filter...
#define CWW_FH_POINT_TYPE float

// ----------------------------------------------------------------------------

class CwwFilterHysteresis {

  public:

    // Public Types:

    struct structThresholdPoint {
      CWW_FH_POINT_TYPE high;
      CWW_FH_POINT_TYPE center;
      CWW_FH_POINT_TYPE low;
    };

    // Public Constants:

    // Public Variables:

    // Public Functions:

             CwwFilterHysteresis ( uint8_t thresholdCount );
    virtual ~CwwFilterHysteresis ();

    boolean defineThreshold ( uint8_t           thresholdIndex,
                              CWW_FH_POINT_TYPE thPointCenter,
                              CWW_FH_POINT_TYPE offsetAroundCenter
                            );
    boolean defineThreshold ( uint8_t           thresholdIndex,
                              CWW_FH_POINT_TYPE thPointLow,
                              CWW_FH_POINT_TYPE thPointCenter,
                              CWW_FH_POINT_TYPE thPointHigh
                            );

    void    initHistory    ( CWW_FH_POINT_TYPE initValue );
    uint8_t mapValueToZone ( CWW_FH_POINT_TYPE value     );

    CWW_FH_POINT_TYPE lastMappedValue ();
    uint8_t           lastMappedZone  ();

    uint8_t zoneCount ();

  private:

    // Private Constants:

    // Private Variables:

    structThresholdPoint * thresholdList;
    uint8_t                thresholdCount;

    CWW_FH_POINT_TYPE lastValue;
    uint8_t           lastZone;

    // Private Functions:

};

// *****************************************************************************

#endif

// *****************************************************************************
