#ifndef HELPER_H
#define HELPER_H

#include <iostream>


const std::string linuxFrontendPath = "../../lafayette53/frontend";
const std::string osxFrontendPath = "../../../../../lafayette53/frontend";

const bool usingLinux = true;

std::string getFrontendPath() {
    if (usingLinux) {
        return linuxFrontendPath;
    } else {
        return osxFrontendPath;
    }
}

#endif // HELPER_H
