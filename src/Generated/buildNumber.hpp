//!                  DO NOT .GITIGNORE THIS HEADER
//!          THE BUILD NUMBER IS USED TO TELL BUILDS APART

#pragma once

// The actual value is set after compiling the code
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wattributes"
extern volatile const char buildNumber[] __attribute__((used));
#pragma GCC diagnostic pop
