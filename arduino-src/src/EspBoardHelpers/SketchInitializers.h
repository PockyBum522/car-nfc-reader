//
// Created by David on 5/26/2023.
//

#ifndef COROLLA_2003_NFC_CONTROLLER_SKETCHINITIALIZERS_H
#define COROLLA_2003_NFC_CONTROLLER_SKETCHINITIALIZERS_H


class SketchInitializers
{
public:
    static void InitializeSpiPins();
    static void InitializeRemotePins(const String &whichCar);
};


#endif //COROLLA_2003_NFC_CONTROLLER_SKETCHINITIALIZERS_H
