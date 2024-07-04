//
// Created by David on 5/24/2023.
//

#ifndef COROLLA_2003_NFC_CONTROLLER_SECRETS_H
#define COROLLA_2003_NFC_CONTROLLER_SECRETS_H
#include <vector>
#include <Models/NfcTag.h>

class Secrets
{
public:
    static std::vector<NfcTag> authorizedNfcTags;
};


#endif //COROLLA_2003_NFC_CONTROLLER_SECRETS_H
