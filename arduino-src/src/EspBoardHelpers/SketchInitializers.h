//
// Created by David on 5/26/2023.
//

#ifndef COROLLA_2003_NFC_CONTROLLER_SKETCHINITIALIZERS_H
#define COROLLA_2003_NFC_CONTROLLER_SKETCHINITIALIZERS_H


class SketchInitializers
{
public:

    void Honda2008InitializeRemotePins();
    void Corolla2003InitializeRemotePins();

    void InitializeSpiPins();

    void InitializeImuPins();
};


#endif //COROLLA_2003_NFC_CONTROLLER_SKETCHINITIALIZERS_H
