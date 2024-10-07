//!                   DO NOT .GITIGNORE THIS FILE
//!          THE BUILD NUMBER IS USED TO TELL BUILDS APART

#include "buildNumber.hpp"

// The actual value is set after compiling the code
volatile const char buildNumber[] = "__BUILD_NUMBER_PLACEHOLDER__*__SHA1SUM__";
